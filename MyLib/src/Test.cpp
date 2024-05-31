#include "Test.hpp"

Test::Test(Learning &L, Model &Mod, Mesh &Mes) : Learn_(L), Model_(Mod), Mesh_(Mes)
{
#ifdef DEBUG_CONSTRUCT_DISTRUCT
    std::cout << "Construct of Test\t" << this << std::endl;
#endif // DEBUG_CONSTRUCT_DISTRUCT
}

void Test::ReadMesh(st_type const Mesh_Path)
{
    if_type mesh_in(Mesh_Path);
    mesh_in >> this->ActMesh_;
    mesh_in.close();
}

void Test::ReadQLog(st_type const QLog_Path)
{
    if_type qlog_in(QLog_Path);
    qlog_in >> this->QLog_;
    qlog_in.close();
}

auto Test::GetTrack() -> matrix_type
{
    return this->Track_;
}

Test::~Test()
{
}