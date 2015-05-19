#ifndef TEST_REFLECTION_FIXTURE_HPP
#define TEST_REFLECTION_FIXTURE_HPP

#include "bw/testing/reflection_test_objects/test_objects.hpp"

class IStorageLookupHandler;
class ObjectManager;
class DefinitionManager;

class TestReflectionFixture
{
public:
	TestReflectionFixture();
	~TestReflectionFixture();
	IObjectManager & getObjectManager() const;
	IDefinitionManager & getDefinitionManager() const;

	TestStructure & getTestStructure();
private:
	std::unique_ptr< ObjectManager > objectManager_;
	std::unique_ptr< DefinitionManager > definitionManager_;
	std::unique_ptr< IStorageLookupHandler > variantStorageLookupHandler_;

	TestObjects testObjects_;
};
#endif //TEST_REFLECTION_FIXTURE_HPP
