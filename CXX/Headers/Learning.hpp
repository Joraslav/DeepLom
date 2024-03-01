#pragma once

#include "Glob.hpp"
#include "ModelSettings.hpp"
#include <iostream>

using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;

class Learning
{
private:
    ModelSettings const &Muscl;
    z_type State, Action;
    r_type Eps,Alf,Gam,t0,dt,Time;
    vector_type Mesh;
    matrix_type Q, Track, MeshHistory;
    st_type Method, QPath, TrackPath, MeshHistoryPath;
public:
    Learning(vector_type const& Set, ModelSettings const &M);
    Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma, ModelSettings const &M);
    void SetMesh(vector_type &MeshState);
    void SetTime(vector_type &TimeArr);
    void SetPath(st_type &QP, st_type &TrackP, st_type &MeshHistoryP);
    void GenerateQ(z_type const &s, z_type const &a);

    void RandomQ();

    auto GreedyPolicy(z_type &ActState) -> z_type;
    void Run(z_type Episode);
    void GetQ();
    void GetState(r_type &Nu);
    void SetMeshHistory(vector_type &M);
    ~Learning();
};