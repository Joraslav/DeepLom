#include "Test.hpp"

Test::Test(Model &Mod, Mesh &Mes) : Model_(Mod), Mesh_(Mes)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Test\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT
}

void Test::ReadMesh(st_type const &Mesh_Path)
{
    ifstream mesh_in(Mesh_Path);
    mesh_in.open(Mesh_Path);
    mesh_in >> this->ActMesh_;
    mesh_in.close();
}

void Test::SetMesh(vector_type const &M)
{
    this->ActMesh_ = M;
}

void Test::ReadQLog(st_type const &QLog_Path)
{
    if_type qlog_in(QLog_Path);
    qlog_in >> this->QLog_;
    qlog_in.close();
}

void Test::SetQLog(matrix_type const &QL)
{
    this->QLog_ = QL;
}

void Test::SetTime(vector_type const &Time)
{
    this->t0 = Time[0];
    this->dt = Time[1];
    this->T = Time[2];
}

void Test::SetStart(vector_type const &Start)
{
    this->XStart_ = Start;
}

auto Test::GetTrack() -> matrix_type
{
    return this->Track_;
}

auto Test::GetMetric() -> vector_type
{
    return this->Metric_;
}

auto Test::GetState(Real const &Nu) -> Int
{
    auto const nCols{this->ActMesh_.size()};
    for (auto i{0u}; i < nCols; i++)
    {
        if (Nu >= ActMesh_[i] && Nu < ActMesh_[i + 1])
        {
            this->Actual_State = i;
            break;
        }
    }
    return this->Actual_State;
}

auto Test::Policy(Int &ActState) -> Int
{
    Int Rez;
    auto Q_iter{this->QLog_.begin() + ActState};
    auto Q_vec = *Q_iter;
    auto Elem = *max_element(Q_vec.begin(), Q_vec.end());
    Rez = FindIndex(Q_vec, Elem);
    return Rez;
}

void Test::RunTest()
{
    vector_type X0 = this->XStart_;
    vector_type F0 = Model_.F(X0, this->t0);
    Track_.push_back(X0);
    for (Real h = this->t0; h < this->T; h += this->dt)
    {
        Real Nu = Metric(X0, F0);
        Metric_.push_back(Nu);
        Int State = GetState(Nu);
        Int Action = Policy(State);
        Model_.SetActiveAction(Action);
        vector_type X = Model_.RungeKutta(X0,h,dt);
        Track_.push_back(X);
        vector_type F = Model_.F(X,h);
        X0 = X;
        F0 = F;
    }
}

void Test::Reload()
{
    cout << "Reload of Test\t" << this << endl;
    this->Metric_.clear();
    this->Track_.clear();
    this->XStart_.clear();
}

Test::~Test()
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Test\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT
}