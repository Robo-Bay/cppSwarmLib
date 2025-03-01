#include "../SwarmUnit.hpp"
#include <boost/test/unit_test.hpp>

using namespace swarm;
BOOST_AUTO_TEST_CASE(TasksTest) {
  constexpr auto l = 5;
  ITask<l, EmptySwarmUnit> t(nullptr);
  BOOST_CHECK_EQUAL(t.get_lvl(), l);
}
