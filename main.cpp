#include <vector>    // std::vector
#include <iostream>  // std::{ostream, cout, endl}
#include <stdexcept> // std::invalid_argument
#include <ranges>    // std::views::iota
#include <iterator>   //iterator

#include "Glob.hpp"
#include "Learning.hpp"
#include "Mesh.hpp"
 
using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;

int main()
{
  #ifdef DEBUG_ADAPTIVE
  vector_type StateCount{190,0,23,0,34,1,2,0,35,0,237};
  vector_type ActMesh{-INFINITY, -5, -3, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, 3, INFINITY};
  Int Goal = FindIndex(ActMesh,0);
  cout << "Goal is\n" << Goal << endl;
  matrix_type QL(StateCount.size(),{0,0,0});
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
  Zeros = FindIndex(StateCount,is_zero);
  cout << "Index zeros in StateCount\n" << Zeros << endl;
  cout << "ActMesh\n" << ActMesh << endl;
  Int Remove{0};
  for (auto &index : Zeros)
  {
    if ((index!=Goal-Remove) && (index!=Goal-1-Remove) && (index!=Goal+1-Remove))
    {
      ActMesh[index+1-Remove] = (ActMesh[index-Remove]+ActMesh[index+1-Remove])*0.5;
      ActMesh.erase(ActMesh.begin()+index-Remove);
      QL.erase(QL.begin()+(index-Remove));
      cout << "ActMesh\n" << ActMesh << endl;
      StateCount.erase(StateCount.begin()+(index-Remove));
      Remove++;
    }
  }  
  #endif //DEBUG_ADAPTIVE


  #ifdef DEBUG_INFO
  matrix_type m1{{1, 2, 3}, {4, 5, 6}}, m2{m1};
  vector_type v1{0,-4,-3}, v2{0,4,3}, v3{0,16,9};
  matrix_type m3{{1,0,0},{0,1,0},{0,0,1}}, m4{{2,0,0},{0,2,0},{0,0,2}};

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

  Int param = 6;
  auto Vec_m1_iter{m1.begin()+1};
  auto Vec_m1 = *Vec_m1_iter;
  auto Index_VecM1 = FindIndex(Vec_m1,param);
  cout << "After FindIndex\n" << Index_VecM1 << endl;

  v3.insert(v3.begin()+3,0);
  cout << v3 << endl;

  #endif   //DEBUG_INFO

  #ifdef DEBUG_CLASSES
  sup_st_type ModelPath{{"ModelSettings.txt"},{"Track.txt"}};
  vector_type Start{M_PI_4, 0.18};
  Model model(ModelPath,Start,3);
  model.SetActiveAction(0);

  sup_st_type MeshPath{{"Mesh.txt"},{"StateCount.txt"},{"MeshHistory.txt"}};
  vector_type MeshSt{-INFINITY, -5, -1, -0.5, -0.1, 0, 0.1, 0.5, 1, INFINITY};
  Int GPos = FindIndex(MeshSt,0);
  Mesh mesh(MeshPath,MeshSt,GPos);

  st_type QPath{"Q.txt"};
  vector_type TimeLearn{0,0.1,10};     /*t0, dt, T*/
  vector_type Settings{0.5,0.476,0.56};   /*Eps, Alf, Gam*/
  Learning training(Settings,model,mesh);
  training.SetPath(QPath);
  training.SetTime(TimeLearn);
  training.RandomQ();
  training.Run(1);
  #endif  //DEBUG_CLASSES

  return 0;
}