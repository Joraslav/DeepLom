#include "Mesh.hpp"

Mesh::Mesh(/* args */)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\t" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    vector_type Default{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
    this->ActMesh = Default;
    this->MeshHistory.push_back(ActMesh);
    this->StateCount.resize(ActMesh.size()-1);
    this->Num_States = ActMesh.size();
    Default.clear();
}

Mesh::Mesh(vector_type const& M, Int const& G)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\t" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->ActMesh = M;
    this->MeshHistory.push_back(M);
    this->Goal = G;
    this->StateCount.resize(this->ActMesh.size()-1);
    this->Num_States = ActMesh.size();
}

Mesh::Mesh(sup_st_type &SupPH, vector_type const& M, Int const& G)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\t" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->ActMesh = M;
    this->MeshHistory.push_back(M);
    this->Goal = G;
    this->StateCount.resize(this->ActMesh.size()-1);
    this->Num_States = ActMesh.size();
    SetPath(SupPH);
}

Mesh::Mesh(Mesh const &other)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Coping to " << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

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
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->ActPath = DataPath + '/' + AP;
    this->StCountPath = DataPath + '/' + SCP;
    this->MHistPath = DataPath + '/' + MHP;
}

void Mesh::SetPath(sup_st_type &SupPh)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->ActPath = DataPath + '/' + SupPh[0];
    this->StCountPath = DataPath + '/' + SupPh[1];
    this->MHistPath = DataPath + '/' + SupPh[2];
}

void Mesh::SetCount(Int const &St)
{
    this->StateCount[St]++;
}

auto Mesh::GetState(Real const& Nu) -> Int
{
    auto const nCols{this->ActMesh.size()};
    for (auto i{0u}; i < nCols; i++)
    {
        if (Nu>=ActMesh[i] && Nu<ActMesh[i+1])
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
    size_t Removed{0};
    size_t k{0}; //k=1
    auto LengthStCount = this->StateCount.size();  //n=length(StateCount)

    while (k != LengthStCount)   //k!=n
    {
        if ((k!=Goal-Removed) && (k!=Goal-Removed+1) && (k!=Goal-Removed-1))
        {
            if (this->StateCount[k]==0)
            {
                ActMesh[k+1] = (ActMesh[k]+ActMesh[k+1])/2;
                ActMesh.erase(ActMesh.begin()+k);
                QL[k].clear();
                Removed++;
                this->StateCount.erase(ActMesh.begin()+k);
                k--;
            }
        }
        k++;
        LengthStCount = this->StateCount.size();  //n=length(StateCount)
    }
    
    k = 0;  //k=1
    if (StateCount[k] >= Param*Sum(StateCount))
    {
        ActMesh.insert(ActMesh.begin()+k+1,3*ActMesh[k+1]);
        QL.insert(QL.begin(),QL[0]);
        StateCount.insert(StateCount.begin(),0);
    }

    k = this->StateCount.size();  //k!=n
    if (StateCount[k] >= Param*Sum(StateCount))
    {
        ActMesh.insert(ActMesh.end()-1,3*ActMesh[k-1]);
        QL.insert(QL.end(),QL[0]);
        StateCount.insert(StateCount.end(),0);
    }

    k = 1;  //k=2
    LengthStCount = this->StateCount.size()-2;  //n=length(StateCount) - 2
    auto nCols{QL.front().size()};
    while (k != LengthStCount)
    {
        if (StateCount[k] >= Param*Sum(StateCount))
        {
            ActMesh.insert(ActMesh.begin()+(k+1),(ActMesh[k]+ActMesh[k+1])*0.5);
            for (auto i{0u}; i < nCols; ++i)
            {
                QL[k][i] *= 0.5;
            }
            QL.insert(QL.begin()+(k+1),QL[k]);
            StateCount.insert(StateCount.begin()+(k+1),0);
        }
        k++;
        LengthStCount = this->StateCount.size();  //?????????????????????
    }

    std::sort(this->ActMesh.begin(),this->ActMesh.end());

    return QL;
}
#endif //ADAPTIVE

Mesh::~Mesh()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Mesh\t" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    os_type ActOut(this->ActPath), CountOut(this->StCountPath), HistOut(this->MHistPath);
    ActOut << this->ActMesh;
    CountOut << this->StateCount;
    HistOut << this->MeshHistory;
}