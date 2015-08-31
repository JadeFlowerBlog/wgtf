#ifndef QML_WINDOW_HPP
#define QML_WINDOW_HPP

#include "core_ui_framework/i_window.hpp"
#include "core_ui_framework/layout_hint.hpp"
#include <map>
#include <memory>
#include <QObject>
#include <QQuickWindow>

class IQtFramework;
class QUrl;
class QQmlContext;
class QQmlEngine;
class QQuickWidget;
class QString;
class QVariant;
class QWindow;

class QmlWindow : public QObject, public IWindow
{
public:
	QmlWindow( IQtFramework & qtFramework, QQmlEngine & qmlEngine );
	virtual ~QmlWindow();

	const char * id() const override;
	const char * title() const override;
	void update() override;
	void close() override;

	void show() override;
	void showModal() override;
	void hide() override;

	const Menus & menus() const override;
	const Regions & regions() const override;

	virtual void * nativeWindowId() const override;
	virtual void makeFramelessWindow() override;

	void setContextObject( QObject * object );
	void setContextProperty( const QString & name, const QVariant & property );

	QQuickWidget * release();
	QQuickWidget * window() const;
	bool load( QUrl & qUrl );

	bool eventFilter( QObject * object, QEvent * event );

	public Q_SLOTS:
		void error( QQuickWindow::SceneGraphError error, const QString &message );

private:
	IQtFramework & qtFramework_;
	std::unique_ptr< QQmlContext > qmlContext_;
	QQuickWidget* mainWindow_;
	std::string id_;
	std::string title_;
	Menus menus_;
	Regions regions_;
	bool released_;
	Qt::WindowModality modalityFlag_;
};

#endif//QML_WINDOW_HPP