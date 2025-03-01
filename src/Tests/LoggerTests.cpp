#include "../Logger.hpp"
#include <boost/test/unit_test.hpp>

using namespace swarm;
BOOST_AUTO_TEST_CASE(LoggerTests) { BOOST_CHECK_EQUAL(sizeof(EmptyLogger), 8); }
