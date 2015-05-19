#include "gl_view.hpp"
#include <assert.h>
#include "moo/render_context.hpp"
#include "common/cooperative_moo.hpp"
#include "appmgr/app.hpp"


GLView::GLView()
	: editorApp_( nullptr )
{
}

void GLView::setEditorApp( IEditorApp* editorApp )
{
	assert( editorApp_ != editorApp );
	editorApp_ = editorApp;
}

bool GLView::paint( bool resize )
{
	if (editorApp_->initDone() == false)
	{
		return false;
	}

	if (BW::CooperativeMoo::beginOnPaint( editorApp_ ) == false)
	{		
		return false;
	}

	if (resize)
	{
		resizeWindow();
	}

	editorApp_->mfApp()->updateFrame( false );

	BW::CooperativeMoo::endOnPaint( editorApp_ );
	return true;
}

//==============================================================================
void GLView::resizeWindow()
{
	BW::Moo::rc().changeMode(BW::Moo::rc().modeIndex(), BW::Moo::rc().windowed(), true);
}

