#pragma once

#include "Glob.hpp"

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using sup_st_type = vector_tmpl<st_type>;

class Model
{
private:
    st_type DataPath, XPath;
    st_type Name_Model, Method;
    Int Num_Action, Active_Action;
    vector_type Start;
    matrix_type X;

public:
    Model(Int const &NumAction, st_type const &Mthd);
    Model(sup_st_type &SP, st_type const &Mthd, vector_type const &XStart, Int const NumAction);
    Model(Model const &other);
    void SetStart(vector_type const &XStart);
    auto U(Int const &Action) -> Real;
    auto F(vector_type x, Real h) -> vector_type;
    void WriteX(vector_type const &X);
    void SetPath(st_type &DP);
    void SetPath(sup_st_type &SP);
    void SetNumActions(Int const &NumAction);
    void SetName(st_type const &NameMuscles);
    void SetActiveAction(Int const Act);
    auto RungeKutta(vector_type &X_0, Real const &h, Real &dt) -> vector_type;
    auto GetNumActions() const -> Int;
    auto GetStart() -> vector_type;
    auto GetF0() -> vector_type;
    friend os_type &operator<<(os_type &os, const Model &M);
    ~Model();
};