#include "Data.hpp"

Data::Data(st_type Met)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Data\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT
    this->Method = Met;
    this->NameOfPath = "Data_All";
    this->DataPath = "../" + this->NameOfPath;
    // ifstream iff(this->DataPath);
    // const char *dir = this->DataPath.c_str();
    // struct stat sd;
    if (filesystem::exists(this->DataPath))
    {
        cout << "Path Existence" << endl;
    }
    else
    {
        mkdir(this->DataPath.c_str());
    }
}

void Data::MakeLearnDir(Int iter)
{
    st_type iter_str = to_string(iter);
    if (iter < 10)
    {
        iter_str = '0' + iter_str;
    }
    this->Actual_Learn_Dir = this->DataPath + '/' + this->Method + "_Data_" + iter_str;
    this->Actual_Dir = this->Actual_Learn_Dir;
    if (filesystem::exists(this->Actual_Learn_Dir))
    {
        cout << "Path to learn Existence" << endl;
    }
    else
    {
        mkdir(this->Actual_Learn_Dir.c_str());
    }
}

void Data::MakeTestDir(Int iter)
{
    st_type iter_str = to_string(iter);
    if (iter < 10)
    {
        iter_str = '0' + iter_str;
    }
    this->Actual_Test_Dir = this->DataPath + "/Test" + this->Method + "_Data_" + iter_str;
    this->Actual_Dir = this->Actual_Test_Dir;
    if (filesystem::exists(this->Actual_Test_Dir))
    {
        cout << "Path to test Existence" << endl;
    }
    else
    {
        mkdir(this->Actual_Test_Dir.c_str());
    }
}

void Data::MakeTestDir(Int I, Int J)
{
    st_type I_str = to_string(I);
    st_type J_str = to_string(J);
    if (I < 10)
    {
        I_str = '0' + I_str;
    }
    if (J < 10)
    {
        J_str = '0' + J_str;
    }
    this->Actual_Test_Dir = this->DataPath + "/Test" + this->Method + "_Data_" + I_str + '_' + J_str;
    this->Actual_Dir = this->Actual_Test_Dir;
    if (filesystem::exists(this->Actual_Test_Dir))
    {
        cout << "Path to test Existence" << endl;
    }
    else
    {
        mkdir(this->Actual_Test_Dir.c_str());
    }
}

void Data::WriteVector(vector_type const &Vec, st_type const Name)
{
    os_type Vec_Out(this->Actual_Dir + '/' + Name);
    Vec_Out << Vec;
}

void Data::WriteMatrix(matrix_type const &Matrix, st_type const Name)
{
    os_type Mat_Out(this->Actual_Dir + '/' + Name);
    Mat_Out << Matrix;
}

Data::~Data()
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Distruct of Data\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT
}