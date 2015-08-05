INCLUDE_DIRECTORIES( ${WG_TOOLS_SOURCE_DIR} )

# Adds Consumer_Release build config
SET(BW_BUILD_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})

FUNCTION( BW_ADD_CONSUMER_RELEASE_CONFIG )
	IF( CMAKE_CONFIGURATION_TYPES )
		LIST( APPEND CMAKE_CONFIGURATION_TYPES Consumer_Release )
		LIST( REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES )
		SET( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
			CACHE STRING "Semicolon separated list of supported configuration types"
			FORCE )
	ENDIF()

	SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
		"Choose the type of build."
		FORCE )
ENDFUNCTION()


FUNCTION( BW_ADD_COMPILE_FLAGS )
	SET( _FLAGS "" )
	FOREACH( _FLAG IN LISTS ARGN )
		SET( _FLAGS "${_FLAGS} ${_FLAG}" )
	ENDFOREACH()
	SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS}" PARENT_SCOPE )
	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}" PARENT_SCOPE )
ENDFUNCTION()


FUNCTION( BW_REMOVE_COMPILE_FLAGS )
	SET( _CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" )
	SET( _CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	FOREACH( _FLAG IN LISTS ARGN )
		STRING( REGEX REPLACE "${_FLAG}" "" _CMAKE_C_FLAGS "${_CMAKE_C_FLAGS}"  )
		STRING( REGEX REPLACE "${_FLAG}" "" _CMAKE_CXX_FLAGS "${_CMAKE_CXX_FLAGS}" )
	ENDFOREACH()
	SET( CMAKE_C_FLAGS "${_CMAKE_C_FLAGS}" PARENT_SCOPE )
	SET( CMAKE_CXX_FLAGS "${_CMAKE_CXX_FLAGS}" PARENT_SCOPE )
ENDFUNCTION()


# Standard BW precompiled headers
FUNCTION( BW_PRECOMPILED_HEADER _PROJNAME _PCHNAME )
	IF( MSVC AND NOT CLANG_CL AND NOT BW_IS_SERVER )
		GET_FILENAME_COMPONENT(_BASENAME ${_PCHNAME} NAME_WE)
		SET( _PCHSRC ${_BASENAME}.cpp )
		SET( _PCHBIN "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${_PROJNAME}.pch" )

		GET_TARGET_PROPERTY( _SRCS ${_PROJNAME} SOURCES )
		SET( _HAVE_PCHCPP FALSE )
		FOREACH( _SRC ${_SRCS} )
			IF( _SRC MATCHES \\.\(cc|cxx|cpp\)$ )
				GET_SOURCE_FILE_PROPERTY( _HEADERTYPE ${_SRC} HEADER_FILE_ONLY )
				GET_SOURCE_FILE_PROPERTY( _GENERATED ${_SRC} GENERATED )
				GET_FILENAME_COMPONENT( _SRCBASE ${_SRC} NAME_WE )
				IF( NOT ${_HEADERTYPE} AND NOT ${_GENERATED} )
					IF( _SRCBASE STREQUAL ${_BASENAME} )
						SET( _PCHFLAGS "/Yc\"${_PCHNAME}\" /Fp\"${_PCHBIN}\"" )
						SET_SOURCE_FILES_PROPERTIES( ${_SRC}
							PROPERTIES COMPILE_FLAGS "${_PCHFLAGS}"
							OBJECT_OUTPUTS "${_PCHBIN}" )
						SET( _HAVE_PCHCPP TRUE )
					ELSE()
						SET( _PCHFLAGS "/Yu\"${_PCHNAME}\" /Fp\"${_PCHBIN}\"" )
						SET_SOURCE_FILES_PROPERTIES( ${_SRC}
							PROPERTIES COMPILE_FLAGS "${_PCHFLAGS}"
							OBJECT_DEPENDS "${_PCHBIN}")
					ENDIF()
				ELSEIF( _SRCBASE STREQUAL ${_BASENAME} )
					SET( _HAVE_PCHCPP TRUE )
				ENDIF()
			ENDIF()
		ENDFOREACH()
		IF(NOT _HAVE_PCHCPP)
			MESSAGE(FATAL_ERROR "${_BASENAME}.cpp was not found.")
		ENDIF()
	ENDIF()
ENDFUNCTION()


# Sets the default blobbing size
SET( BW_IDEAL_FILES_PER_BLOB 42 )

# Combine source file inputs into _RETURNVAR.
# If BW_BLOB_CONFIG is enabled then the source files will create a blob file.
FUNCTION( BW_BLOB_SOURCES _RETURNVAR )
	SET( _SOURCES "" )
	SET( _CPPSRCS "" )

	# add all inputs to the output sources list
	FOREACH( _SRC IN LISTS ARGN )
		LIST( APPEND _SOURCES ${_SRC} )
		# add all c++ sources to a list
		IF( _SRC MATCHES \\.\(cc|cxx|cpp\)$ )
			LIST( APPEND _CPPSRCS ${_SRC} )
		ENDIF()
	ENDFOREACH()

	IF( BW_BLOB_CONFIG )
		SET( _FILESPERBLOB ${BW_IDEAL_FILES_PER_BLOB} )
		LIST( LENGTH _CPPSRCS _NUMSOURCES )
		IF( ${_FILESPERBLOB} GREATER ${_NUMSOURCES} )
			SET( _FILESPERBLOB ${_NUMSOURCES} )
		ELSE()
			MATH( EXPR _NUMBLOBS "${_NUMSOURCES} / ${_FILESPERBLOB}" )
			MATH( EXPR _REMAINDER "${_NUMSOURCES} - (${_NUMBLOBS} * ${_FILESPERBLOB})" )
			IF( ${_NUMBLOBS} GREATER "0" AND ${_REMAINDER} GREATER "0" )
				# Evenly distribute remaining files across blobs
				MATH( EXPR _FILESPERBLOB "(${_NUMSOURCES} / (1 + ${_NUMBLOBS}))" )
			ENDIF()
		ENDIF()
		SET( _SRCINDEX 0 )
		SET( _BLOBINDEX 1 )
		STRING( TOLOWER ${_RETURNVAR} _RETURNVARLOWER )
		# create initial blob file name
		SET( _BLOBNAME "${CMAKE_CURRENT_BINARY_DIR}/blob/${PROJECT_NAME}_${_RETURNVARLOWER}_${_BLOBINDEX}.cpp" )
		SET( _TOBLOBLIST "" )
		SET( _BLOBFILES "" )
		SET( _BLOBHEADER "" )
		# iterate through c++s adding them to a blob file
		FOREACH( _SRC ${_CPPSRCS} )
			LIST( APPEND _TOBLOBLIST ${_SRC} )
			LIST( LENGTH _TOBLOBLIST _BLOBSRCCOUNT )
			IF( ${_BLOBSRCCOUNT} EQUAL ${_FILESPERBLOB} )
				_BW_BLOB_SOURCES_INTERNAL( ${_BLOBNAME} "${_BLOBHEADER}" ${_TOBLOBLIST} )
				# add to list of generated files
				LIST( APPEND _BLOBFILES ${_BLOBNAME} )
				# generate next blob file name
				MATH( EXPR _BLOBINDEX "${_BLOBINDEX} + 1" )
				SET( _BLOBNAME "${CMAKE_CURRENT_BINARY_DIR}/blob/${PROJECT_NAME}_${_RETURNVARLOWER}_${_BLOBINDEX}.cpp" )
				# empty current list
				SET( _TOBLOBLIST "" )
			ENDIF()
			MATH( EXPR _SRCINDEX "${_SRCINDEX} + 1" )
		ENDFOREACH()
		# write out any remaining files to blob
		IF( _TOBLOBLIST )
			_BW_BLOB_SOURCES_INTERNAL( ${_BLOBNAME} "${_BLOBHEADER}" ${_TOBLOBLIST} )
			LIST( APPEND _BLOBFILES ${_BLOBNAME} )
		ENDIF()
		LIST( LENGTH _BLOBFILES _NUMBLOBS )
		MESSAGE( STATUS "${PROJECT_NAME} ${_NUMSOURCES} files ${_NUMBLOBS} blobs ${_FILESPERBLOB} files per blob" )
		# append blob files to output sources
		LIST( APPEND _SOURCES ${_BLOBFILES} )
	ENDIF()
	# set output
	SET( ${_RETURNVAR} ${_SOURCES} PARENT_SCOPE )
ENDFUNCTION()


# Internal function for writing blob file
FUNCTION( _BW_BLOB_SOURCES_INTERNAL _BLOBNAME _BLOBHEADER )
	SET( _BLOBCONTENT "${_BLOBHEADER}" )
	FOREACH( _SRC IN LISTS ARGN )
		GET_FILENAME_COMPONENT( _ABSOLUTEPATH "${_SRC}" ABSOLUTE )
		SET_SOURCE_FILES_PROPERTIES( "${_ABSOLUTEPATH}" PROPERTIES HEADER_FILE_ONLY TRUE )
		SET( _BLOBCONTENT "${_BLOBCONTENT}\n#pragma message(\"inclusion of file ${_SRC}\")")
		SET( _BLOBCONTENT "${_BLOBCONTENT}\n#include \"${CMAKE_CURRENT_SOURCE_DIR}/${_SRC}\"")
		# undef and redefine PY_ATTR_SCOPE as cpp's leave it in an undetermined state
		SET( _BLOBCONTENT "${_BLOBCONTENT}\n#undef PY_ATTR_SCOPE\n#define PY_ATTR_SCOPE")
	ENDFOREACH()
	# Only touch the file if the content has changed
	IF( EXISTS ${_BLOBNAME} )
		FILE( READ ${_BLOBNAME} _OLDBLOBCONTENT )
	ENDIF()
	IF( NOT "${_BLOBCONTENT}" STREQUAL "${_OLDBLOBCONTENT}" )
		# TODO: Change write to GENERATE when CMake is upgraded to >= 2.8.12
		FILE( WRITE ${_BLOBNAME} ${_BLOBCONTENT} )
	ENDIF()
	SOURCE_GROUP( "Blob" FILES ${_BLOBNAME} )
	# Set generated so we don't try and use precompiled headers with this file
	SET_SOURCE_FILES_PROPERTIES( ${_BLOBNAME} PROPERTIES GENERATED TRUE )
ENDFUNCTION()


# For appending properties because SET_TARGET_PROPERTIES overwrites
FUNCTION( BW_APPEND_TARGET_PROPERTIES _PROJNAME _PROPNAME _PROPS_TO_APPEND )
	SET( NEW_PROPS ${_PROPS_TO_APPEND} )
	GET_TARGET_PROPERTY( EXISTING_PROPS ${_PROJNAME} ${_PROPNAME} )
	IF( EXISTING_PROPS )
		SET( NEW_PROPS "${NEW_PROPS} ${EXISTING_PROPS}" )
	ENDIF()
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES ${_PROPNAME} ${NEW_PROPS} )
ENDFUNCTION()

# For appending to LINK_FLAGS because SET_TARGET_PROPERTIES overwrites it
FUNCTION( BW_APPEND_LINK_FLAGS _PROJNAME _FLAGS_TO_APPEND )
	BW_APPEND_TARGET_PROPERTIES( ${_PROJNAME} LINK_FLAGS ${_FLAGS_TO_APPEND} )
ENDFUNCTION()

# Add Microsoft Windows Common-Controls as a dependency on the target
MACRO( BW_USE_MICROSOFT_COMMON_CONTROLS _PROJNAME )
	BW_APPEND_LINK_FLAGS( ${_PROJNAME}
		"/MANIFESTDEPENDENCY:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' publicKeyToken='6595b64144ccf1df' language='*' processorArchitecture='*'\""
	)
ENDMACRO( BW_USE_MICROSOFT_COMMON_CONTROLS )


# Assigns the given target to a Solution Folder
# http://athile.net/library/blog/?p=288
MACRO( BW_PROJECT_CATEGORY _PROJNAME _CATEGORY )
	SET_PROPERTY( TARGET ${_PROJNAME} PROPERTY FOLDER ${_CATEGORY} )
ENDMACRO( BW_PROJECT_CATEGORY )


# Links the given executable target name to all BW_LIBRARY_PROJECTS
MACRO( BW_LINK_LIBRARY_PROJECTS _PROJNAME )
	MESSAGE( WARNING
		 "BW_LINK_LIBRARY_PROJECTS is deprecated. Use BW_LINK_TARGET_LIBRARIES instead." )
	ARRAY2D_BEGIN_LOOP( _islooping "${BW_LIBRARY_PROJECTS}" 2 "libname;libpath" )
		WHILE( _islooping )
			TARGET_LINK_LIBRARIES( ${_PROJNAME} ${libname} )
			ARRAY2D_ADVANCE()
		ENDWHILE()
	ARRAY2D_END_LOOP()
ENDMACRO( BW_LINK_LIBRARY_PROJECTS )

# Set the output directory for the given executable target name to
# the given location.
FUNCTION( BW_SET_BINARY_DIR _PROJNAME _DIRNAME )
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${_DIRNAME}"
		PDB_OUTPUT_DIRECTORY_DEBUG "${_DIRNAME}"
		)
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_HYBRID "${_DIRNAME}"
		PDB_OUTPUT_DIRECTORY_HYBRID "${_DIRNAME}"
		)
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_CONSUMER_RELEASE "${_DIRNAME}"
		PDB_OUTPUT_DIRECTORY_CONSUMER_RELEASE "${_DIRNAME}"
		)	
