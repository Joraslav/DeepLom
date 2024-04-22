#pragma once

#include "Glob.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;
using sup_st_type = vector_tmpl<st_type>;

class Learning
{
private:
    Model const &Mod;
    Mesh &ActMesh;
    z_type State, Action;
    r_type Eps,Alf,Gam,t0,dt,Time;
    vector_type MeshState;
    matrix_type Q, Track;
    sup_st_type Path;
    st_type Method, QPath, TrackPath;
public:
    Learning(vector_type const& Set, Model const &M, Mesh &AM);
    Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma, Model const &M, Mesh &AM);
    void SetTime(vector_type &TimeArr);
    void SetPath(st_type &QP, st_type &TrackP, st_type &MeshHistoryP);
    void SetPath(sup_st_type &SupPh);
    void SetStart(matrix_type const &Start);
    void GenerateQ(z_type const &s, z_type const &a);
    void RandomQ();
    auto GreedyPolicy(z_type &ActState) -> z_type;
    void Run(z_type Episode);
    void GetQ();
    void GetState(r_type &Nu);
    auto GetReward(r_type const& x) -> r_type;
    ~Learning();
};