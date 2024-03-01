#pragma once

#include "Glob.hpp"

#include <cmath>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>

using namespace tls;

class ModelSettings
{
private:
    z_type Num_Muscles;
    st_type Name_Model;
public:
    ModelSettings();
    ModelSettings(z_type const& NumMuscles, st_type const& NameMuscles);
    ModelSettings(ModelSettings const &other);
    void SetNumMuscles(z_type const& NumMuscles);
    void SetNameMuscles(st_type const& NameMuscles);
    auto GetNumMuscles() const -> z_type;
    ~ModelSettings();
};