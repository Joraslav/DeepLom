#pragma once

#include "Glob.hpp"
#include <iostream>

using namespace tls;

// template<class T>
class Learning
{
private:
    z_type State, Action;
    r_type Eps,Alf,Gam,t0,dt,Time;
    vector_type Mesh;
    matrix_type Q, Track, MeshHistory;
    st_type Method, QPath, TrackPath;
public:
    Learning(st_type &Met,r_type &Epsilon, r_type &Alfa, r_type &Gamma);
    void SetMesh(vector_type &MeshState);
    void SetTime(vector_type &TimeArr);
    void SetPath(st_type &QP, st_type &TrackP);
    void GenerateQ(z_type const &s, z_type const &a);
    void GetQ();
    void SetMeshHistory(vector_type &M);
    ~Learning();
};