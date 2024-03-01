#include "Glob.hpp"
#include "Learning.hpp"
#include "ModelSettings.hpp"

#include <iostream>

Learning::Learning(vector_type const& Set, ModelSettings const &M) : Muscl(M)
{
    std::cout << "Construct of Learning\n" << this << std::endl;
    this->Eps = Set[0];
    this->Alf = Set[1];
    this->Gam = Set[2];
}

Learning::Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma, ModelSettings const &M) : Muscl(M)
{
    std::cout << "Construct of Learning\n" << this << std::endl;
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

void Learning::SetPath(st_type &QP, st_type &TrackP, st_type &MeshHistoryP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + QP;
    this->TrackPath = DataPath + '/' + TrackP;
    this->MeshHistoryPath = DataPath + '/' + MeshHistoryP;
}

void Learning::GenerateQ(z_type const &s, z_type const &a)
{
    this->Q.resize(s);
    for (auto i{0u}; i < Q.size(); ++i)
    {
        this->Q[i].resize(a);
    }
}



void Learning::RandomQ()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    auto const nRows{this->Q.size()};
    auto const nCols{this->Q.front().size()};

    for (auto i{0u}; i < nRows; ++i)
    {
        for (auto j{0u}; j < nCols; ++j)
        {
            this->Q[i][j] = dist(gen);
        }
    }
}



auto Learning::GreedyPolicy(z_type &ActState) -> z_type
{
    z_type Rez;
    auto Q_iter{this->Q.begin()};
    Q_iter += ActState-1;
    auto Q_vec = *Q_iter;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    if (dist(gen) < this->Eps)
    {
        std::uniform_int_distribution<> dist(0, pow(2,this->Muscl.GetNumMuscles()));
        Rez = dist(gen);
    }
    else
    {
        auto Elem = *std::max_element(begin(Q_vec),end(Q_vec));
        Rez = tls::FindIndex(Q_vec,Elem);
    }
    return Rez;
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
    std::cout << "Distructor of Learning\n" << this << std::endl;

    os_type QOut(this->QPath), TrackOut(this->TrackPath), MeshHOut{this->MeshHistoryPath};
    QOut << this->Q;
    TrackOut << this->Track;
    MeshHOut << this->MeshHistory;
}