ENDFUNCTION( BW_SET_BINARY_DIR )


# Marks the executable given as a Unit test
MACRO( BW_ADD_TEST _PROJNAME )
	ADD_TEST( NAME ${_PROJNAME}
		COMMAND $<TARGET_FILE:${_PROJNAME}>)

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		DEBUG_OUTPUT_NAME
		"${_PROJNAME}_d" )

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		HYBRID_OUTPUT_NAME
		"${_PROJNAME}_h" )

	BW_SET_BINARY_DIR( ${_PROJNAME} "${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}" )

ENDMACRO( BW_ADD_TEST )

MACRO( BW_ADD_TOOL_TEST _PROJNAME )
	BW_ADD_TEST( ${_PROJNAME} )
ENDMACRO( BW_ADD_TOOL_TEST )

MACRO( BW_TARGET_LINK_LIBRARIES )
	TARGET_LINK_LIBRARIES( ${ARGN} )
ENDMACRO( BW_TARGET_LINK_LIBRARIES )

# Add a library
MACRO( BW_ADD_LIBRARY _PROJNAME )
	ADD_LIBRARY( ${_PROJNAME} ${ARGN} )
ENDMACRO( BW_ADD_LIBRARY )

# Add an executable
MACRO( BW_ADD_EXECUTABLE _PROJNAME )
	IF (BW_PLATFORM_MAC)
		IF(NOT MACOSX_BUNDLE_ICON_FILE)
			SET( MACOSX_BUNDLE_ICON_FILE application.icns )
			SET( _ICON ${BW_BUILD_CMAKE_DIR}/xcode/application.icns )
			SET_SOURCE_FILES_PROPERTIES(${_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
		ENDIF()
		ADD_EXECUTABLE( ${_PROJNAME} MACOSX_BUNDLE ${ARGN} ${_ICON} )
	ELSE (BW_PLATFORM_WINDOWS)
		ADD_EXECUTABLE( ${_PROJNAME} WIN32 ${ARGN} )
	ENDIF()

ENDMACRO( BW_ADD_EXECUTABLE )

# Helper macro for adding a tool executable
MACRO( BW_ADD_TOOL_EXE _PROJNAME _DIRNAME )
	BW_ADD_EXECUTABLE( ${_PROJNAME} ${ARGN} )

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		DEBUG_OUTPUT_NAME
		"${_PROJNAME}_d" )
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		HYBRID_OUTPUT_NAME
		"${_PROJNAME}" )

	BW_SET_BINARY_DIR( ${_PROJNAME} "${BW_GAME_DIR}/${_DIRNAME}/${BW_PLATFORM}" )
	IF( BW_PYTHON_DLL_SUPPORT )
		BW_COPY_TARGET( ${_PROJNAME} libpython-shared )
	ENDIF()

	IF( CMAKE_MFC_FLAG EQUAL 2 )
		IF( NOT BW_NO_UNICODE )
			# Force entry point for MFC
			BW_APPEND_LINK_FLAGS( ${_PROJNAME} "/entry:wWinMainCRTStartup" )
		ENDIF()
		BW_USE_MICROSOFT_COMMON_CONTROLS( ${_PROJNAME} )
	ENDIF()

	BW_PROJECT_CATEGORY( ${_PROJNAME} "Executables" )
