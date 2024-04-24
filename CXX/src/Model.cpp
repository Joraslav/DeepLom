#include "Model.hpp"

Model::Model()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = 5;
    this->Name_Model = "Kapitza";
}

Model::Model(z_type const& NumAction, st_type const& Name)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    SetNumActions(NumAction);
    SetName(Name);
}

Model::Model(Model const &other)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Coping to " << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = other.Num_Action;
    this->Name_Model = other.Name_Model;
    this->DataPath = other.DataPath;
    this->X = other.X;
    this->XPath = other.XPath;
    this->Start = other.Start;
}

void Model::SetStart(vector_type const& XStart)
{
    this->Start = XStart;
}

auto Model::U(z_type const& Action) -> r_type
{
    r_type Rez{0.6371};
    switch (Action)
    {
    case 0:
        Rez = 1.;
        break;
    case 2:
        Rez *= 2.;
        break;
    case 3:
        Rez *= 3.;
        break;
    case 4:
        Rez *= 4.;
        break;
    case 5:
        Rez *= 5.;
        break;
    default:
        break;
    }
    return Rez;
}

void Model::F(vector_type const& x, z_type const Action, r_type const h)
{
    r_type g{9.81}, l{0.1}, mu{25};
    r_type k = g/(l*powl(mu,2.));
    r_type a{3.1855};
    this->X[0][0] = k*x[1];
    this->X[0][1] = -sinl(x[0])*((U(Action)+a)*cosl(h)+1);  
}

void Model::SetPath(st_type &DP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + DP;
}

void Model::SetPath(sup_st_type &SP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + SP[0];
    this->XPath = DataPath + '/' + SP[1];
}

void Model::SetNumActions(z_type const& NumAction)
{
    this->Num_Action = NumAction;
}

void Model::SetName(st_type const& Name)
{
    this->Name_Model = Name;
}

void Model::Step(r_type const& dt)
{
    
}

auto Model::GetNumActions() const -> z_type
{
    return this->Num_Action;
}

auto Model::GetX0() -> vector_type
{
    return this->Start;
}

auto Model::GetF0() -> vector_type
{
    auto XBegin = this->X.begin();
    return *XBegin;
}

os_type& operator<<(os_type& os, const Model& M)
{
    os << "Name of Muscles Model\t" << M.Name_Model << std::endl;
    os << "Number of Muscles\t" << M.Num_Action << std::endl;
    return os;
}

Model::~Model()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    os_type ModelOut{this->DataPath}, XOut{this->XPath};
    ModelOut << *this;
    XOut << this->X;
}