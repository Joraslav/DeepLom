#pragma once

// #include <sys/stat.h>
#include <filesystem>

#include "Glob.hpp"
#include "Learning.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

using namespace tls;
using vector_type = vector_tmpl<Real>;
using matrix_type = matrix_tmpl<Real>;
using sup_st_type = vector_tmpl<st_type>;

class Data
{
private:
    st_type Method, NameOfPath, DataPath;
    st_type Actual_Learn_Dir, Actual_Test_Dir;
    st_type Actual_Dir;
public:
    Data(st_type Met);
    void MakeLearnDir(Int iter);
    void MakeTestDir(Int iter);
    void MakeTestDir(Int I, Int J);
    void WriteMatrix(matrix_type const &Matrix, st_type const Name);
    void WriteVector(vector_type const &Vec, st_type const Name);
    ~Data();
};
