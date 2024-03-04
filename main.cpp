#include <vector>    // std::vector
#include <iostream>  // std::{ostream, cout, endl}
#include <stdexcept> // std::invalid_argument
#include <ranges>    // std::views::iota

#include "Glob.hpp"
#include "Learning.hpp"
#include "Mesh.hpp"
 
using namespace tls;
using vector_type = vector_tmpl<r_type>;
using matrix_type = matrix_tmpl<r_type>;

int main()
{
  // matrix_type m1{{1, 2, 3}, {4, 5, 6}}, m2{m1};
  // matrix_type m3{{1,0,0},{0,1,0},{0,0,1}}, m4{{2,0,0},{0,2,0},{0,0,2}};

  vector_type MeshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
  r_type GP = -0.1;

  // vector_type v1{0,-4,-3}, v2{0,4,3}, v3{0,16,9};

  #ifdef DEBUG_INFO
  std::cout << "m1:\n" << m1 << std::endl;
  std::cout << "m2:\n" << m2 << std::endl;
  std::cout << "m1 + m2:\n" << m1 + m2 << std::endl;

  std::cout << "v1:\n" << v1 << std::endl;
  std::cout << "v2:\n" << v2 << std::endl;
  std::cout << "v1+v2:\n" << v1+v2 << std::endl;

  r_type p=2.0;
  std::cout << "Norm v1\n" << Norm(v1,p) << std::endl;
  std::cout << "Sclar product of vector:\n" << v1*v2 << std::endl;
  std::cout << "Multiplication m3*m4:\n" << m4*m3 << std::endl;
  std::cout << "Mesh is\n" << MeshSatate << std::endl;
  std::cout << "Sign v1(1)\n" << sgn(v1[1]) << std::endl;
  std::cout << "Metric v1 and v2\n" << Metric(v1,v3) << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist_r_type(0.0, 1.0);
  std::uniform_int_distribution<> dist_z_type(1, 5);
  r_type r_Rand = dist_r_type(gen);
  std::cout << "Random r_type is\n" << r_Rand << std::endl;
  z_type z_Rand = dist_z_type(gen);
  std::cout << "Random z_type is\n" << z_Rand << std::endl;

  auto Iter{m1.begin()};
  ++Iter;
  auto NewVec = *Iter;
  auto MaxNewVec = *std::max_element(begin(NewVec),end(NewVec));
  std::cout << NewVec << std::endl;
  std::cout << "Max Elem from m1(2)\n" << MaxNewVec << std::endl;

  z_type FindElem;
  FindElem = *std::find(NewVec.begin(),NewVec.end(),MaxNewVec);
  std::cout << "After find\n" << FindElem << std::endl;
  #endif   //DEBUG_INFO

  st_type ModelP{"ModelSettings.txt"};
  z_type NumMus = 2;
  st_type NameMus("Lalu");
  ModelSettings ModelTest(NumMus, NameMus);
  ModelTest.SetPath(ModelP);

  st_type ActMesh{"ActMesh.txt"}, StateCount{"StateCount.txt"}, MeshHistiry{"MeshHistory.txt"};
  sup_st_type SupMeshPath{ActMesh,StateCount,MeshHistiry};  /*Mesh, Count, History*/
  Mesh MeshState(MeshSt,GP);
  MeshState.SetPath(SupMeshPath);

  st_type Metod("SARSA");
  st_type QP("Q.txt"), TrackP("Track.txt"), MeshHP("MeshHistory.txt");
  sup_st_type SupTestPath{QP,TrackP,MeshHP};
  r_type Epsi = 0.5, Alfi = 0.3, Gamu = 0.4;
  vector_type SetTest{Epsi, Alfi, Gamu};  /*Epsilon, Alfa, Gamma*/
  vector_type Time{0, 0.1, 1}; /*t0, dt, T*/
  z_type s=4, a=NumMus;

  Learning test(SetTest, ModelTest, MeshState);
  test.SetTime(Time);
  test.SetPath(SupTestPath);
  test.GenerateQ(s,pow(2,a));

  test.Run(2);

  test.RandomQ();
  test.GetQ();
  test.GreedyPolicy(s);

  return 0;
}