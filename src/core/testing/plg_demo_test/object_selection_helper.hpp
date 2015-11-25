#ifndef OBJECT_SELECTION_HELPER_HPP
#define OBJECT_SELECTION_HELPER_HPP

#include <QObject>
#include <QVariant>
#include "core_data_model/i_value_change_notifier.hpp"
#include "core_reflection/object_handle.hpp"

class Variant;

class ObjectSelectionHelper
	: public IValueChangeNotifier
{

public:
    ObjectSelectionHelper();
    ~ObjectSelectionHelper();
	void init(const ObjectHandle & value);
	Variant variantValue() const override;
	bool variantValue( const Variant& data ) override;

	const ObjectHandle& value() const;
	void value( const ObjectHandle& data );

private:
	ObjectSelectionHelper( const ObjectSelectionHelper& other );
	ObjectSelectionHelper& operator=( const ObjectSelectionHelper& other );
	ObjectHandle	value_;
};


#endif //SELECTION_HELPER_HPP