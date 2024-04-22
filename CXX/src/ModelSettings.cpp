#include "ModelSettings.hpp"

ModelSettings::ModelSettings()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = 0;
    this->Name_Model = "Default";
}

ModelSettings::ModelSettings(z_type const& NumAction, st_type const& Name)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    SetNumMuscles(NumAction);
    SetNameMuscles(Name);
}

ModelSettings::ModelSettings(ModelSettings const &other)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Coping to " << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Action = other.Num_Action;
    this->Name_Model = other.Name_Model;
    this->DataPath = other.DataPath;
}

void ModelSettings::SetPath(st_type &DP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + DP;
}

void ModelSettings::SetNumMuscles(z_type const& NumAction)
{
    this->Num_Action = NumAction;
}

void ModelSettings::SetNameMuscles(st_type const& Name)
{
    this->Name_Model = Name;
}

auto ModelSettings::GetNumMuscles() const -> z_type
{
    return this->Num_Action;
}

os_type& operator<<(os_type& os, const ModelSettings& M)
{
    os << "Name of Muscles Model\t" << M.Name_Model << std::endl;
    os << "Number of Muscles\t" << M.Num_Action << std::endl;
    return os;
}

ModelSettings::~ModelSettings()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    os_type MusclOut{this->DataPath};
    MusclOut << *this;
}