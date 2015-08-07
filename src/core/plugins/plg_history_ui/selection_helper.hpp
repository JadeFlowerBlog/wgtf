#ifndef SELECTION_HELPER_HPP
#define SELECTION_HELPER_HPP

#include <QObject>
#include <QVariant>
#include "core_qt_common/qt_new_handler.hpp"

class HistorySelectionHandler;


// TODO: http://jira.bigworldtech.com/browse/NGT-849
// Eventually, we need to remove this class
class SelectionHelper
    : public QObject
{
    Q_OBJECT

	Q_PROPERTY( QVariant source
	READ getSource
	WRITE setSource
	NOTIFY sourceChanged )

	DECLARE_QT_MEMORY_HANDLER
public:
	typedef HistorySelectionHandler SourceType;


    SelectionHelper( QObject * parent = nullptr );
    ~SelectionHelper();

	void source( SourceType* selectionSource );
	const SourceType* source() const;

	Q_INVOKABLE void select( const QList<QVariant>& selectionList );

private:
	QVariant getSource() const;
	bool setSource( const QVariant& source );

signals:
	void sourceChanged();

private:
	SourceType* source_;

};


#endif //SELECTION_HELPER_HPP
