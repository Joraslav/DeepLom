#pragma once

#include "Glob.hpp"

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;

class Model
{
private:
    st_type DataPath;
    z_type Num_Action;
    st_type Name_Model;
public:
    Model();
    Model(z_type const& NumAction, st_type const& Name);
    Model(Model const &other);
    void SetPath(st_type &DP);
    void SetNumActions(z_type const& NumMuscles);
    void SetName(st_type const& NameMuscles);
    auto GetNumActions() const -> z_type;

    friend os_type& operator<<(os_type& os, const Model& M);

    ~Model();
};