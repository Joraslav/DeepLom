#include <vector> // std::vector
#include <iostream> // std::{ostream, cout, endl}
#include <stdexcept> // std::invalid_argument
#include <ranges> // std::views::iota

#include "Glob.hpp"
#include "Learning.hpp"

int main()
{
  using namespace tls;
  // using matrix_type = matrix_tmpl<int>;
  // using vector_type = vector_tmpl<int>;

  matrix_tmpl<z_type> m1{{1, 2, 3}, {4, 5, 6}}, m2{m1};
  matrix_tmpl<z_type> m3{{1,0,0},{0,1,0},{0,0,1}}, m4{{2,0,0},{0,2,0},{0,0,2}};

  std::cout << "m1:\n" << m1 << std::endl;
  std::cout << "m2:\n" << m2 << std::endl;
  std::cout << "m1 + m2:\n" << m1 + m2 << std::endl;

  vector_tmpl<r_type> v1{0,-4,-3}, v2{0,4,3}, v3{0,16,9};

  std::cout << "v1:\n" << v1 << std::endl;
  std::cout << "v2:\n" << v2 << std::endl;
  std::cout << "v1+v2:\n" << v1+v2 << std::endl;

  vector_tmpl<r_type> MeshSatate{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};

  r_type p=2.0;
  std::cout << "Norm v1\n" << Norm(v1,p) << std::endl;

  std::cout << "Sclar product of vector:\n" << v1*v2 << std::endl;

  std::cout << "Multiplication m3*m4:\n" << m4*m3 << std::endl;

  std::cout << "Mesh is\n" << MeshSatate << std::endl;

  std::cout << "Sign v1(1)\n" << sgn(v1[1]) << std::endl;

  std::cout << "Metric v1 and v2\n" << Metric(v1,v3) << std::endl;

  st_type Metod = "SARSA", QP = "Q.txt", TrackP = "Track.txt";
  r_type Epsi = 0.5, Alfi = 0.3, Gamu = 0.4;
  z_type s=10, a=4;
  vector_tmpl<r_type> Time{0,0.1,10}; /*t0, dt, T*/

  Learning test(Metod,Epsi,Alfi,Gamu);
  test.SetMesh(MeshSatate);
  test.SetTime(Time);
  test.SetPath(QP,TrackP);
  test.GenerateQ(s,a);
  // test.SetMeshHistory(MeshSatate);
  test.GetQ();
  test.Run(100);
  return 0;
}