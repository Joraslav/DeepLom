#include "Mesh.hpp"

Mesh::Mesh(/* args */)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    vector_type Default{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
    this->ActMesh = Default;
    this->MeshHistory.resize(Default.size());
    this->MeshHistory[0].assign(Default.begin(),Default.end());
    Default.clear();
}

Mesh::Mesh(vector_type const& M, z_type const& G)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Mesh\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->ActMesh = M;

    this->MeshHistory.resize(M.size());
    this->MeshHistory[0].assign(M.begin(),M.end());

    this->Goal = FindIndex(this->ActMesh,G);
    this->StateCount.resize(this->ActMesh.size()-1);
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

void Mesh::SetCount(z_type const &St)
{
    this->StateCount[St]++;
}

auto Mesh::GetMesh() const -> vector_type
{
    return this->ActMesh;
}

auto Mesh::GetGoal() const -> z_type
{
    return this->Goal;
}

#ifdef ADAPTIVE
auto Mesh::Adaptive(matrix_type &QL, z_type const Param) -> matrix_type
{
    sz_type Removed{0};
    sz_type k{0}; //k=1
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
    std::cout << "Distruct of Mesh\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    os_type ActOut(this->ActPath), CountOut(this->StCountPath), HistOut(this->MHistPath);
    ActOut << this->ActMesh;
    CountOut << this->StateCount;
    HistOut << this->MeshHistory;
}