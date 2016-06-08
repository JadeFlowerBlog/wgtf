#include "buttons_definition_extension.hpp"

#include "core_data_model/i_item_role.hpp"
#include "core_data_model/i_item.hpp"
#include "helpers/qt_helpers.hpp"

namespace wgt
{

QHash<int, QByteArray> ButtonsDefinitionExtension::roleNames() const
{
    QHash<int, QByteArray> result;
    registerRole(buttonsDefinitionRole::roleName_, result);
    return result;
}

QVariant ButtonsDefinitionExtension::data(const QModelIndex& index, int role) const
{
    size_t roleId;
    if (!decodeRole(role, roleId))
    {
        return QVariant();
    }

    assert(index.isValid());
    IItem* item = reinterpret_cast<IItem *>(index.internalPointer());
    if (item == nullptr || roleId != buttonsDefinitionRole::roleId_)
    {
        return QVariant();
    }

    return QtHelpers::toQVariant(item->getData(index.column(), roleId), nullptr);
}

bool ButtonsDefinitionExtension::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return false;
}

} // namespace wgt