#pragma once

#include "Glob.hpp"

#define ADAPTIVE

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using sup_st_type = vector_tmpl<st_type>;

class Mesh
{
private:
    st_type ActPath, StCountPath, MHistPath;
    vector_type ActMesh, StateCount;
    matrix_type MeshHistory;
    Int Goal;
public:
    Mesh(/* args */);
    Mesh(vector_type const& M, Int const& G);
    Mesh(sup_st_type &SupPH, vector_type const& M, Int const& G);
    Mesh(Mesh const &other);
    void SetPath(st_type &AP, st_type &SCP, st_type &MHP);
    void SetPath(sup_st_type &SupPh);
    void SetCount(Int const &St);

    auto GetMesh() const -> vector_type;
    auto GetGoal() const -> Int;

    #ifdef ADAPTIVE
    auto Adaptive(matrix_type &QL, Real const Param) -> matrix_type;
    #endif //ADAPTIVE

    ~Mesh();
};
