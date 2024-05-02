#pragma once

#include "Glob.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using sup_st_type = vector_tmpl<st_type>;

class Learning
{
private:
    Model &Model_;
    Mesh &Mesh_;
    Int State, Action;
    Real Eps,Alf,Gam,t0,dt,Time;
    vector_type MeshState;
    // matrix_type Q, Track;
    matrix_type Q;
    sup_st_type Path;
    st_type Method, QPath, TrackPath;
public:
    Learning(vector_type const& Set, Model &M, Mesh &AM);
    Learning(st_type &Met, Real &Epsilon, Real &Alfa, Real &Gamma, Model &M, Mesh &AM);
    void SetTime(vector_type &TimeArr);
    void SetPath(st_type &QP, st_type &TrackP);
    void SetPath(st_type &QP);
    void SetPath(sup_st_type &SupPh);
    // void SetStart(matrix_type const &Start);
    void GenerateQ(Int const &s, Int const &a);
    void RandomQ();
    auto GreedyPolicy(Int &ActState) -> Int;
    void Run(Int const Episode);
    void GetState(Real &Nu);
    auto GetReward(Real const& x) -> Real;
    ~Learning();
};