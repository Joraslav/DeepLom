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
#include "Data.hpp"
#include "Test.hpp"

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

     vector_type vec_to_read;
     ifstream in("../Mesh.txt");
     in >> vec_to_read;
     in.close();
     cout << vec_to_read << endl;
     matrix_type mat_to_read;
     ifstream in_mat("../FileTest.txt");
     in_mat >> mat_to_read;
     cout << mat_to_read << endl;

#endif // DEBUG_INFO

#ifdef DEBUG_CLASSES
     st_type methodQLearning{"Q-Learning"}, methodSARSA{"SARSA"};
     st_type Track{"Track.txt"};
     st_type MeshName{"Mesh.txt"}, MeshHistoryName{"MeshHistory.txt"};
     st_type StateCountName{"StateCount.txt"}, StateCountHistoryName{"StateCountHistory.txt"};
     sup_st_type NamesMesh{MeshName, MeshHistoryName, StateCountName, StateCountHistoryName};

     vector_type meshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.05, 0.1, 0.3, 0.5, 1, 3, 5, INFINITY};
     Int GPos = FindIndex(meshSt, 0);

     vector_type Start{numbers::pi / 5, 0.2};
     vector_type TimeLearn{0., 0.1, 10};
     vector_type Settings{0.55, 0.6, 0.48}; /*Eps, Alf, Gam*/

     Model Q_Model(methodQLearning, Track, Start, 20);
     Model SARSA_Model(methodSARSA, Track, Start, 20);

     Mesh Q_Mesh(meshSt, GPos, methodQLearning, NamesMesh);
     Mesh SARSA_Mesh(meshSt, GPos, methodSARSA, NamesMesh);

     Learning Q(Settings, Q_Model, Q_Mesh);
     Q.SetTime(TimeLearn);
     Q.SetMethod(methodQLearning);
     Learning SARSA(Settings, SARSA_Model, SARSA_Mesh);
     SARSA.SetTime(TimeLearn);
     SARSA.SetMethod(methodSARSA);

     Data Q_Data(methodQLearning);
     Data SARSA_Data(methodSARSA);

#endif // DEBUG_CLASSES

#ifdef LEARN
     st_type methodQLearning{"Q-Learning"}, methodSARSA{"SARSA"};
     st_type Track{"Track.txt"};
     st_type MeshName{"Mesh.txt"}, MeshHistoryName{"MeshHistory.txt"};
     st_type StateCountName{"StateCount.txt"}, StateCountHistoryName{"StateCountHistory.txt"};
     sup_st_type NamesMesh{MeshName, MeshHistoryName, StateCountName, StateCountHistoryName};

     vector_type meshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.05, 0.1, 0.3, 0.5, 1, 3, 5, INFINITY};
     Int GPos = FindIndex(meshSt, 0);

     vector_type Start{numbers::pi / 5, 0.2};
     vector_type TimeLearn{0., 0.1, 10};    /*t0, dt, T*/
     vector_type Settings{0.55, 0.6, 0.48}; /*Eps, Alf, Gam*/

     Model Q_Model(methodQLearning, Track, Start, 20);
     Model SARSA_Model(methodSARSA, Track, Start, 20);

     Mesh Q_Mesh(meshSt, GPos, methodQLearning, NamesMesh);
     Mesh SARSA_Mesh(meshSt, GPos, methodSARSA, NamesMesh);

     Learning Q(Settings, Q_Model, Q_Mesh);
     Q.SetTime(TimeLearn);
     Q.SetMethod(methodQLearning);
     Learning SARSA(Settings, SARSA_Model, SARSA_Mesh);
     SARSA.SetTime(TimeLearn);
     SARSA.SetMethod(methodSARSA);

     Data Q_Data(methodQLearning);
     Data SARSA_Data(methodSARSA);

     Int Epo{2000};
     for (Int i = 1; i < 31; i++)
     {
          Q_Data.MakeLearnDir(i);
          SARSA_Data.MakeLearnDir(i);

          Q.Run(Epo);

          Q_Data.WriteMatrix(Q.GetMatrix("Q"), "Q.txt");
          Q_Data.WriteMatrix(Q.GetMatrix("QLog"), "QLog.txt");
          Q_Data.WriteMatrix(Q_Mesh.GetMatrix("Mesh"), MeshHistoryName);
          Q_Data.WriteMatrix(Q_Mesh.GetMatrix("StateCount"), StateCountHistoryName);
          Q_Data.WriteVector(Q_Mesh.GetVector("Mesh"), MeshName);
          Q_Data.WriteVector(Q_Mesh.GetVector("StateCount"), StateCountName);

          Q_Model.Reload();
          Q_Mesh.Reload(meshSt, GPos);
          Q.Reload(Settings);

          SARSA.Run(Epo);

          SARSA_Data.WriteMatrix(SARSA.GetMatrix("Q"), "Q.txt");
          SARSA_Data.WriteMatrix(SARSA.GetMatrix("QLog"), "QLog.txt");
          SARSA_Data.WriteMatrix(SARSA_Mesh.GetMatrix("Mesh"), MeshHistoryName);
          SARSA_Data.WriteMatrix(SARSA_Mesh.GetMatrix("StateCount"), StateCountHistoryName);
          SARSA_Data.WriteVector(SARSA_Mesh.GetVector("Mesh"), MeshName);
          SARSA_Data.WriteVector(SARSA_Mesh.GetVector("StateCount"), StateCountName);

          SARSA_Model.Reload();
          SARSA_Mesh.Reload(meshSt, GPos);
          SARSA.Reload(Settings);
     }

