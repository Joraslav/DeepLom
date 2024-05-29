#include "Model.hpp"

Model::Model(Int const &NumAction, st_type const &Mthd)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = NumAction;
    this->Method = Mthd;
    this->Name_Model = "Kapitza";
}

Model::Model(sup_st_type &SP, st_type const &Mthd, vector_type const &XStart, Int const NumAction)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->Method = Mthd;
    SetPath(SP);
    SetStart(XStart);
    this->Num_Action = NumAction;
    this->Active_Action = 1;
    this->Name_Model = "Kapitza";
}

Model::Model(Model const &other)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Coping to\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = other.Num_Action;
    this->Name_Model = other.Name_Model;
    this->Start = other.Start;
    this->Active_Action = other.Active_Action;
}

void Model::SetStart(vector_type const &XStart)
{
    this->Start = XStart;
    this->X.push_back(XStart);
}

auto Model::U(Int const &Action) -> Real
{
    Real Rez{0.3};
    return Action*Rez;
    // return Action - 1;
}

auto Model::F(vector_type x, Real h) -> vector_type
{
    vector_type Rez(x.size());
    Real g{9.81}, l{0.1}, mu{25};
    Real k = g/(l*powl(mu,2.));
    // Real k{0.08};
    Real a{1.5};
    Rez[0] = k*x[1];
    Rez[1] = -sinl(x[0])*((U(Active_Action)+a)*cosl(h)+1);
    // Rez[0] = x[1];
    // Rez[1] = -sinl(x[0]) + U(Active_Action) * sgn(x[0]);
    return Rez;
}

void Model::WriteX(vector_type const &X)
{
    this->X.push_back(X);
}

void Model::SetPath(st_type &DP)
{
    st_type DataPath = "../" + Method + '_' + "Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + DP;
}

void Model::SetPath(sup_st_type &SP)
{
    st_type DataPath = "../" + Method + '_' + "Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + SP[0];
    this->XPath = DataPath + '/' + SP[1];
}

void Model::SetNumActions(Int const &NumAction)
{
    this->Num_Action = NumAction;
}

void Model::SetName(st_type const &Name)
{
    this->Name_Model = Name;
}

void Model::SetActiveAction(Int const Act)
{
    this->Active_Action = Act;
}

auto Model::RungeKutta(vector_type &X_0, Real const &h, Real &dt) -> vector_type
{
    vector_type k_1 = F(X_0, h);
    auto const &_k_2 = (dt / 2.) * k_1 + X_0;
    vector_type k_2 = F(_k_2, h + dt / 2.);
    auto const &_k_3 = (dt / 2.) * k_2 + X_0;
    vector_type k_3 = F(_k_3, h + dt / 2.);
    auto const &_k_4 = dt * k_3 + X_0;
    vector_type k_4 = F(_k_4, h + dt);
    Real p = 2.;
    vector_type Rez = X_0 + (dt / 6.) * (k_1 + p * k_2 + p * k_3 + k_4);
    return Rez;
}

auto Model::Euler(vector_type &X_0, Real const &h, Real &dt) -> vector_type
{
    vector_type Rez = X_0 + dt*F(X_0,h);
    return Rez;
}
auto Model::GetNumActions() const -> Int
{
    return this->Num_Action;
}

auto Model::GetStart() -> vector_type
{
    return this->Start;
}

auto Model::GetF0() -> vector_type
{
    auto F0 = F(Start,0.0);
    return F0;
}

os_type &operator<<(os_type &os, const Model &M)
{
    os << "Name of Model\t" << M.Name_Model << std::endl;
    os << "Number of Actions\t" << M.Num_Action << std::endl;
    return os;
}

Model::~Model()
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Model\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT

    os_type ModelOut{this->DataPath}, XOut{this->XPath};
    ModelOut << *this;
    XOut << this->X;
}