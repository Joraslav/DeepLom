#include "Glob.hpp"
#include "Learning.hpp"
#include "ModelSettings.hpp"

#include <iostream>


Learning::Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma)
{
    std::cout << "Construct\n";
    this->Method = Met;
    this->Eps = Epsilon;
    this->Alf = Alfa;
    this->Gam = Gamma;

}

void Learning::SetModelSettings(z_type &Num, st_type &NameModal)
{
    this->Muscl.Num_Muscles = Num;
    this->Muscl.Modal_Muscl = NameModal;
}

void Learning::SetMesh(vector_type &MeshState)
{
    auto const nCols{MeshState.size()};
    this->Mesh.resize(nCols);

    for (auto i{0u}; i < nCols; ++i)
    {
        this->Mesh[i] = MeshState[i];
    }
}

void Learning::SetTime(vector_type &TimeArr)
{
    this->t0 = TimeArr[0];
    this->dt = TimeArr[1];
    this->Time = TimeArr[2];
}

void Learning::SetPath(st_type &QP, st_type &TrackP, st_type &MeshHistoryP)
{
    st_type path = "../";
    this->QPath = path + QP;
    this->TrackPath = path + TrackP;
    this->MeshHistoryPath = path + MeshHistoryP;
}

void Learning::GenerateQ(z_type const &s, z_type const &a)
{
    this->Q.resize(s);
    for (auto i{0u}; i < Q.size(); ++i)
    {
        this->Q[i].resize(a);
    }
}

auto Learning::GreedyPolicy(z_type &ActState) -> z_type
{
    
}

void Learning::Run(z_type Episode)
{
    z_type Epoch = 1;
    std::cout << "Begin\n" << std::endl;
    while (Epoch != Episode)
    {
        if (Epoch%50==0)
        {
            std::cout << "Epoch  =\t" << Epoch << std::endl;
        }
        for (r_type h = this->t0; h < Time; h=h+dt)
        {
            std::cout << "Check " << h << std::endl;
        }
        Epoch++;
    }
        
}

void Learning::GetQ()
{
    std::cout << this->Q;
}

void Learning::GetState(r_type &Nu)
{
    auto const nCols{this->Mesh.size()};

    for (auto i{0u}; i < nCols; i++)
    {
        if (Nu>=Mesh[i] && Nu<Mesh[i+1])
        {
            this->State = ++i;
            break;
        }
    }
}

void Learning::SetMeshHistory(vector_type &M)
{
    auto LastIndex{this->MeshHistory.size()};
    this->MeshHistory[LastIndex].assign(M.begin(),M.end());
}

Learning::~Learning()
{
    std::cout << "Distructor is Begin" << std::endl;

    os_type QOut(this->QPath), TrackOut(this->TrackPath), MeshHOut{this->MeshHistoryPath};
    QOut << this->Q;
    TrackOut << this->Track;
    MeshHOut << this->MeshHistory;

    auto const QnRows{this->Q.size()};
    // auto const nRows{this->Q.size()}, nCols{this->Q.front().size()};
    for (auto i{0u}; i < QnRows; ++i)
    {
        this->Q[i].clear();
    }
    this->Q.clear();
    QPath.clear();

    auto const TracknRows{this->Track.size()};
    for (auto i{0u}; i < TracknRows; ++i)
    {
        this->Track[i].clear();
    }
    this->Track.clear();
    TrackPath.clear();

    auto const MeshHistorynRows{this->MeshHistory.size()};
    for (auto i{0u}; i < MeshHistorynRows; ++i)
    {
        this->MeshHistory[i].clear();
    }
    this->MeshHistory.clear();
    MeshHistoryPath.clear();
}