#endif // LEARN

#ifdef TEST

     random_device rd;
     mt19937 gen(rd());
     uniform_real_distribution<> dist(-numbers::pi / 3, numbers::pi / 3);

     Test Q_Test(Q_Model, Q_Mesh);
     for (Int i = 1; i < 31; i++)
     {
          st_type i_str = to_string(i);
          if (i < 10)
          {
               i_str = '0' + i_str;
          }
          st_type Path_to{"../Data_All/Q-Learning_Data_" + i_str};
          st_type Path_to_Mesh = Path_to + '/' + "Mesh.txt";
          st_type Path_to_Q = Path_to + '/' + "QLog.txt";
          vector_type Mesh;
          matrix_type QLog;
          if_type mesh_in(Path_to_Mesh);
          if_type qlog_in(Path_to_Q);
          mesh_in >> Mesh;
          mesh_in.close();
          qlog_in >> QLog;
          qlog_in.close();

          vector_type TimeTest{0., 0.1, 200}; /*t0, dt, T*/

          Q_Test.SetMesh(Mesh);
          Q_Test.SetQLog(QLog);
          Q_Test.SetTime(TimeTest);

          // Q_Test.ReadMesh(Path_to_Mesh);
          // Q_Test.ReadQLog(Path_to_Q);
          for (Int j = 1; j < 21; j++)
          {
               Q_Data.MakeTestDir(i, j);

               vector_type TestStart{dist(gen), 0.08};
               Q_Test.SetStart(TestStart);
               Q_Test.RunTest();

               Q_Data.WriteMatrix(Q_Test.GetTrack(), "Track.txt");
               Q_Data.WriteVector(Q_Test.GetMetric(), "Metric.txt");

               Q_Test.Reload();
          }
     }

     Test SARSA_Test(SARSA_Model, SARSA_Mesh);
     for (Int i = 1; i < 31; i++)
     {
          st_type i_str = to_string(i);
          if (i < 10)
          {
               i_str = '0' + i_str;
          }
          st_type Path_to{"../Data_All/SARSA_Data_" + i_str};
          st_type Path_to_Mesh = Path_to + '/' + "Mesh.txt";
          st_type Path_to_Q = Path_to + '/' + "QLog.txt";
          vector_type Mesh;
          matrix_type QLog;
          if_type mesh_in(Path_to_Mesh);
          if_type qlog_in(Path_to_Q);
          mesh_in >> Mesh;
          mesh_in.close();
          qlog_in >> QLog;
          qlog_in.close();

          vector_type TimeTest{0., 0.1, 200}; /*t0, dt, T*/

          SARSA_Test.SetMesh(Mesh);
          SARSA_Test.SetQLog(QLog);
          SARSA_Test.SetTime(TimeTest);

          for (Int j = 1; j < 21; j++)
          {
               SARSA_Data.MakeTestDir(i, j);

               vector_type TestStart{dist(gen), 0.08};
               SARSA_Test.SetStart(TestStart);
               SARSA_Test.RunTest();

               SARSA_Data.WriteMatrix(Q_Test.GetTrack(), "Track.txt");
               SARSA_Data.WriteVector(Q_Test.GetMetric(), "Metric.txt");

               SARSA_Test.Reload();
          }
     }

#endif // TEST

     return 0;
}
