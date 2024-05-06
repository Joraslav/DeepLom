#include <vector>    // std::vector
#include <iostream>  // std::{ostream, cout, endl}
#include <stdexcept> // std::invalid_argument
#include <ranges>    // std::views::iota

#include "Glob.hpp"
#include "Learning.hpp"
#include "Mesh.hpp"
 
using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;

int main()
{


  #ifdef DEBUG_INFO
  vector_type MeshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};

  matrix_type m1{{1, 2, 3}, {4, 5, 6}}, m2{m1};
  vector_type v1{0,-4,-3}, v2{0,4,3}, v3{0,16,9};
  matrix_type m3{{1,0,0},{0,1,0},{0,0,1}}, m4{{2,0,0},{0,2,0},{0,0,2}};
  cout << "m1:\n" << m1 << endl;
  cout << "m2:\n" << m2 << endl;
  cout << "m1 + m2:\n" << m1 + m2 << endl;

  cout << "v1:\n" << v1 << std::endl;
  cout << "v2:\n" << v2 << std::endl;
  cout << "v1+v2:\n" << v1+v2 << std::endl;

  cout << "Norm v1\n" << Norm(v1) << endl;
  cout << "Sclar product of vector:\n" << v1*v2 << endl;
  cout << "Multiplication m3*m4:\n" << m4*m3 << endl;
  cout << "Sign v1(1)\n" << sgn(v1[1]) << endl;
  cout << "Metric v1 and v3\n" << Metric(v1,v3) << endl;

  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dist_r_type(0.0, 1.0);
  uniform_int_distribution<> dist_z_type(1, 5);
  Real r_Rand = dist_r_type(gen);
  cout << "Random r_type is\n" << r_Rand << endl;
  Int z_Rand = dist_z_type(gen);
  cout << "Random z_type is\n" << z_Rand << endl;

  Int param = 16;
  Int Index_v3 = FindIndex(v3,param);
  cout << "Index of 16 in v3 is\n" << Index_v3 << endl;

  Real Nu = Metric(v1,v2);
  cout << "Metric on v1 and v2 is " << Nu << endl;

  Real p{2.};
  matrix_type m5{p*m4};
  std::cout << "p*m4 is " << '\n' << m5 << std::endl;

  matrix_type Mat{{1,0,0},{0,1,0},{0,0,1}};
  cout << Mat << endl;
  auto Vec{Mat.back()};
  Mat.push_back(Vec);
  cout << "Iter_i is\n" << Mat << endl;


  #endif   //DEBUG_INFO

  #ifdef DEBUG_CLASSES
  sup_st_type ModelPath{{"ModelSettings.txt"},{"Track.txt"}};
  vector_type Start{0.05, 0.05};
  Model model(ModelPath,Start);
  model.SetActiveAction(0);

  sup_st_type MeshPath{{"Mesh.txt"},{"StateCount.txt"},{"MeshHistory.txt"}};
  vector_type MeshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
  Int GPos = FindIndex(MeshSt,0);
  Mesh mesh(MeshPath,MeshSt,GPos);

  st_type QPath{"Q.txt"};
  vector_type TimeLearn{0,0.1,1};     /*t0, dt, T*/
  vector_type Settings{0.5,0.476,0.56};   /*Eps, Alf, Gam*/
  Learning training(Settings,model,mesh);
  training.SetPath(QPath);
  training.SetTime(TimeLearn);
  training.Run(1);
  #endif  //DEBUG_CLASSES

  return 0;
}