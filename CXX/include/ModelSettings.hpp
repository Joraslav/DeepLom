#pragma once

#include "Glob.hpp"

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;

class ModelSettings
{
private:
    st_type DataPath;
    z_type Num_Action;
    st_type Name_Model;
public:
    ModelSettings();
    ModelSettings(z_type const& NumAction, st_type const& Name);
    ModelSettings(ModelSettings const &other);
    void SetPath(st_type &DP);
    void SetNumMuscles(z_type const& NumMuscles);
    void SetNameMuscles(st_type const& NameMuscles);
    auto GetNumMuscles() const -> z_type;

    friend os_type& operator<<(os_type& os, const ModelSettings& M);

    ~ModelSettings();
};