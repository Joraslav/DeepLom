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

ModelSettings::~ModelSettings()
{
    #ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Model\n" << this << std::endl;
    #endif //DEBUG_CONSTRUCT_DISTRUCT
}