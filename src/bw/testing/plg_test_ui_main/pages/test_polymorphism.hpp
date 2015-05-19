#ifndef TEST_POLYMORPHISM_HPP
#define TEST_POLYMORPHISM_HPP

#include "reflection/reflected_object.hpp"
#include "testing/reflection_test_objects/test_polystruct.hpp"
#include "math/vector3.hpp"

class TestPolyCheckBox
	: public TestPolyStruct
{
	DECLARE_REFLECTED
public:
	TestPolyCheckBox();
private:
	bool checked_;
};

class TestPolyTextField
	: public TestPolyStruct
{
	DECLARE_REFLECTED
public:
	TestPolyTextField();
private:
	void getText( std::string * text ) const;
	void setText( const std::string & text );

	void getNumber( int * num ) const;
	void setNumber( const int & num );

	std::string text_;
	int num_;
};

class TestPolyComboBox
	: public TestPolyStruct
{
	DECLARE_REFLECTED
public:
	TestPolyComboBox();
private:
	virtual void getSelected( int * select ) const;
	virtual void setSelected( const int & select );

	int curSelected_;
};

class TestPolyColor3
	: public TestPolyComboBox
{
	DECLARE_REFLECTED
public:
	TestPolyColor3();
private:
	void getColor3( BW::Vector3 * color ) const;
	void setColor3( const BW::Vector3 & color );
	virtual void getSelected( int * select ) const;
	virtual void setSelected( const int & select );

	BW::Vector3 color3_;
	int seletColor_;
	const BW::Vector3 red_;
	const BW::Vector3 green_;
	const BW::Vector3 blue_;
};
#endif // TEST_POLYMORPHISM_HPP