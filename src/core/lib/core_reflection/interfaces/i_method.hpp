﻿#ifndef I_METHOD_HPP
#define I_METHOD_HPP

class ObjectHandle;
class ReflectedMethodParameters;
class Variant;

class IMethod
{
public:
	virtual ~IMethod() {}
	virtual Variant invoke( const ObjectHandle& object, const ReflectedMethodParameters& parameters ) = 0;
	// TODO return a collection of the arg types
	virtual size_t parameterCount() const = 0;
};

#endif // I_METHOD_HPP