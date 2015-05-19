#ifndef TEST_OBJECTS_COMMON_HPP
#define TEST_OBJECTS_COMMON_HPP

#include "reflection/i_definition_manager.hpp"

namespace TestObjectCommon
{
	static const wchar_t * s_MemberVariableGroup = L"Member variables";
	static const wchar_t * s_MemberStdCollectionVariableGroup = L"Member std collection variables";
	static const wchar_t * s_MemberBWCollectionVariableGroup = L"Member bw collection variables";

	template< typename T >
	struct RandomValueGenerator
	{
		static T value( const IDefinitionManager & defManager )
		{
			return T();
		}
	};


	template< typename T >
	struct RandomValueGenerator< T * >
	{
		static T * value( const IDefinitionManager & defManager )
		{
			T * pType = nullptr;
			return valueInt( pType, defManager );
		}

		static T * valueInt(
			const ReflectedPolyStruct * pStruct,
			const IDefinitionManager & defManager )
		{
			std::vector< IClassDefinition * > definitions;
			defManager.getDefinitionsOfType(
				getClassIdentifier< T >(), definitions );
			if (definitions.size() == 0)
			{
				return nullptr;
			}
			return
				definitions[ rand() % definitions.size() ]->create();
		}


		static T * valueInt( ... )
		{
			return nullptr;
		}
	};


	template<>
	struct RandomValueGenerator< bool >
	{
		static bool value( const IDefinitionManager & )
		{
			return ( std::rand() % 2 ) == 0 ? true : false; 
		}
	};

	template<>
	struct RandomValueGenerator< int32_t >
	{
		static int32_t value( const IDefinitionManager & )
		{
			static uint64_t range  = ( uint64_t ) INT_MAX - INT_MIN;
			return ( int32_t ) ( ( ( float ) std::rand() / RAND_MAX * range ) + INT_MIN );
		}
	};

	template<>
	struct RandomValueGenerator< uint32_t >
	{
		static uint32_t value( const IDefinitionManager & )
		{
			return ( uint32_t ) ( ( float ) std::rand() / RAND_MAX * UINT_MAX );
		}
	};


	template<>
	struct RandomValueGenerator< int64_t >
	{
		static int64_t value( const IDefinitionManager & )
		{
			static uint64_t range = INT64_MAX - INT64_MIN;
			return ( int64_t ) ( ( double ) std::rand() / RAND_MAX * range );
		}
	};


	template<>
	struct RandomValueGenerator< uint64_t >
	{
		static uint64_t value( const IDefinitionManager & )
		{
			return ( uint64_t ) ( ( double ) std::rand() / RAND_MAX * UINT64_MAX );
		}
	};


	template<>
	struct RandomValueGenerator< float >
	{
		static float value( const IDefinitionManager & )
		{
			return ( float ) std::rand() / RAND_MAX;
		}
	};


	template<>
	struct RandomValueGenerator< double >
	{
		static double value( const IDefinitionManager & )
		{
			return ( double ) std::rand() / RAND_MAX;
		}
	};


	template<>
	struct RandomValueGenerator< BW::Vector3 >
	{
		static BW::Vector3 value( const IDefinitionManager & defManager )
		{
			return BW::Vector3(
				RandomValueGenerator< float >::value( defManager ),
				RandomValueGenerator< float >::value( defManager ),
				RandomValueGenerator< float >::value( defManager ) );
		}
	};

	template<>
	struct RandomValueGenerator< BW::Vector4 >
	{
		static BW::Vector4 value( const IDefinitionManager & defManager )
		{
			return BW::Vector4( 
				RandomValueGenerator< BW::Vector3 >::value( defManager ),
				RandomValueGenerator< float >::value( defManager ) );
		}
	};

	template< typename T >
	struct RandomValueGenerator< std::shared_ptr< T > >
	{
		static std::shared_ptr< T > value( const IDefinitionManager & defManager )
		{
			return std::shared_ptr< T >(
				RandomValueGenerator< T * >::value( defManager ));
		}
	};


	template< typename T, typename Collection >
	void populateData(
		const IDefinitionManager & defManager,
		Collection & collection, size_t itemsToGenerate = 10 )
	{
		for( auto i = 0; i < itemsToGenerate; ++i )
		{
			collection.push_back(
				RandomValueGenerator< T >::value( defManager ) );
		}
	}
};


#endif //TEST_OBJECTS_COMMON_HPP