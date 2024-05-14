#pragma once

#include "Glob.hpp"

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using sup_st_type = vector_tmpl<st_type>;

class Mesh
{
private:
    st_type ActPath, StCountPath, MHistPath, SCHistPath;
    st_type Method;
    vector_type ActMesh, StateCount;
    matrix_type MeshHistory, StateCountHistory;
    Int Goal, Active_State, Num_States;
public:
    Mesh(st_type const& Mthd);
    Mesh(vector_type const& M, st_type const& Mthd, Int const& G);
    Mesh(sup_st_type &SupPH, st_type const& Mthd, vector_type const& M, Int const& G);
    Mesh(Mesh const &other);
    void SetPath(st_type &AP, st_type &SCP, st_type &MHP);
    void SetPath(sup_st_type &SupPh);
    void SetCount(Int const &St);
    auto GetState(Real const& Nu) -> Int;
    auto GetMesh() -> vector_type;
    auto GetGoal() const -> Int;
    auto GetNumState() -> Int;

    #ifdef ADAPTIVE
    auto Adaptive(matrix_type &QL, Real const Param) -> matrix_type;
    #endif //ADAPTIVE

    ~Mesh();
};