ENDMACRO()

# Helper macro for adding a tool plugin
MACRO( BW_ADD_TOOL_PLUGIN _PROJNAME _DIRNAME )
	ADD_LIBRARY( ${_PROJNAME} ${ARGN} )

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		DEBUG_OUTPUT_NAME
		"${_PROJNAME}_d" )
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		HYBRID_OUTPUT_NAME
		"${_PROJNAME}" )

	BW_SET_BINARY_DIR( ${_PROJNAME} "${BW_GAME_DIR}/${_DIRNAME}/${BW_PLATFORM}/plugins" )

	BW_PROJECT_CATEGORY( ${_PROJNAME} "Plugins" )
ENDMACRO()

# Helper macro for adding a tool plugin
MACRO( BW_ADD_TOOL_TEST_PLUGIN _PROJNAME )
	ADD_LIBRARY( ${_PROJNAME} ${ARGN} )

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		DEBUG_OUTPUT_NAME
		"${_PROJNAME}_d" )
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		HYBRID_OUTPUT_NAME
		"${_PROJNAME}" )

	BW_SET_BINARY_DIR( ${_PROJNAME} "${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}/plugins" )

	BW_PROJECT_CATEGORY( ${_PROJNAME} "Unit Tests/Plugins" )
ENDMACRO()

