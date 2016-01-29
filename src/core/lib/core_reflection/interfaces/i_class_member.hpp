﻿#ifndef I_CLASS_MEMBER_HPP
#define I_CLASS_MEMBER_HPP

class TypeId;

class IClassMember
{
public:
	virtual ~IClassMember() {}
	virtual const TypeId& getType() const = 0;
	virtual const char* getName() const = 0;
};

#endif // I_CLASS_MEMBER_HPP
