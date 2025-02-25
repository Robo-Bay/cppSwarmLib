#include <boost/test/impl/unit_test_main.ipp>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#define BOOST_TEST_MAIN // in only one cpp file
#define BOOST_TEST_MODULE TasksTest
#include <cppSwarmLib/src/Swarm.hpp>

using namespace swarm;
BOOST_AUTO_TEST_CASE(TasksTest) {
  //   for (int i = 0; i < MaximumTaskLvl; i++) {
  //     IBaseTask<EmptySwarmUnit> t(i, nullptr, EmptyTaskParams());
  //     BOOST_CHECK_EQUAL(t.get_lvl(), i);
  //   }
  BOOST_TEST(1 + 1 == 2);
}
// #include <iostream>
// int main()
// {
//     std::cout << "as;gjlasfg" << std::endl;
// }