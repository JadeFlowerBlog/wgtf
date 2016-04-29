
#ifndef __CUSTOM_SLOT_H__
#define __CUSTOM_SLOT_H__

#include "plugins/plg_node_editor/interfaces/i_slot.hpp"

class CustomSlot : public Implements<ISlot>
{
    DECLARE_REFLECTED
public:
    CustomSlot(ObjectHandleT<INode> node, bool isInput)
        : m_isInput(isInput)
        , m_pNode(node)
    {}
    
    CustomSlot(const CustomSlot&) = delete;
    CustomSlot& operator=(const CustomSlot&) = delete;

    virtual ~CustomSlot(){}

    bool IsInput() const override { return m_isInput; }
    std::string Label() const override;
    std::string Icon() const override;
    QColor Color() const override;
    MetaType* Type() const override;
    bool Editable() const override;
    ObjectHandleT<INode> Node() const override;

    const IListModel* GetConnectedSlots() const override;
    bool CanConnect(ObjectHandleT<ISlot> slot) override;
    bool Connect(ObjectHandleT<ISlot> slot) override;
    bool Disconnect(ObjectHandleT<ISlot> slot) override;

private:
    bool m_isInput;

    ObjectHandleT<INode> m_pNode;
    GenericListT<ObjectHandleT<ISlot>> m_connectedSlots;    
};

#endif //__CUSTOM_SLOT_H__