# Helper macro for adding asset pipeline exes and dlls
MACRO( BW_ADD_ASSETPIPELINE_EXE _PROJNAME )
	BW_ADD_TOOL_EXE( ${_PROJNAME} asset_pipeline ${ARGN} )
ENDMACRO()

MACRO( BW_ADD_ASSETPIPELINE_DLL _PROJNAME )
	BW_ADD_LIBRARY( ${_PROJNAME} SHARED ${ARGN} )

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		DEBUG_OUTPUT_NAME
		"${_PROJNAME}_d" )
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		HYBRID_OUTPUT_NAME
		"${_PROJNAME}" )

	BW_SET_BINARY_DIR( ${_PROJNAME} "${BW_GAME_DIR}/asset_pipeline/${BW_PLATFORM}" )
	BW_PROJECT_CATEGORY( ${_PROJNAME} "Asset Pipeline/Converters" )
ENDMACRO()

MACRO( BW_ADD_UNITTEST_ASSETPIPELINE_DLL _PROJNAME )
	BW_ADD_LIBRARY( ${_PROJNAME} SHARED ${ARGN} )

	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		DEBUG_OUTPUT_NAME
		"${_PROJNAME}_d" )
	SET_TARGET_PROPERTIES( ${_PROJNAME} PROPERTIES
		HYBRID_OUTPUT_NAME
		"${_PROJNAME}" )

	BW_SET_BINARY_DIR( ${_PROJNAME} "${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}" )
	BW_PROJECT_CATEGORY( ${_PROJNAME} "Unit Tests/Asset Pipeline" )
