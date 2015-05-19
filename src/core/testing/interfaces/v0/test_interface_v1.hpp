#ifndef V0_TEST_INTERFACE_V1_HPP
#define V0_TEST_INTERFACE_V1_HPP

#include "test_interface_v0.hpp"

DECLARE_INTERFACE_BEGIN( TestInterface, 0, 1 )
	virtual void test() = 0;
	virtual void test2() = 0;
DECLARE_INTERFACE_END()

#endif //V0_TEST_INTERFACE_V1_HPP
