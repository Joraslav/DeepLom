#pragma once

#include "Glob.hpp"
#include "ModelSettings.hpp"
#include "Mesh.hpp"

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;
using sup_st_type = vector_tmpl<st_type>;

class Learning
{
private:
    ModelSettings const &Muscl;
    Mesh const &ActMesh;
    z_type State, Action;
    r_type Eps,Alf,Gam,t0,dt,Time;
    vector_type MeshState;
    matrix_type Q, Track;
    sup_st_type Path;
    st_type Method, QPath, TrackPath;
public:
    Learning(vector_type const& Set, ModelSettings const &M, Mesh const &AM);
    Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma, ModelSettings const &M, Mesh const &AM);
    // void SetMesh(vector_type &MeshState);
    void SetTime(vector_type &TimeArr);
    void SetPath(st_type &QP, st_type &TrackP, st_type &MeshHistoryP);
    void SetPath(sup_st_type &SupPh);
    void GenerateQ(z_type const &s, z_type const &a);

    void RandomQ();

    auto GreedyPolicy(z_type &ActState) -> z_type;
    void Run(z_type Episode);
    void GetQ();
    void GetState(r_type &Nu);
    // void SetMeshHistory(vector_type &M);
    ~Learning();
};