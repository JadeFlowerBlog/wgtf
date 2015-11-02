#include "pch.hpp"
#include "test_fixture.hpp"
#include "reflection_test_module.hpp"

#include "core_generic_plugin/interfaces/i_plugin_context_manager.hpp"
#include "core_reflection/reflection_macros.hpp"
#include "core_variant/variant.hpp"
#include "core_python27/defined_instance.hpp"
#include "core_python27/scenario.hpp"


TestFixture::TestFixture( const char * testName,
	TestResult & result )
{
	const char * m_name = testName;
	TestResult & result_ = result;

	std::vector< std::wstring > plugins;
	plugins.push_back( L"plugins/plg_variant" );
	plugins.push_back( L"plugins/plg_reflection" );
	this->load( plugins );

	auto & context = *pluginManager_.getContextManager().getGlobalContext();

	Variant::setMetaTypeManager(
		context.queryInterface< IMetaTypeManager >() );

	IDefinitionManager * pDefinitionManager =
			context.queryInterface< IDefinitionManager >();
	CHECK( pDefinitionManager != nullptr );
	if (pDefinitionManager != nullptr)
	{
		auto & definitionManager = (*pDefinitionManager);
		REGISTER_DEFINITION( ReflectedPython::DefinedInstance );
		REGISTER_DEFINITION( Scenario );
	}

	scriptingEngine_.init( context );

	reflectionModule_.reset( new ReflectionTestModule( context,
		testName,
		result ) );

	interfaces_.push( context.registerInterface( &scriptingEngine_, false ) );
}


TestFixture::~TestFixture()
{
	auto & context = *pluginManager_.getContextManager().getGlobalContext();

	// Module is de-registered by Py_Finalize
	reflectionModule_.reset( nullptr );

	scriptingEngine_.fini( context );

	while (interfaces_.size())
	{
		context.deregisterInterface( interfaces_.top() );
		interfaces_.pop();
	}
}

