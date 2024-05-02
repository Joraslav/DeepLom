#include "Glob.hpp"
#include "Learning.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <iterator>

Learning::Learning(vector_type const& Set, Model &M, Mesh &AM) : Model_(M), Mesh_(AM)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Learning\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Eps = Set[0];
    this->Alf = Set[1];
    this->Gam = Set[2];
}

Learning::Learning(st_type &Met, Real &Epsilon, Real &Alfa, Real &Gamma, Model &M, Mesh &AM) : Model_(M), Mesh_(AM)
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

void Learning::SetPath(st_type &QP, st_type &TrackP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + QP;
    this->TrackPath = DataPath + '/' + TrackP;
}

void Learning::SetPath(st_type &QP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + QP;
}

void Learning::SetPath(sup_st_type &SupPh)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + SupPh[0];
    this->TrackPath = DataPath + '/' + SupPh[1];
}

void Learning::GenerateQ(Int const &s, Int const &a)
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

auto Learning::GreedyPolicy(Int &ActState) -> Int
{
    Int Rez;
    auto Q_iter{this->Q.begin()};
    Q_iter += ActState;
    auto Q_vec = *Q_iter;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    if (dist(gen) < this->Eps)
    {
        std::uniform_int_distribution<> dist(0, pow(2,this->Model_.GetNumActions()));
        Rez = dist(gen);
    }
    else
    {
        auto Elem = *std::max_element(begin(Q_vec),end(Q_vec));
        Rez = tls::FindIndex(Q_vec,Elem);
    }
    return Rez;
}

void Learning::Run(Int const Episode)
{
    Int Epoch{1};
    std::cout << "Begin\n" << std::endl;
    while (Epoch != Episode)
    {
        this->MeshState = Mesh_.GetMesh();
        if (Epoch%50==0)
        {
            std::cout << "Epoch =\t" << Epoch << std::endl;
        }
        vector_type X0 = this->Model_.GetStart();
        vector_type F0 = this->Model_.GetF0();
        // Real Nu;
        for (Real h = this->t0; h <= Time; h=h+dt)
        {
            // Nu = Metric(X0,F0);

        }

        // this->Q = ActMesh.Adaptive(this->Q,0.3);

        Epoch++;
    }
        
}

void Learning::GetState(Real &Nu)
{
    auto const nCols{this->MeshState.size()};

    for (auto i{0u}; i < nCols; i++)
    {
        if (Nu>=MeshState[i] && Nu<MeshState[i+1])
        {
            this->State = i;
            this->Mesh_.SetCount(--State);
            break;
        }
    }
}

auto Learning::GetReward(Real const& x) -> Real
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
    // TrackOut << this->Track;
}