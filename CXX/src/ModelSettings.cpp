#include "ModelSettings.hpp"

ModelSettings::ModelSettings()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Muscles = 0;
    this->Name_Model = "Default";
}

ModelSettings::ModelSettings(z_type const& NumMuscles, st_type const& NameMuscles)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    SetNumMuscles(NumMuscles);
    SetNameMuscles(NameMuscles);
}

ModelSettings::ModelSettings(ModelSettings const &other)
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Coping to " << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT

    this->Num_Muscles = other.Num_Muscles;
    this->Name_Model = other.Name_Model;
    this->DataPath = other.DataPath;
}

void ModelSettings::SetPath(st_type &DP)
{
    st_type DataPath = "../Data";
    mkdir(DataPath.c_str());
    this->DataPath = DataPath + '/' + DP;
}

void ModelSettings::SetNumMuscles(z_type const& NumMuscles)
{
    this->Num_Muscles = NumMuscles;
}

void ModelSettings::SetNameMuscles(st_type const& NameMuscles)
{
    this->Name_Model = NameMuscles;
}

auto ModelSettings::GetNumMuscles() const -> z_type
{
    return this->Num_Muscles;
}

os_type& operator<<(os_type& os, const ModelSettings& M)
{
    os << "Name of Muscles Model\t" << M.Name_Model << std::endl;
    os << "Number of Muscles\t" << M.Num_Muscles << std::endl;
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