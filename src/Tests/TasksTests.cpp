#include <boost/test/impl/unit_test_main.ipp>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "../SwarmUnit.hpp"
#define BOOST_TEST_MODULE TasksTest

using namespace swarm;
BOOST_AUTO_TEST_CASE(TasksTest) {
  for (auto i = 0; i < MaximumTaskLvl; ++i) {
    IBaseTask<EmptySwarmUnit> t(i, nullptr, EmptyTaskParams());
    BOOST_CHECK_EQUAL(t.get_lvl(), i);
  }
  BOOST_CHECK_EQUAL(
      IBaseTask<EmptySwarmUnit>(MaximumTaskLvl, nullptr, EmptyTaskParams())
          .get_lvl(),
      MaximumTaskLvl - 1);
  BOOST_CHECK_EQUAL(
      IBaseTask<EmptySwarmUnit>(MaximumTaskLvl * 2, nullptr, EmptyTaskParams())
          .get_lvl(),
      MaximumTaskLvl - 1);
}
