#pragma once

#include "Glob.hpp"

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;
using sup_st_type = vector_tmpl<st_type>;

class Model
{
private:
    st_type DataPath;
    st_type Name_Model, XPath;
    z_type Num_Action;
    vector_type Start;
    matrix_type X;
public:
    Model();
    Model(z_type const& NumAction, st_type const& Name);
    Model(Model const &other);
    void SetStart(vector_type const& XStart);
    auto U(z_type const& Action) -> r_type;
    void F(vector_type const& x, z_type const Action, r_type const h);
    void SetPath(st_type &DP);
    void SetPath(sup_st_type &SP);
    void SetNumActions(z_type const& NumMuscles);
    void SetName(st_type const& NameMuscles);
    void Step(r_type const& dt);
    auto GetNumActions() const -> z_type;
    auto GetX0() -> vector_type;
    auto GetF0() -> vector_type;
    friend os_type& operator<<(os_type& os, const Model& M);
    ~Model();
};