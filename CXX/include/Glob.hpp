#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <stdexcept> // std::invalid_argument
#include <ranges> // std::views::iota
#include <algorithm>  //std::minmax_element
#include <random>   //randomize
#include <iterator>   //iterator
#include <direct.h>   //making folder

// #define DEBUG_INFO
#define DEBUG_CONSTRUCT_DISTRUCT
#define DEBUG_CLASSES
// #define DEBUG_REWARD

namespace tls
{
  using namespace std;
  using Int = int;
  using Real = long double;
  using st_type = string;
  using os_type = ofstream;
  template<class T> using vector_tmpl = vector<T>;
  template<class T> using matrix_tmpl = vector_tmpl<vector_tmpl<T>>;

  template<class T>
  auto operator+(matrix_tmpl<T> const& l, matrix_tmpl<T> const& r) -> matrix_tmpl<T>;

  template<class T>
  auto operator-(matrix_tmpl<T> const& l, matrix_tmpl<T> const& r) -> matrix_tmpl<T>;

  template<class T>
  auto operator+(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> vector_tmpl<T>;

  template<class T>
  auto operator-(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> vector_tmpl<T>;

  template<class T>
  auto operator<<(std::ostream& out, matrix_tmpl<T> const& m) -> ostream&;

  template<class T>
  auto operator<<(std::ostream& out, vector_tmpl<T> const& v) -> ostream&;

  template<class T>
  auto operator*(matrix_tmpl<T> const &l, matrix_tmpl<T> const &r) -> matrix_tmpl<T>;

  template<class T>
  auto operator*(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> Real;

  template<class T>
  auto operator*(T const& c, vector_tmpl<T> const& v) -> vector_tmpl<T>;

  template<class T>
  auto operator*(T const& c, matrix_tmpl<T> const& m) -> matrix_tmpl<T>;

  template<class T>
  auto operator*(matrix_tmpl<T> const& m, vector_tmpl<T> const& v) -> vector_tmpl<T>;

  template <class T>
  auto sgn(T v) -> T;

  template<class T>
  auto Norm(vector_tmpl<T> const& v) -> Real;

  template<class T>
  auto Metric(vector_tmpl<T> const& h, vector_tmpl<T> const& f) -> Real;

  template<class T, class U>
  auto FindIndex(vector_tmpl<T> const& v, const U elem) -> Int;

  template<class T>
  auto Sum(vector_tmpl<T> const &v) -> T;
}

template<class T>
auto tls::sgn(T v) -> T
{
  return (v < T(0)) ? -1 : ((v > T(0)) ? 1 : 0);
}

template<class T>
auto tls::Norm(vector_tmpl<T> const& v) -> Real
{
  auto Rez = 0.0;
  auto ScalarProd = v*v;
  Rez = sqrtl(ScalarProd);
  return Rez;
}

template<class T>
auto tls::Metric(vector_tmpl<T> const& h, vector_tmpl<T> const& f) -> Real
{
  Real Rez;

  const auto& hPlusf = h+f;
  const auto& sumh = hPlusf*h;
  const auto& Sign = tls::sgn(sumh);
  const auto& Norm = tls::Norm(hPlusf);

  Rez = Norm*(Sign-abs(Sign)+1);

  return Rez;
}

template<class T, class U>
auto tls::FindIndex(vector_tmpl<T> const& v, const U elem) -> Int
{
  Int Rez;
  auto it = find(v.begin(),v.end(),elem);
  if (it != v.end())
  {
    Rez = it - v.begin();
  }
  else
  {
    cout << "Can't find index" << endl;
    return 1;
  }
  return Rez;
}

template<class T>
auto tls::Sum(vector_tmpl<T> const &v) -> T
{
  T Rez;
  auto const nCols{v.size()};

  for (auto i{0u}; i < nCols; ++i)
  {
    Rez += v[i];
  }

  return Rez;  
}

template <class T>
auto tls::operator+(matrix_tmpl<T> const& l, matrix_tmpl<T> const& r) -> matrix_tmpl<T>
{
    if (l.size() != r.size()) { throw std::invalid_argument("wrong sizes"); }  //Проверка размерности

    auto m{l};
    auto const nRows{m.size()}, nCols{m.front().size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    for (auto j{0u}; j < nCols; ++j)
    {
      m[i][j] += r[i][j];
    }
  }
  return m;
}

template <class T>
auto tls::operator-(matrix_tmpl<T> const& l, matrix_tmpl<T> const& r) -> matrix_tmpl<T>
{
    if (l.size() != r.size()) { throw std::invalid_argument("wrong sizes"); }  //Проверка размерности

    auto m{l};
    auto const nRows{m.size()}, nCols{m.front().size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    for (auto j{0u}; j < nCols; ++j)
    {
      m[i][j] -= r[i][j];
    }
  }
  return m;
}

template <class T>
auto tls::operator+(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> vector_tmpl<T>
{
  if (l.size() != r.size()){throw std::invalid_argument("wrong sizes");}  //Проверка размерности

  auto Rez{l};
  auto const nRows{Rez.size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    Rez[i] += r[i];
  }
  
  return Rez;
}

template <class T>
auto tls::operator-(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> vector_tmpl<T>
{
  if (l.size() != r.size()){throw std::invalid_argument("wrong sizes");}  //Проверка размерности

  auto Rez{l};
  auto const nRows{Rez.size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    Rez[i] -= r[i];
  }
  
  return Rez;
}

template<class T>
auto tls::operator<<(std::ostream& out, matrix_tmpl<T> const& m) -> ostream&
{
  for (auto const& i : m) {
    for (auto const& j : i) {
      out << j << '\t';
    }
    out << '\n';
  }
  return out;
}

template<class T>
auto tls::operator<<(std::ostream& out, vector_tmpl<T> const& v) -> ostream&
{
  for (auto const &i : v)
  {
    out << i << '\t';
  }
  return out;
}

template<class T>
auto tls::operator*(matrix_tmpl<T> const &l, matrix_tmpl<T> const &r) -> matrix_tmpl<T>
{
  if (l.front().size() != r.size()){throw std::invalid_argument("wrong sizes");}  //Проверка размерности

  auto const nRows{l.size()}, nCols{r.front().size()};
  matrix_tmpl<T> Rez;
  Rez.resize(nRows);
  for (auto i{0u}; i < nRows; ++i)
  {
    Rez[i].resize(nCols);
  }
  
  for (auto i{0u}; i < nRows; ++i)
  {
    for (auto j{0u}; j < nCols; ++j)
    {
      Rez[i][j] = 0;
      for (auto k{0u}; k < r.size(); ++k)
      {
        Rez[i][j] += l[i][k]*r[k][j];
      }
    }
  }

  return Rez;
}

template<class T>
auto tls::operator*(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> Real
{
  if (l.size() != r.size()){throw std::invalid_argument("wrong sizes");}  //Проверка размерности

  Real Rez = 0.0;
  auto const nRows{l.size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    Rez += l[i] * r[i];
  }
  
  return Rez;
}

template<class T>
auto tls::operator*(T const& c, vector_tmpl<T> const& v) -> vector_tmpl<T>
{
  vector_tmpl<T> Rez{v};
	auto const nRows{v.size()};
	for (auto i{0u}; i < nRows; ++i)
	{
		Rez[i] = c*Rez[i];
	}
	return Rez;
}

template<class T>
auto tls::operator*(T const& c, matrix_tmpl<T> const& m) -> matrix_tmpl<T>
{
  matrix_tmpl<T> Rez{m};
  auto const nRows{m.size()}, nCols{m.front().size()};
  for (auto i{0u}; i < nRows; ++i)
  {
    for (auto j{0u}; j < nCols; ++j)
    {
      Rez[i][j] = c*Rez[i][j];
    }
  }
  return Rez;
}

template<class T>
auto tls::operator*(matrix_tmpl<T> const& m, vector_tmpl<T> const& v) -> vector_tmpl<T>
{
  if (m.front().size() != v.size()){throw std::invalid_argument("wrong sizes");}  //Проверка размерности
  vector_tmpl<T> Rez{v};
  for (auto i{0u}; i < m.size(); ++i)
  {
    Rez[i] = m[i]*v;
  }
  return Rez;
}