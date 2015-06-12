#include "generic_plugin/generic_plugin.hpp"

#include "../interfaces/test_interface.hpp"
#include <windows.h>

class TestClassB
	: public Implements< TestInterface > //Always implement latest version
{
public:
	TestClassB( float value )
		: value_( value )
	{
	}

	void test( int value )
	{
		char buffer[ 256 ];
		sprintf( buffer, "%s - %f - %d\n", typeid( *this ).name(), value_, value );
		::OutputDebugStringA( buffer );
	}

private:
	float value_;
};

//==============================================================================
class TestPlugin2
	: public PluginMain
{
public:
	//==========================================================================
	TestPlugin2( IContextManager & contextManager ){}

	//==========================================================================
	bool PostLoad( IContextManager & contextManager )
	{
		contextManager.registerInterface( new TestClassB( 0.5f ) );
		contextManager.registerInterface( new TestClassB( 2.5f ) );
		return true;
	}

	//==========================================================================
	void Initialise( IContextManager & contextManager )
	{

	}
};

PLG_CALLBACK_FUNC( TestPlugin2 )

