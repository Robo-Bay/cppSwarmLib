#include <random>
namespace swarm {
class SwarmStaticRandom {
  static inline std::random_device rd = std::random_device();
  static inline std::mt19937 gen = std::mt19937(rd());

public:
  static double rnd(double min = 0, double max = 1) {
    return std::uniform_real_distribution<>(min, max)(gen);
  }
};
} // namespace swarm
