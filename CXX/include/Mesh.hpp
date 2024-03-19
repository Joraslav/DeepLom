#pragma once

#include "Glob.hpp"

#define ADAPTIVE

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;
using sup_st_type = vector_tmpl<st_type>;

class Mesh
{
private:
    st_type ActPath, StCountPath, MHistPath;
    vector_type ActMesh, StateCount;
    matrix_type MeshHistory;
    z_type Goal;
public:
    Mesh(/* args */);
    Mesh(vector_type const& M, z_type const& G);
    Mesh(Mesh const &other);
    void SetPath(st_type &AP, st_type &SCP, st_type &MHP);
    void SetPath(sup_st_type &SupPh);
    void SetCount(z_type const &St);

    auto GetMesh() const -> vector_type;
    auto GetGoal() const -> z_type;

    #ifdef ADAPTIVE
    auto Adaptive(matrix_type &QL, z_type const Param) -> matrix_type;
    #endif //ADAPTIVE

    ~Mesh();
};
