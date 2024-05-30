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

void Learning::SetPath(sup_st_type &SupQ)
{
    st_type DataPath = "../" + Method + '_' + "Data";
    mkdir(DataPath.c_str());
    this->QPath = DataPath + '/' + SupQ[0];
    this->QLogPath = DataPath + '/' + SupQ[1];
}

void Learning::SetMethod(st_type &Mthd)
{
    this->Method = Mthd;
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

auto Learning::GreedyPolicy(Real Eps, Int &ActState) -> Int
{
    Int Rez;
    auto Q_iter{this->Q.begin() + ActState};
    auto Q_vec = *Q_iter;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    if (dist(gen) < Eps)
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
    Int Epoch{1};
    cout << '\n'
         << "Begin of Method \t" << this->Method << "\n"
         << std::endl;
    while (Epoch <= Episode)
    {
        if (Epoch % 50 == 0)
        {
            this->Eps = this->Eps * 0.5;
            // this->Gam = this->Gam + 0.05;
            cout << this->Method << "Epoch = " << Epoch << endl;
        }
        vector_type X0 = this->Model_.GetStart();
        vector_type F0 = this->Model_.GetF0();
        for (Real h = this->t0; h <= Time; h = h + dt)
        {
            Real Nu = Metric(X0, F0);
            this->Actual_State = Mesh_.GetState(Nu);
            this->Actual_Action = GreedyPolicy(this->Eps, this->Actual_State);
            Model_.SetActiveAction(this->Actual_Action);
            vector_type X = Model_.RungeKutta(X0, h, dt);
            vector_type F = Model_.F(X, h);
            Nu = Metric(X, F);
            Real Rew = GetReward(Nu);
            this->Next_State = Mesh_.GetState(Nu);
            this->Next_Action = GreedyPolicy(this->Eps, this->Next_State);
            if (this->Method == "SARSA")
            {
                this->Q[Actual_State][Actual_Action] = Q[Actual_State][Actual_Action] +
                                                       Alf * (Rew + Gam * Q[Next_State][Next_Action] - Q[Actual_State][Actual_Action]);
            }
            else if (this->Method == "Q-Learning")
            {
                auto Q_iter{this->Q.begin() + Next_State};
                auto Q_vec = *Q_iter;
                auto Max = *max_element(Q_vec.begin(), Q_vec.end());
                this->Q[Actual_State][Actual_Action] = Q[Actual_State][Actual_Action] +
                                                       Alf * (Rew + Gam * Max - Q[Actual_State][Actual_Action]);
            }
            X0 = X;
            F0 = F;
        }
#ifdef ADAPTIVE
        if (Epoch != Episode)
        {
            this->Q = Mesh_.Adaptive(this->Q, 0.3);
        }
#endif // ADAPTIVE
        Epoch++;
    }
    MakeQLog();
}

void Learning::MakeQLog()
{
    this->QLog = matrix_type(this->Q.size(), vector_type(this->Q.front().size(), 0));
    auto const nRows{this->Q.size()};
    for (auto i{0u}; i < nRows; ++i)
    {
        auto Q_iter{this->Q.begin() + i};
        auto Q_vec = *Q_iter;
        auto Max_elem = *max_element(Q_vec.begin(), Q_vec.end());
        Int Iter = FindIndex(Q_vec, Max_elem);
        this->QLog[i][Iter] = 1;
    }
}

void Learning::Test()
{
    vector_type X0 = Model_.GetStart();
    vector_type F0 = Model_.GetF0();
    for (Real h = this->t0; h < this->Time; h += this->dt)
    {
        Real Nu = Metric(X0, F0);
        this->Actual_State = Mesh_.GetState(Nu);
        this->Actual_Action = GreedyPolicy(0., this->Actual_State);
        Model_.SetActiveAction(this->Actual_Action);
        vector_type X = Model_.RungeKutta(X0, h, dt);
        // vector_type X = Model_.Euler(X0,h,dt);
        Model_.WriteX(X);
        vector_type F = Model_.F(X, h);
        X0 = X;
        F0 = F;
    }
}

auto Learning::GetReward(Real const &x) -> Real
{
    return -exp2l(abs(x) / 0.54) + 2.;
}

void Learning::Reload(vector_type const &Set)
{
    cout << "Reload of Learning\t" << this << endl;
    this->Q.clear();
    this->QLog.clear();

    this->Eps = Set[0];
    this->Alf = Set[1];
    this->Gam = Set[2];
    GenerateQ(Mesh_.GetNumState(), Model_.GetNumActions());
}

auto Learning::GetMatrix(st_type const ID) -> matrix_type
{
    if (ID == "Q")   {return this->Q;}
    else if (ID == "QLog")    {return this->QLog;}
    else    {return matrix_type(3, vector_type(3, 0.));}
}

Learning::~Learning()
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Learning\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    // os_type QOut(this->QPath), QLogOut(this->QLogPath);
    // QOut << this->Q;
    // QLogOut << this->QLog;
}