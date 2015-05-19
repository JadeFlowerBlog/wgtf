#include "bw_filedialog_qi.hpp"

#include "../helpers/qt_helpers.hpp"

#include <cassert>
#include <QtCore>
#include <QQmlEngine>

//==============================================================================
BWFileDialogQI::BWFileDialogQI( QObject * parent )
	: QObject( parent )
	, fileDialog_( NULL )
{
}

//==============================================================================
BWFileDialogQI::~BWFileDialogQI()
{
	delete fileDialog_;
	fileDialog_ = NULL;
}

//==============================================================================
void BWFileDialogQI::componentOnComplete()
{
	// We should have all properties we need by this point, create a new file dialog
	createFileDialog();
}

//==============================================================================
void BWFileDialogQI::setTitle( QString title )
{
	// Set the file dialog's title to be used later when the createFileDialog
	// function is invoked
	if ( title_ == title )
	{
		return;
	}
	title_ = title;
}

//==============================================================================
void BWFileDialogQI::setFilter( QString filter )
{
	// Set the file dialog's filter to be used later when the createFileDialog
	// function is invoked
	if ( filter_ == filter )
	{
		return;
	}
	filter_ = filter;
}

//==============================================================================
void BWFileDialogQI::setFileModeFlag( int fileModeFlag )
{
	// Set the file dialog's file mode flag to be used later when the createFileDialog
	// function is invoked
	if ( fileModeFlag_ == static_cast< QFileDialog::FileMode >( fileModeFlag  ) )
	{
		return;
	}
	fileModeFlag_ = fileModeFlag;
}

//==============================================================================
void BWFileDialogQI::setMode( int mode )
{
	// Set the file dialog's accept mode to be used later when the createFileDialog
	// function is invoked
	if ( mode_ == static_cast< QFileDialog::AcceptMode >( mode  ) )
	{
		return;
	}
	mode_ = mode;
}

//==============================================================================
bool BWFileDialogQI::showDialog()
{
	// Show the file dialog and let qml know if there is any file name selected
	bool success = fileDialog_->exec();
	if ( success )
	{
		// Cache the file name selection
		fileNames_ = fileDialog_->selectedFiles();

		emit fileNamesChanged();
	}
	return success;
}

//==============================================================================
QStringList BWFileDialogQI::getFileNames()
{
	// Returns the cached file names.
	// NOTE: Must call the showDialog prior to this.
	return fileNames_;
}

//==============================================================================
void BWFileDialogQI::createFileDialog()
{
	// Make sure we don't create the QFileDialog instance multiple times
	assert( NULL == fileDialog_ );

	// Create a new QFileDialog and set it up using properties from qml
	fileDialog_ = new QFileDialog();

	fileDialog_->setWindowTitle( title_ );
	fileDialog_->setNameFilter( filter_ );
	fileDialog_->setAcceptMode( static_cast< QFileDialog::AcceptMode >( mode_ ) );
	fileDialog_->setFileMode( static_cast< QFileDialog::FileMode >( fileModeFlag_ ) );
}

