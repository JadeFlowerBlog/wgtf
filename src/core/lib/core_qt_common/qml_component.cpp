#include "qml_component.hpp"

#include <QQmlComponent>
#include <QQmlEngine>

QmlComponent::QmlComponent( QQmlEngine & qmlEngine )
	: qmlComponent_( new QQmlComponent( &qmlEngine ) )
{
}

QmlComponent::~QmlComponent()
{

}

QQmlComponent * QmlComponent::component() const
{
	return qmlComponent_.get();
}