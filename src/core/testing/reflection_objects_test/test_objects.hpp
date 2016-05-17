#ifndef TEST_OBJECTS_HPP
#define TEST_OBJECTS_HPP

#include "test_structure.hpp"

namespace wgt
{
class TestObjects
{
public:
	void initDefs( IDefinitionManager & definitionManager );
	void finiDefs( IDefinitionManager & definitionManager );

	TestStructure & getTestStructure();

private:
	TestStructure structure_;
};


}
#endif //TEST_OBJECTS_HPP
