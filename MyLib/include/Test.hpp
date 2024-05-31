#pragma once

#include "Glob.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Learning.hpp"

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using sup_st_type = vector_tmpl<st_type>;

class Test
{
private:
    Learning &Learn_;
    Model &Model_;
    Mesh &Mesh_;
    matrix_type QLog_, Track_;
    vector_type ActMesh_, Metric_, XStart_;

public:
    Test(Learning &L, Model &Mod, Mesh &Mes);
    void ReadMesh(st_type const Mesh_Path);
    void ReadQLog(st_type const QLog_Path);
    auto GetTrack() -> matrix_type;
    ~Test();
};
