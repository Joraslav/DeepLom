#include "Model.hpp"

Model::Model()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = 0;
    this->Name_Model = "Default";
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
}

void Model::SetPath(st_type &DP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + DP;
}

void Model::SetNumActions(z_type const& NumAction)
{
    this->Num_Action = NumAction;
}

void Model::SetName(st_type const& Name)
{
    this->Name_Model = Name;
}

auto Model::GetNumActions() const -> z_type
{
    return this->Num_Action;
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

    os_type Out{this->DataPath};
    Out << *this;
}