#include "../Swarm.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

using namespace swarm;
BOOST_AUTO_TEST_CASE(VectorContTest) {
  constexpr auto size = 128;
  SwarmVectorContainer svc(size);
  for (auto i = 0; i < size; i++) {
    svc.add_unit(SwarmUnitLink(new EmptySwarmUnit()));
  }
  BOOST_CHECK_EQUAL(svc.size(), size);
}
BOOST_AUTO_TEST_CASE(SetContTest) {}