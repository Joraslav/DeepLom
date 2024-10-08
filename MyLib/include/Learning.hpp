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
    Int Actual_State, Actual_Action, Next_State, Next_Action;
    Real Eps, Alf, Gam, t0, dt, Time;
    matrix_type Q, QLog;
    sup_st_type Path;
    st_type QPath, QLogPath, Method, TimePath;

public:
    Learning(vector_type const &Set, Model &M, Mesh &AM);
    Learning(Real &Epsilon, Real &Alfa, Real &Gamma, Model &M, Mesh &AM);
    void SetTime(vector_type &TimeArr);
    void SetPath(sup_st_type &SupQ);
    void SetMethod(st_type &Mthd);
    void GenerateQ(Int const &s, Int const &a);
    void RandomQ();
    auto GreedyPolicy(Real Eps, Int &ActState) -> Int;
    void Run(Int const Episode);
    void MakeQLog();
    void Test();
    auto GetReward(Real const &x) -> Real;
    void Reload(vector_type const &Set);
    auto GetMatrix(st_type const ID) -> matrix_type;
    ~Learning();
};