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
    Model &Model_;
    Mesh &Mesh_;
    Int Actual_State, Actual_Action;
    Real t0, dt, T;
    matrix_type QLog_, Track_;
    vector_type ActMesh_, Metric_, XStart_;

public:
    Test(Model &Mod, Mesh &Mes);
    void ReadMesh(st_type const &Mesh_Path);
    void SetMesh(vector_type const &M);
    void ReadQLog(st_type const &QLog_Path);
    void SetQLog(matrix_type const &QL);
    void SetTime(vector_type const &Time);
    void SetStart(vector_type const &Start);
    auto GetTrack() -> matrix_type;
    auto GetMetric() -> vector_type;
    auto GetState(Real const &Nu) -> Int;
    auto Policy(Int &ActState) -> Int;
    void RunTest();
    void Reload();
    ~Test();
};
