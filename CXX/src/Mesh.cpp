#include "Mesh.hpp"

Mesh::Mesh(st_type const &Mthd)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    vector_type Default{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
    this->ActMesh = Default;
    this->MeshHistory.push_back(ActMesh);
    this->StateCount.resize(ActMesh.size() - 1);
    this->Num_States = ActMesh.size();
    this->Method = Mthd;
    Default.clear();
}

Mesh::Mesh(vector_type const &M, st_type const &Mthd, Int const &G)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->ActMesh = M;
    this->MeshHistory.push_back(M);
    this->Goal = G;
    this->StateCount.resize(this->ActMesh.size() - 1);
    this->Num_States = ActMesh.size();
    this->Method = Mthd;
}

Mesh::Mesh(sup_st_type &SupPH, st_type const &Mthd, vector_type const &M, Int const &G)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->ActMesh = M;
    this->MeshHistory.push_back(M);
    this->Goal = G;
    this->StateCount.resize(this->ActMesh.size() - 1);
    this->Num_States = ActMesh.size();
    this->Method = Mthd;
    SetPath(SupPH);
}

Mesh::Mesh(Mesh const &other)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Coping to " << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->ActMesh = other.ActMesh;
    this->Goal = other.Goal;
    this->MeshHistory = other.MeshHistory;
    this->StateCount = other.StateCount;
    this->ActPath = other.ActPath;
    this->StCountPath = other.StCountPath;
    this->MHistPath = other.MHistPath;
    this->Num_States = other.Num_States;
}

void Mesh::SetPath(st_type &AP, st_type &SCP, st_type &MHP)
{
    st_type DataPath = "../" + Method + '_' + "Data";
    mkdir(DataPath.c_str());
    this->ActPath = DataPath + '/' + AP;
    this->StCountPath = DataPath + '/' + SCP;
    this->MHistPath = DataPath + '/' + MHP;
}

void Mesh::SetPath(sup_st_type &SupPh)
{
    st_type DataPath = "../" + Method + '_' + "Data";
    mkdir(DataPath.c_str());
    this->ActPath = DataPath + '/' + SupPh[0];
    this->StCountPath = DataPath + '/' + SupPh[1];
    this->MHistPath = DataPath + '/' + SupPh[2];
    this->SCHistPath = DataPath + '/' + SupPh[3];
}

void Mesh::SetCount(Int const &St)
{
    this->StateCount[St]++;
}

auto Mesh::GetState(Real const &Nu) -> Int
{
    auto const nCols{this->ActMesh.size()};
    for (auto i{0u}; i < nCols; i++)
    {
        if (Nu >= ActMesh[i] && Nu < ActMesh[i + 1])
        {
            this->Active_State = i;
            this->SetCount(Active_State);
            break;
        }
    }
    return this->Active_State;
}

auto Mesh::GetMesh() -> vector_type
{
    return this->ActMesh;
}

auto Mesh::GetGoal() const -> Int
{
    return this->Goal;
}

auto Mesh::GetNumState() -> Int
{
    return this->Num_States;
}

#ifdef ADAPTIVE
auto Mesh::Adaptive(matrix_type &QL, Real const Param) -> matrix_type
{
    this->StateCountHistory.push_back(StateCount);
    vector_type Zeros;
    Zeros = FindIndex(StateCount, is_zero);
    if (!Zeros.empty())
    {
        Int Remove{0};
        for (auto &index_zero : Zeros)
        {
            if ((index_zero != Goal) && (index_zero != Goal - 1) && (index_zero != Goal + 1))
            {
                ActMesh[index_zero + 1 - Remove] = (ActMesh[index_zero - Remove] + ActMesh[index_zero + 1 - Remove]) * 0.5;
                ActMesh.erase(ActMesh.begin() + index_zero - Remove);
                QL.erase(QL.begin() + (index_zero - Remove));
                StateCount.erase(StateCount.begin() + (index_zero - Remove));
                Remove++;
            }
        }
    }
    Real SumElem = accumulate(StateCount.begin(), StateCount.end(), 0);
    vector_type Good_Pos;
    for (auto i{0u}; i < StateCount.size(); ++i)
    {
        if (StateCount[i] >= Param * SumElem)
        {
            Good_Pos.push_back(i);
        }
    }
    if (!Good_Pos.empty())
    {
        auto index_min_inf = FindIndex(ActMesh, -INFINITY);
        auto index_max_inf = FindIndex(ActMesh, INFINITY);
        Int Move{0};
        Real half{0.5};
        for (auto &index_g_p : Good_Pos)
        {
            if (index_g_p == index_min_inf)
            {
                ActMesh.insert(ActMesh.begin() + 1, 3 * ActMesh[index_g_p + 1]);
                QL.insert(QL.begin(), vector_type(QL.front().size(), 0));
                StateCount.insert(StateCount.begin(), 0);
                Move++;
            }
            else if (index_g_p == index_max_inf - 1)
            {
                ActMesh.insert(ActMesh.end() - 1, 3 * ActMesh[index_g_p + Move]);
                QL.insert(QL.end(), vector_type(QL.back().size(), 0));
                StateCount.push_back(0);
            }
            else
            {
                ActMesh.insert(ActMesh.begin() + index_g_p + Move + 1, (ActMesh[index_g_p + Move] + ActMesh[index_g_p + Move + 1]) * half);
                QL[index_g_p + Move] = half * QL[index_g_p + Move];
                QL.insert(QL.begin() + index_g_p + Move, QL[index_g_p + Move]);
                StateCount.insert(StateCount.begin() + index_g_p + Move, 0);
                Move++;
            }
        }
    }
    this->MeshHistory.push_back(ActMesh);
    this->StateCount.clear();
    this->StateCount.resize(this->ActMesh.size() - 1);
    this->Goal = FindIndex(ActMesh,0);
    return QL;
}
#endif // ADAPTIVE

Mesh::~Mesh()
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Mesh\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    os_type ActOut(this->ActPath), CountOut(this->StCountPath), HistOut(this->MHistPath);
    ActOut << this->ActMesh;
    CountOut << this->StateCount;
    HistOut << this->MeshHistory;
}