ENDMACRO()

MACRO( BW_CUSTOM_COMMAND _PROJNAME _CMD )
	ADD_CUSTOM_TARGET( ${_PROJNAME}
		COMMAND ${_CMD} ${ARGN}
	)
	BW_PROJECT_CATEGORY( ${_PROJNAME} "Build Commands" )
ENDMACRO( BW_CUSTOM_COMMAND )

# Copy output from one target to output dir of another target
# **Important Note**
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#command:add_custom_command
# "Do not list the output in more than one independent target that may build in
# parallel or the two instances of the rule may conflict
# (instead use add_custom_target to drive the command and make the other
# targets depend on that one)."
FUNCTION( BW_COPY_TARGET _TO_TARGET _FROM_TARGET )
	ADD_CUSTOM_COMMAND(
		TARGET ${_TO_TARGET}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:${_FROM_TARGET}>
			$<TARGET_FILE_DIR:${_TO_TARGET}>/$<TARGET_FILE_NAME:${_FROM_TARGET}>
		COMMENT "Copying from ${_FROM_TARGET} to ${_TO_TARGET}'s output dir." )
ENDFUNCTION()

# Copy binary from output directory to the unit test output directory
# **Important Note**
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#command:add_custom_command
# "Do not list the output in more than one independent target that may build in
# parallel or the two instances of the rule may conflict
# (instead use add_custom_target to drive the command and make the other
# targets depend on that one)."
FUNCTION( BW_COPY_TARGET_TO_UNIT_TEST_BIN _FROM_TARGET )
	ADD_CUSTOM_COMMAND(
		TARGET ${_FROM_TARGET}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:${_FROM_TARGET}>
			${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}/$<TARGET_FILE_NAME:${_FROM_TARGET}>
		COMMENT "Copying from ${_FROM_TARGET} to ${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}."
		VERBATIM
	)
ENDFUNCTION()

# Copy plugin from output directory to the unit test plugins directory
# **Important Note**
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#command:add_custom_command
# "Do not list the output in more than one independent target that may build in
# parallel or the two instances of the rule may conflict
# (instead use add_custom_target to drive the command and make the other
# targets depend on that one)."
FUNCTION( BW_COPY_PLUGIN_TO_UNIT_TEST_BIN _FROM_TARGET )
	ADD_CUSTOM_COMMAND(
		TARGET ${_FROM_TARGET}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:${_FROM_TARGET}>
			${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}/plugins/$<TARGET_FILE_NAME:${_FROM_TARGET}>
		COMMENT "Copying from ${_FROM_TARGET} to ${BW_GAME_DIR}/unit_tests/${BW_PLATFORM}/plugins."
		VERBATIM
	)
