#ifndef GLOBAL_TYPE_ID_HPP
#define GLOBAL_TYPE_ID_HPP

#include <stdint.h>
#include <functional>

class TypeId
{
public:
	TypeId( const char * name );
	TypeId( const char * name, uint64_t hashCode );

	uint64_t getHashcode() const { return hashCode_; }
	const char * getName() const { return name_; }

	bool operator == ( const TypeId & other ) const;
	bool operator != ( const TypeId & other ) const;
	bool operator < (const TypeId & other) const;

	//==========================================================================
	template< typename T >
	static const TypeId getType()
	{
		return TypeId( typeid( T ).name() );
	}

private:
	uint64_t		hashCode_;
	const char *	name_;
};

namespace std
{
	template<>
	struct hash< const TypeId >
		: public unary_function< const TypeId, size_t >
	{
	public:
		size_t operator()( const TypeId & v ) const
		{
			hash< uint64_t > hash_fn;
			return hash_fn( v.getHashcode() );
		}
	};
}

#endif //GLOBAL_TYPE_ID_HPP