#pragma once

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

namespace tls
{
  using z_type = int;
  using r_type = double;
  using st_type = std::string;
  using os_type = std::ofstream;
  template<class T> using vector_tmpl = std::vector<T>;
  template<class T> using matrix_tmpl = vector_tmpl<vector_tmpl<T>>;
  using vector_type = vector_tmpl<r_type>;
  using matrix_type = matrix_tmpl<r_type>;

  template<class T>
  auto operator+(matrix_tmpl<T> const& left, matrix_tmpl<T> const& right) -> matrix_tmpl<T>;

  template<class T>
  auto operator+(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> vector_tmpl<T>;

  template<class T>
  auto operator<<(std::ostream& out, matrix_tmpl<T> const& m) -> std::ostream&;

  template<class T>
  auto operator<<(std::ostream& out, vector_tmpl<T> const& v) -> std::ostream&;

  template<class T>
  auto operator*(matrix_tmpl<T> const &l, matrix_tmpl<T> const &r) -> matrix_tmpl<T>;

  template<class T>
  auto operator*(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> r_type;

  template <class T>
  auto sgn(T v) -> T;

  template<class T>
  auto Norm(vector_tmpl<T> const& v, const r_type &p) -> r_type;

  template<class T>
  auto Metric(vector_tmpl<T> const& h, vector_tmpl<T> const& f) -> r_type;

  // template<class T>
  // auto IntToIter(vector_tmpl<T> const& v, const z_type &it) -> vector_tmpl<T>::iterator;
}

template<class T>
auto tls::sgn(T v) -> T
{
  return (v < T(0)) ? -1 : ((v > T(0)) ? 1 : 0);
}

template<class T>
auto tls::Norm(vector_tmpl<T> const& v, const r_type &p) -> r_type
{
  auto Rez = 0.0;
  auto const nRows{v.size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    Rez += pow(v[i], p);
  }
  Rez = pow(Rez,1.0/p);

  return Rez;
}

template<class T>
auto tls::Metric(vector_tmpl<T> const& h, vector_tmpl<T> const& f) -> r_type
{
  r_type Rez;

  const auto& hPlusf = h+f;
  const auto& sumh = hPlusf*h;
  const auto& Sign = tls::sgn(sumh);
  const auto& Norm = tls::Norm(hPlusf,2);

  Rez = Norm*(Sign-abs(Sign)+1);

  return Rez;
}

// template<class T>
// auto tls::IntToIter(vector_tmpl<T> const& v, const z_type &it) -> vector_tmpl<T>::iterator;
// {
//   auto iter{v.begin()};
//   iter += it;
//   return iter;
// }

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

template<class T>
auto tls::operator<<(std::ostream& out, matrix_tmpl<T> const& m) ->
  std::ostream&
{
  for (auto const& i : m) {
    for (auto const& j : i) {
      out << j << '\t' << '\t';
    }
    out << '\n';
  }
  return out;
}

template<class T>
auto tls::operator<<(std::ostream& out, vector_tmpl<T> const& v) -> std::ostream&
{
  for (auto const &i : v)
  {
    out << i << '\t' << '\t';
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
auto tls::operator*(vector_tmpl<T> const& l, vector_tmpl<T> const& r) -> r_type
{
  if (l.size() != r.size()){throw std::invalid_argument("wrong sizes");}  //Проверка размерности

  r_type Rez = 0.0;
  auto const nRows{l.size()};

  for (auto i{0u}; i < nRows; ++i)
  {
    Rez += l[i] * r[i];
  }
  
  return Rez;
}