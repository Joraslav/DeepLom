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


  #ifdef DEBUG_INFO
  vector_type MeshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
  r_type GP = -0.1;

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
  r_type r_Rand = dist_r_type(gen);
  cout << "Random r_type is\n" << r_Rand << endl;
  z_type z_Rand = dist_z_type(gen);
  cout << "Random z_type is\n" << z_Rand << endl;

  auto Iter{m1.begin()};
  ++Iter;
  auto NewVec = *Iter;
  auto MaxNewVec = *max_element(begin(NewVec),end(NewVec));
  cout << NewVec << endl;
  cout << "Max Elem from m1(2)\n" << MaxNewVec << endl;

  z_type FindElem;
  FindElem = *find(NewVec.begin(),NewVec.end(),MaxNewVec);
  cout << "After find\n" << FindElem << endl;

  r_type Nu = Metric(v1,v2);
  cout << "Metric on v1 and v2 is " << Nu << endl;

  r_type p{2.};
  matrix_type m5{p*m4};
  std::cout << "p*m4 is " << '\n' << m5 << std::endl;
  #endif   //DEBUG_INFO

  #ifdef DEBUG_CLASSES
  sup_st_type ModelPath{{"ModelSettings.txt"},{"Track.txt"}};
  vector_type Start{0.05, 0.05};
  
  #endif  //DEBUG_CLASSES

  return 0;
}