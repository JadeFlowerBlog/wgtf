//--------------------------------------------------------------------------------------------------
//
// maya_plugin.hpp
//
//--------------------------------------------------------------------------------------------------
//  Copyright (c) 2015 Wargaming World, Ltd. All rights reserved.
//--------------------------------------------------------------------------------------------------

#pragma warning( push )
#pragma warning( disable: 4244 4100 4238 4239 4263 4245 4201 )

#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>

#include <vector>

class NGTEventLoop;
class MayaWindow;

class NGTMayaPlugin : public MPxCommand {
public:
	NGTMayaPlugin();
	virtual ~NGTMayaPlugin();

	static void *creator();
	virtual MStatus doIt(const MArgList& args);
	bool getNGTPlugins(std::vector< std::wstring >& plugins, const wchar_t* filepath);

private:
	NGTEventLoop * ngtEventLoop_;
	MayaWindow * mayaWindow_;
};