ENDFUNCTION()

FUNCTION( BW_SET_OPTIONAL_FILES _RETURN_VAR )
	FOREACH( _FILENAME ${ARGN} )
		IF( EXISTS ${_FILENAME} )
			LIST( APPEND _DETECTED_FILES ${_FILENAME} )
		ENDIF()
	ENDFOREACH()
	SET( ${_RETURN_VAR} ${_DETECTED_FILES} PARENT_SCOPE )
ENDFUNCTION()

#-------------------------------------------------------------------

#
# 2D array helpers: http://www.cmake.org/pipermail/cmake/2011-November/047829.html
#-------------------------------------------------------------------

macro( array2d_get_item out_value offset )
	math( EXPR _finalindex "${_array2d_index}+${offset}" )
	list( GET _array2d_array ${_finalindex} _item )
	set( ${out_value} "${_item}" )
endmacro()

#-------------------------------------------------------------------

macro( array2d_begin_loop out_advanced array width var_names )
	set( _array2d_out_advanced ${out_advanced} )
	set( _array2d_index 0 )
	set( _array2d_array ${array} )
	set( _array2d_width ${width} )
	set( _array2d_var_names ${var_names} )
	array2d_advance()
endmacro()

#-------------------------------------------------------------------

macro( array2d_advance )
	if( NOT _array2d_array )
		set( ${_array2d_out_advanced} false )
	else()
		list( LENGTH _array2d_array _size )
		math( EXPR _remaining "${_size}-${_array2d_index}" )

		if( (_array2d_width LESS 1) OR (_size LESS _array2d_width) OR (_remaining LESS _array2d_width) )
			set( ${_array2d_out_advanced} false )
		else()
			math( EXPR _adjusted_width "${_array2d_width}-1" )
			foreach( offset RANGE ${_adjusted_width} )
				list( GET _array2d_var_names ${offset} _var_name )
				array2d_get_item( ${_var_name} ${offset} )
			endforeach()

			math( EXPR _index "${_array2d_index}+${_array2d_width}" )
			set( _array2d_index ${_index} )
			set( ${_array2d_out_advanced} true )
		endif()
	endif()
endmacro()

#-------------------------------------------------------------------

macro( array2d_end_loop )
endmacro()
#-------------------------------------------------------------------

# Old way of iterating through 2D list, kept for reference.
#LIST( LENGTH BW_LIBRARY_PROJECTS _itercount )
#MATH( EXPR _itercount "(${_itercount} / 2)-1" )
#FOREACH( i RANGE( ${_itercount} ) )
#	MATH( EXPR i1 "${i}*2" )
#	MATH( EXPR i2 "${i}*2+1" )
#	LIST( GET BW_LIBRARY_PROJECTS ${i1} libname )
#	LIST( GET BW_LIBRARY_PROJECTS ${i2} libpath )

#	MESSAGE( STATUS "Adding library: ${libname} from ${libpath}" )
#	ADD_SUBDIRECTORY( ${libpath} )
#ENDFOREACH()


MACRO( BW_DEPLOY_RESOURCES _TARGET_DIR _RESOURCES )
    FOREACH( resFile ${_RESOURCES} )
        ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resFile}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${_TARGET_DIR}
            VERBATIM 
	    )
    ENDFOREACH()
ENDMACRO()

# Add a target to generate API documentation with Doxygen
SET( DOXYGEN_EXECUTABLE "${BW_SOURCE_DIR}/core/third_party/doxygen/bin/doxygen.exe" )
SET( DOXYQML_EXECUTABLE "${BW_SOURCE_DIR}/core/third_party/doxyqml/bin/doxyqml.bat" )
SET( GRAPHVIZ_DOT_PATH "${BW_SOURCE_DIR}/core/third_party/Graphviz2.38/bin/dot.exe" )

FUNCTION( BW_GENERATE_DOC _target _Doxyfile _OutputDir )
    IF( BW_PLATFORM_WINDOWS )
        CONFIGURE_FILE( ${_Doxyfile} ${_OutputDir}/Doxyfile @ONLY )
        ADD_CUSTOM_TARGET( ${_target}_DOC
            ${DOXYGEN_EXECUTABLE} ${_OutputDir}/Doxyfile
            WORKING_DIRECTORY ${_OutputDir}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    ENDIF()
ENDFUNCTION()
