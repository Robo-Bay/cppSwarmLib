#include <array>
namespace swarm {

// Умножение вектора на скаляр (скаляр слева)
template <std::size_t Dim>
std::array<double, Dim> operator*(double d, std::array<double, Dim> a) {
  for (auto &element : a) {
    element *= d;
  }
  return a;
}

// Умножение вектора на скаляр (скаляр справа)
template <std::size_t Dim>
std::array<double, Dim> operator*(std::array<double, Dim> a, double d) {
  return d * a; // Используем уже реализованную версию
}

// Сложение векторов
template <std::size_t Dim>
std::array<double, Dim> operator+(std::array<double, Dim> a,
                                  const std::array<double, Dim> &b) {
  for (size_t i = 0; i < Dim; ++i) {
    a[i] += b[i];
  }
  return a;
}

template <std::size_t Dim>
std::array<double, Dim> operator-(std::array<double, Dim> a,
                                  const std::array<double, Dim> &b) {
  for (size_t i = 0; i < Dim; ++i) {
    a[i] -= b[i];
  }
  return a;
}

// Покомпонентное умножение векторов
template <std::size_t Dim>
std::array<double, Dim> operator*(std::array<double, Dim> a,
                                  const std::array<double, Dim> &b) {
  for (size_t i = 0; i < Dim; ++i) {
    a[i] *= b[i];
  }
  return a;
}
template <std::size_t Dim>
std::array<double, Dim>
clamp(std::array<double, Dim> a,
      const std::array<std::pair<double, double>, Dim> &e) {
  for (size_t i = 0; i < Dim; ++i) {
    a[i] = std::min(std::max(a[i], e[i].first), e[i].second);
  }
  return a;
}
} // namespace swarm