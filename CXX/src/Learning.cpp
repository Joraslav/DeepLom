#include "Glob.hpp"
#include "Learning.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <iterator>

Learning::Learning(vector_type const &Set, Model &M, Mesh &AM) : Model_(M), Mesh_(AM)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Learning\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->Eps = Set[0];
    this->Alf = Set[1];
    this->Gam = Set[2];
    GenerateQ(Mesh_.GetNumState(), Model_.GetNumActions());
}

Learning::Learning(Real &Epsilon, Real &Alfa, Real &Gamma, Model &M, Mesh &AM) : Model_(M), Mesh_(AM)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Learning\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->Eps = Epsilon;
    this->Alf = Alfa;
    this->Gam = Gamma;
    GenerateQ(Mesh_.GetNumState(), Model_.GetNumActions());
}

void Learning::SetTime(vector_type &TimeArr)
{
    this->t0 = TimeArr[0];
    this->dt = TimeArr[1];
    this->Time = TimeArr[2];
}

void Learning::SetPath(st_type &QP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + QP;
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
    auto Q_iter{this->Q.begin() + ActState};
    auto Q_vec = *Q_iter;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    if (dist(gen) < this->Eps)
    {
        std::uniform_int_distribution<> dist(0, this->Model_.GetNumActions() - 1);
        Rez = dist(gen);
    }
    else
    {
        auto Elem = *std::max_element(Q_vec.begin(), Q_vec.end());
        Rez = tls::FindIndex(Q_vec, Elem);
    }
    return Rez;
}

void Learning::Run(Int const Episode)
{
    Int Epoch{0};
    std::cout << "Begin\n"
              << std::endl;
    while (Epoch != Episode)
    {
        if (Epoch % 50 == 0)
        {
            std::cout << "Epoch =\t" << Epoch + 1 << std::endl;
        }
        vector_type X0 = this->Model_.GetStart();
        vector_type F0 = this->Model_.GetF0();
        for (Real h = this->t0; h < Time + dt; h = h + dt)
        {
            Real Nu = Metric(X0, F0);
            this->Actual_State = Mesh_.GetState(Nu);
            this->Actual_Action = GreedyPolicy(this->Actual_State);
            Model_.SetActiveAction(this->Actual_Action);
            vector_type X = Model_.RungeKutta(h, dt);
            vector_type F = Model_.F(X, h);
            Nu = Metric(X, F);
            Real Rew = GetReward(Nu);
            this->Next_State = Mesh_.GetState(Nu);
            this->Next_Action = GreedyPolicy(this->Next_State);
            this->Q[Actual_State][Actual_Action] = Q[Actual_State][Actual_Action] + Alf * (Rew + Gam * Q[Next_State][Next_Action] - Q[Actual_State][Actual_Action]);
            X0 = X;
            F0 = F;
        }
#ifdef ADAPTIVE
        this->Q = Mesh_.Adaptive(this->Q, 0.3);
#endif // ADAPTIVE
        Epoch++;
    }
}

auto Learning::GetReward(Real const &x) -> Real
{
    return -exp2l(abs(x) / M_E) + 2.;
}

Learning::~Learning()
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Learning\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    os_type QOut(this->QPath);
    QOut << this->Q;
}