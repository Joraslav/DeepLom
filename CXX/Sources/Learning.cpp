#include "Learning.hpp"
#include "Glob.hpp"

#include <iostream>


Learning::Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma)
{
    std::cout << "Construct\n";
    this->Method = Met;
    this->Eps = Epsilon;
    this->Alf = Alfa;
    this->Gam = Gamma;

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

void Learning::SetPath(st_type &QP, st_type &TrackP)
{
    this->QPath = QP;
    this->TrackPath = TrackP;
}

void Learning::GenerateQ(z_type const &s, z_type const &a)
{
    this->Q.resize(s);
    for (auto i{0u}; i < Q.size(); ++i)
    {
        this->Q[i].resize(a);
    }
}

void Learning::Run(z_type Episode)
{
    z_type Epoch = 1;
    std::cout << "Begin\n" << std::endl;
    while (Epoch != Episode)
    {
        for (r_type h = this->t0; h < Time; h=h+dt)
        {
            if (Epoch%50==0)
            {
                std::cout << "Epoch  =\t" << Epoch << std::endl;
            }
            
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
        if (Nu>=Mesh[i] && Nu<=Mesh[i+1])
        {
            this->State = i;
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

    os_type QOut(this->QPath), TrackOut(this->TrackPath);
    QOut << this->Q;
    TrackOut << this->Track;

    auto const QnRows{this->Q.size()};
    // auto const nRows{this->Q.size()}, nCols{this->Q.front().size()};
    for (auto i{0u}; i < QnRows; ++i)
    {
        this->Q[i].clear();
    }
    this->Q.clear();

    auto const TracknRows{this->Track.size()};
    for (auto i{0u}; i < TracknRows; ++i)
    {
        this->Track[i].clear();
    }
    this->Track.clear();
}
