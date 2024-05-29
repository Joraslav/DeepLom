#include <cmath>
#include <vector>    // std::vector
#include <iostream>  // std::{ostream, cout, endl}
#include <stdexcept> // std::invalid_argument
#include <ranges>    // std::views::iota
#include <iterator>  //iterator

#include "Glob.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Learning.hpp"

#ifdef DEBUG_MLPACK
#include <mlpack.hpp>
#endif // DEBUG_MLPACK

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using str_vec = vector_tmpl<st_type>;

int main()
{
#ifdef DEBUG_ADAPTIVE
     vector_type StateCount{1, 0, 1, 4, 170, 0, 210, 180, 2, 0, 215};
     vector_type ActMesh{-INFINITY, -5, -3, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 3, INFINITY};
     Int Goal = FindIndex(ActMesh, 0);
     cout << "Goal is\n"
          << Goal << endl;
     matrix_type QL(StateCount.size(), {0, 0, 0});
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_real_distribution<> dist(0.0, 1.0);
     auto const nRows{QL.size()};
     auto const nCols{QL.front().size()};
     for (auto i{0u}; i < nRows; ++i)
     {
          for (auto j{0u}; j < nCols; ++j)
          {
               QL[i][j] = dist(gen);
          }
     }
     vector_type Zeros;
     Zeros = FindIndex(StateCount, is_zero);
     cout << "Index zeros in StateCount\n"
          << Zeros << endl;
     cout << "ActMesh\n"
          << ActMesh << endl;
     Int Remove{0};
     for (auto &index : Zeros)
     {
          if ((index != Goal) && (index != Goal - 1) && (index != Goal + 1))
          {
               ActMesh[index + 1 - Remove] = (ActMesh[index - Remove] + ActMesh[index + 1 - Remove]) * 0.5;
               ActMesh.erase(ActMesh.begin() + index - Remove);
               QL.erase(QL.begin() + (index - Remove));
               cout << "ActMesh\n"
                    << ActMesh << endl;
               StateCount.erase(StateCount.begin() + (index - Remove));
               Remove++;
          }
     }
     Real p_p = 0.2;
     Real SumElem = accumulate(StateCount.begin(), StateCount.end(), 0);

     auto const Length{StateCount.size()};
     vector_type Good_Pos;
     for (auto i{0u}; i < Length; ++i)
     {
          if (StateCount[i] >= p_p * SumElem)
          {
               Good_Pos.push_back(i);
          }
     }
     cout << "Good_Pos\n"
          << Good_Pos << endl;
     auto index_min_inf = FindIndex(ActMesh, -INFINITY);
     auto index_max_inf = FindIndex(ActMesh, INFINITY);

     Int Move{0};
     Real half{0.5};
     for (auto &index_g_p : Good_Pos)
     {
          if (index_g_p == index_min_inf)
          {
               ActMesh.insert(ActMesh.begin() + 1, 3 * ActMesh[index_g_p + 1]);
               cout << "ActMesh\n"
                    << ActMesh << endl;
               QL.insert(QL.begin(), vector_type(QL.front().size(), 0));
               StateCount.insert(StateCount.begin(), 0);
               Move++;
          }
          else if (index_g_p == index_max_inf - 1)
          {
               ActMesh.insert(ActMesh.end() - 1, 3 * ActMesh[index_g_p + Move]);
               cout << "ActMesh\n"
                    << ActMesh << endl;
               QL.insert(QL.end(), vector_type(QL.back().size(), 0));
               StateCount.push_back(0);
          }
          else
          {
               ActMesh.insert(ActMesh.begin() + index_g_p + Move + 1, (ActMesh[index_g_p + Move] + ActMesh[index_g_p + Move + 1]) * half);
               cout << "ActMesh\n"
                    << ActMesh << endl;
               QL[index_g_p + Move] = half * QL[index_g_p + Move];
               QL.insert(QL.begin() + index_g_p + Move, QL[index_g_p + Move]);
               StateCount.insert(StateCount.begin() + index_g_p + Move, 0);
               Move++;
          }
     }

#endif // DEBUG_ADAPTIVE

#ifdef DEBUG_INFO
     matrix_type m1{{1, 2, 3}, {4, 5, 6}}, m2{m1};
     vector_type v1{0, -4, -3}, v2{0, 4, 3}, v3{0, 16, 9};
     matrix_type m3{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, m4{{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};

     cout << "Norm v1\n"
          << Norm(v1) << endl;
     cout << "Sclar product of vector:\n"
          << v1 * v2 << endl;
     cout << "Multiplication m3*m4:\n"
          << m4 * m3 << endl;
     cout << "Sign v1(1)\n"
          << sgn(v1[1]) << endl;
     cout << "Metric v1 and v3\n"
          << Metric(v1, v3) << endl;

     random_device rd;
     mt19937 gen(rd());
     uniform_real_distribution<> dist_r_type(0.0, 1.0);
     uniform_int_distribution<> dist_z_type(1, 5);
     Real r_Rand = dist_r_type(gen);
     cout << "Random r_type is\n"
          << r_Rand << endl;
     Int z_Rand = dist_z_type(gen);
     cout << "Random z_type is\n"
          << z_Rand << endl;

     Int param = 6;
     auto Vec_m1_iter{m1.begin() + 1};
     auto Vec_m1 = *Vec_m1_iter;
     auto Index_VecM1 = FindIndex(Vec_m1, param);
     cout << "After FindIndex\n"
          << Index_VecM1 << endl;

     v3.insert(v3.begin() + 3, 0);
     cout << v3 << endl;

#endif // DEBUG_INFO

#ifdef DEBUG_CLASSES
     st_type methodQLearning{"Q-Learning"};
     st_type methodSARSA{"SARSA"};
     st_type timelearn{"Time.txt"};

     str_vec model_set{{"ModelSettings.txt"}, {"Track.txt"}};
     vector_type start{acos(-1.) / 3., 0.26};
     Model to_q(model_set, methodQLearning, start, 5);
     to_q.SetActiveAction(0);
     Model to_sarsa(model_set, methodSARSA, start, 5);
     to_sarsa.SetActiveAction(0);

     sup_st_type MeshPath{{"Mesh.txt"}, {"StateCount.txt"}, {"MeshHistory.txt"}, {"StateCountHistory.txt"}};
     vector_type meshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.05, 0.1, 0.5, 1, 3, 5, INFINITY};
     Int GPos = FindIndex(meshSt, 0);
     Mesh mesh_q(MeshPath, methodQLearning, meshSt, GPos);
     Mesh mesh_sarsa(MeshPath, methodSARSA, meshSt, GPos);

     sup_st_type SupQ{{"Q.txt"}, {"QLog.txt"}, timelearn};
     vector_type TimeLearn{0., 0.1, 50.};    /*t0, dt, T*/
     vector_type Settings{0.6, 0.376, 0.56}; /*Eps, Alf, Gam*/
     Learning Q(Settings, to_q, mesh_q);
     Learning SARSA(Settings, to_sarsa, mesh_sarsa);
     Q.SetMethod(methodQLearning);
     Q.SetPath(SupQ);
     Q.SetTime(TimeLearn);
     Q.Run(1200);
     Q.Test();
     SARSA.SetMethod(methodSARSA);
     SARSA.SetPath(SupQ);
     SARSA.SetTime(TimeLearn);
     SARSA.Run(1200);
     SARSA.Test();
#endif // DEBUG_CLASSES
     return 0;
}
