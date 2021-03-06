CMAKE_MINIMUM_REQUIRED( VERSION 3.1.1 )
PROJECT( plg_color_picker )

INCLUDE( WGToolsCoreProject )

FIND_PACKAGE( QT5 )

SET( METADATA_SRCS
	metadata/color_picker.mpp
)
SOURCE_GROUP( "MetaData" FILES ${METADATA_SRCS} )


SET( ALL_SRCS
	color_picker.hpp
	color_picker.cpp
	color_picker_context.hpp
	color_picker_context.cpp
	plg_color_picker_main.cpp
)
SOURCE_GROUP( "" FILES ${ALL_SRCS} )

BW_BLOB_SOURCES( BLOB_SRCS
	${ALL_SRCS}
)

BW_QT_ADD_QRC( WGColorPicker resources/WGColorPicker )
BW_GLOB_RESOURCE_FILES()

BW_ADD_TOOL_PLUGIN( ${PROJECT_NAME} SHARED
	${BLOB_SRCS}
	${RESOURCE_FILES}
	${COMPILED_RESOURCES}
)

BW_TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PRIVATE
	core_generic_plugin
	core_qt_common
	core_ui_framework
	Qt5::Core
	Qt5::Gui
	Qt5::QuickWidgets
)

BW_PROJECT_CATEGORY( ${PROJECT_NAME} "Plugins" )
