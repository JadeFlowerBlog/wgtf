
#ifndef __I_CONNECTION_H__
#define __I_CONNECTION_H__

#include "core_reflection/reflected_object.hpp"
#include "core_data_model/i_list_model.hpp"

#include "i_slot.hpp"

class IConnection
{
    DECLARE_REFLECTED
public:
    IConnection() = default;
    virtual ~IConnection(){}
    virtual ISlot* Input() const = 0;
    virtual ISlot* Output() const = 0;
    virtual bool Bind(ObjectHandleT<ISlot> outputSlot, ObjectHandleT<ISlot> inputSlot) = 0;
};

#endif //__I_CONNECTION_H__