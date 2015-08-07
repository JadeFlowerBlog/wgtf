#ifndef QT_SCRIPTING_ENGINE_HPP
#define QT_SCRIPTING_ENGINE_HPP

/*
The QtScriptingEngine manages QtScriptObjects to allow C++ classes
to be used in QML.
This is used internally by the QtUIFramework to create
context objects and properties.
Details: https://confluence.wargaming.net/display/NGT/NGT+Reflection+System
*/

#include "core_reflection/utilities/reflection_utilities.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <QObject>
#include <QVariant>

class IClassDefinition;
class IDefinitionManager;
class IQtFramework;
class IQtTypeConverter;
class IComponentContext;
class ICommandManager;
class ICopyPasteManager;
class BWCopyable;
class ObjectHandle;
class QtScriptObject;

class QtScriptingEngine : public QObject
{
	Q_OBJECT

public:
	QtScriptingEngine();
	virtual ~QtScriptingEngine();

	void initialise( 
		IQtFramework & qtFramework, IComponentContext & contextManager );

	void finalise();

	template< typename T >
	QtScriptObject * createScriptObject( const T & object )
	{
		if (defManager_ == nullptr)
		{
			return nullptr;
		}

		auto provider = 
			ReflectionUtilities::generateBaseProvider( object, *defManager_ );

		return createScriptObject( provider );
	}

	QtScriptObject * createScriptObject( const ObjectHandle & object );

protected:
	// TODO: These invokables need to be refactored into different modules to
	// reduce the bloat of this class
	Q_INVOKABLE QObject * createObject( QString definition );
	Q_INVOKABLE bool queueCommand( QString command );
	Q_INVOKABLE void beginUndoFrame();
	Q_INVOKABLE void endUndoFrame();
	Q_INVOKABLE void abortUndoFrame();
	Q_INVOKABLE void deleteMacro( QString command );
	Q_INVOKABLE void selectControl( BWCopyable* control, bool append = true );
	Q_INVOKABLE void deselectControl( BWCopyable* control, bool reset = false );
	Q_INVOKABLE QObject * iterator( const QVariant & collection );
	// this function is used to resolve breaking binding issue for checkbox and pushbutton, since
	// clicking on checkbox or pushbutton will break the "checked" property binding
	//see: https://bugreports.qt.io/browse/QTBUG-42505 for reference
	Q_INVOKABLE bool setValueHelper( QObject * object, QString property, QVariant value );

private:
	QMetaObject * getMetaObject(
		const IClassDefinition & classDefinition,
		int& firstMethodIndex );

	const IDefinitionManager * defManager_;
	ICommandManager * commandSystemProvider_;
	ICopyPasteManager * copyPasteManager_;
	IComponentContext * contextManager_;

	std::mutex metaObjectsMutex_;
	std::map< std::string, QMetaObject * > metaObjects_;
	std::vector< std::unique_ptr< IQtTypeConverter > > qtTypeConverters_;
	std::map< ObjectHandle, QtScriptObject * > scriptObjects_;
};

#endif//QT_SCRIPTING_ENGINE_HPP