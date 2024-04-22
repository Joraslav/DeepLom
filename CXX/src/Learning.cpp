#include "Glob.hpp"
#include "Learning.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <iterator>

Learning::Learning(vector_type const& Set, Model const &M, Mesh &AM) : Mod(M), ActMesh(AM)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Learning\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Eps = Set[0];
    this->Alf = Set[1];
    this->Gam = Set[2];
}

Learning::Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma, Model const &M, Mesh &AM) : Mod(M), ActMesh(AM)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Learning\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Method = Met;
    this->Eps = Epsilon;
    this->Alf = Alfa;
    this->Gam = Gamma;
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
    // this->MeshHistoryPath = DataPath + '/' + MeshHistoryP;
}

void Learning::SetPath(sup_st_type &SupPh)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + SupPh[0];
    this->TrackPath = DataPath + '/' + SupPh[1];
}

void Learning::SetStart(matrix_type const &Start)
{
    this->Track = Start;
    cout << this->Track << endl;
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
        std::uniform_int_distribution<> dist(0, pow(2,this->Mod.GetNumActions()));
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
    z_type Epoch{1};
    std::cout << "Begin\n" << std::endl;
    while (Epoch != Episode)
    {
        this->MeshState = ActMesh.GetMesh();
        if (Epoch%50==0)
        {
            std::cout << "Epoch =\t" << Epoch << std::endl;
        }
        for (r_type h = this->t0; h < Time; h=h+dt)
        {
            
        }

        // this->Q = ActMesh.Adaptive(this->Q,0.3);

        Epoch++;
    }
        
}

void Learning::GetQ()
{
    std::cout << this->Q;
}

void Learning::GetState(r_type &Nu)
{
    auto const nCols{this->MeshState.size()};

    for (auto i{0u}; i < nCols; i++)
    {
        if (Nu>=MeshState[i] && Nu<MeshState[i+1])
        {
            this->State = ++i;
            this->ActMesh.SetCount(--State);
            break;
        }
    }
}

auto Learning::GetReward(r_type const& x) -> r_type
{
    return -exp2l(abs(x)/4.)+11;
}

Learning::~Learning()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distructor of Learning\n" << this << std::endl;
    #endif   //DEBUG_CONSTRUCT_DISTRUCT

    os_type QOut(this->QPath), TrackOut(this->TrackPath);
    QOut << this->Q;
    TrackOut << this->Track;
}