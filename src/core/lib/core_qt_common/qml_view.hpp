#ifndef QML_VIEW_HPP
#define QML_VIEW_HPP

#include "core_ui_framework/i_view.hpp"
#include "core_ui_framework/layout_hint.hpp"

#include <memory>
#include <string>
#include <QObject>
#include <QQuickWindow>

class QObject;
class QUrl;
class QQmlContext;
class QQmlEngine;
class QQuickWidget;
class QString;
class QVariant;
class IQtFramework;

class QmlView : public QObject, public IView
{
	Q_OBJECT
public:
	QmlView( const char * id, IQtFramework & qtFramework, QQmlEngine & qmlEngine );
	virtual ~QmlView();

	const char * id() const override;
	const char * title() const override;
	const char * windowId() const override;
	const LayoutHint& hint() const override;
	void update() override;

	QQuickWidget * releaseView();
	void retainView();
	QQuickWidget * view() const;

	void setContextObject( QObject * object );
	void setContextProperty( const QString & name, const QVariant & property );

	bool load( QUrl & qUrl );

	virtual void focusInEvent() override;
	virtual void focusOutEvent() override;

	virtual void registerListener( IViewEventListener* listener ) override;
	virtual void deregisterListener( IViewEventListener* listener ) override;

public slots:
	void error( QQuickWindow::SceneGraphError error, const QString &message );

private:
	IQtFramework & qtFramework_;
	std::unique_ptr< QQmlContext > qmlContext_;
	QQuickWidget * quickView_;
	std::string id_;
	std::string title_;
	std::string windowId_;
	LayoutHint hint_;

	bool released_;

	typedef std::vector<IViewEventListener*> Listeners;
	Listeners listeners_;
};

#endif//QML_VIEW_HPP