### Initialise compiler and linker flags ###

IF( BW_PLATFORM_WINDOWS )
	INCLUDE( BWCompiler_msvc )
ELSEIF( BW_PLATFORM_MAC )
	INCLUDE( BWCompiler_clang )
ELSE()
	# TODO: Add compiler and linker support for other compilers/platforms here
	# TODO: Split into per compiler files for the BW_* compiler and linker settings
	MESSAGE( FATAL " Unsupported compiler " )
ENDIF()

# Define a _${_CONFIG_NAME} preprocessor variable (e.g. _DEBUG)
SET_PROPERTY( DIRECTORY APPEND PROPERTY
	COMPILE_DEFINITIONS $<$<CONFIG:DEBUG>:"_DEBUG"> )
SET_PROPERTY( DIRECTORY APPEND PROPERTY
	COMPILE_DEFINITIONS $<$<CONFIG:HYBRID>:"_HYBRID"> )
SET_PROPERTY( DIRECTORY APPEND PROPERTY
	COMPILE_DEFINITIONS $<$<CONFIG:CONSUMER_RELEASE>:"_CONSUMER_RELEASE"> )

### Overide all CMake compile and link flags
### Don't modify these, modify the BW versions

FUNCTION( _BW_SET_LIST CACHENAME VALUE )
	# replace ';' separated list items with ' '
	STRING( REPLACE ";" " " TEMPVAR "${VALUE}" )
	SET( "${CACHENAME}" "${TEMPVAR}" ${ARGN} )
ENDFUNCTION()

_BW_SET_LIST( CMAKE_C_FLAGS "${BW_C_FLAGS}"
	CACHE STRING "Flags used by the compiler for all build types" FORCE )
_BW_SET_LIST( CMAKE_C_FLAGS_DEBUG "${BW_C_FLAGS_DEBUG}"
	CACHE STRING "Flags used by the compiler for all Debug builds" FORCE )
_BW_SET_LIST( CMAKE_C_FLAGS_HYBRID "${BW_C_FLAGS_HYBRID}"
	CACHE STRING "Flags used by the compiler for all Hybrid builds" FORCE )
_BW_SET_LIST( CMAKE_C_FLAGS_CONSUMER_RELEASE "${BW_C_FLAGS_CONSUMER_RELEASE}"
	CACHE STRING "Flags used by the compiler for all Consumer_Release builds" FORCE )

_BW_SET_LIST( CMAKE_CXX_FLAGS "${BW_CXX_FLAGS}"
	CACHE STRING "Flags used by the compiler for all build types" FORCE )
_BW_SET_LIST( CMAKE_CXX_FLAGS_DEBUG "${BW_CXX_FLAGS_DEBUG}"
	CACHE STRING "Flags used by the compiler for all Debug builds" FORCE )
_BW_SET_LIST( CMAKE_CXX_FLAGS_HYBRID "${BW_CXX_FLAGS_HYBRID}"
	CACHE STRING "Flags used by the compiler for all Hybrid builds" FORCE )
_BW_SET_LIST( CMAKE_CXX_FLAGS_CONSUMER_RELEASE "${BW_CXX_FLAGS_CONSUMER_RELEASE}"
	CACHE STRING "Flags used by the compiler for all Consumer_Release builds" FORCE )

_BW_SET_LIST( CMAKE_EXE_LINKER_FLAGS "${BW_EXE_LINKER_FLAGS}"
	CACHE STRING "Flags used by the linker" FORCE )
_BW_SET_LIST( CMAKE_EXE_LINKER_FLAGS_DEBUG "${BW_EXE_LINKER_FLAGS_DEBUG}"
	CACHE STRING "Flags used by the linker for Debug builds" FORCE )
_BW_SET_LIST( CMAKE_EXE_LINKER_FLAGS_HYBRID "${BW_EXE_LINKER_FLAGS_HYBRID}"
	CACHE STRING "Flags used by the linker for Hybrid builds" FORCE )
_BW_SET_LIST( CMAKE_EXE_LINKER_FLAGS_CONSUMER_RELEASE "${BW_EXE_LINKER_FLAGS_CONSUMER_RELEASE}"
	CACHE STRING "Flags used by the linker for Consumer_Release builds" FORCE )

_BW_SET_LIST( CMAKE_MODULE_LINKER_FLAGS "${BW_MODULE_LINKER_FLAGS}"
	CACHE STRING "Flags used by the linker for the creation of modules" FORCE )
_BW_SET_LIST( CMAKE_MODULE_LINKER_FLAGS_DEBUG "${BW_MODULE_LINKER_FLAGS_DEBUG}"
	CACHE STRING "Flags used by the linker for Debug builds" FORCE )
_BW_SET_LIST( CMAKE_MODULE_LINKER_FLAGS_HYBRID "${BW_MODULE_LINKER_FLAGS_HYBRID}"
	CACHE STRING "Flags used by the linker for Hybrid builds" FORCE )
_BW_SET_LIST( CMAKE_MODULE_LINKER_FLAGS_CONSUMER_RELEASE "${BW_MODULE_LINKER_FLAGS_CONSUMER_RELEASE}"
	CACHE STRING "Flags used by the linker for Consumer_Release builds" FORCE )

_BW_SET_LIST( CMAKE_SHARED_LINKER_FLAGS "${BW_SHARED_LINKER_FLAGS}"
	CACHE STRING "Flags used by the linker for the creation of dll's" FORCE )
_BW_SET_LIST( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${BW_SHARED_LINKER_FLAGS_DEBUG}"
	CACHE STRING "Flags used by the linker for Debug builds" FORCE )
_BW_SET_LIST( CMAKE_SHARED_LINKER_FLAGS_HYBRID "${BW_SHARED_LINKER_FLAGS_HYBRID}"
	CACHE STRING "Flags used by the linker for Hybrid builds" FORCE )
_BW_SET_LIST( CMAKE_SHARED_LINKER_FLAGS_CONSUMER_RELEASE "${BW_SHARED_LINKER_FLAGS_CONSUMER_RELEASE}"
	CACHE STRING "Flags used by the linker for Consumer_Release builds" FORCE )

_BW_SET_LIST( CMAKE_STATIC_LINKER_FLAGS "${BW_STATIC_LINKER_FLAGS}"
	CACHE STRING "Flags used by the linker for the creation of static libraries" FORCE )
_BW_SET_LIST( CMAKE_STATIC_LINKER_FLAGS_DEBUG "${BW_STATIC_LINKER_FLAGS_DEBUG}"
	CACHE STRING "Flags used by the linker for Debug builds" FORCE )
_BW_SET_LIST( CMAKE_STATIC_LINKER_FLAGS_HYBRID "${BW_STATIC_LINKER_FLAGS_HYBRID}"
	CACHE STRING "Flags used by the linker for Hybrid builds" FORCE )
_BW_SET_LIST( CMAKE_STATIC_LINKER_FLAGS_CONSUMER_RELEASE "${BW_STATIC_LINKER_FLAGS_CONSUMER_RELEASE}"
	CACHE STRING "Flags used by the linker for Consumer_Release builds" FORCE )

_BW_SET_LIST( CMAKE_RC_FLAGS "${BW_RC_FLAGS}"
	CACHE STRING "Flags used by the resource compiler" FORCE )

IF( CMAKE_VERBOSE )
	MESSAGE( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}" )
	MESSAGE( STATUS "CMAKE_C_FLAGS_HYBRID: ${CMAKE_C_FLAGS_HYBRID}" )
	MESSAGE( STATUS "CMAKE_C_FLAGS_CONSUMER_RELEASE: ${CMAKE_C_FLAGS_CONSUMER_RELEASE}" )
	MESSAGE( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}" )
	MESSAGE( STATUS "CMAKE_CXX_FLAGS_HYBRID: ${CMAKE_CXX_FLAGS_HYBRID}" )
	MESSAGE( STATUS "CMAKE_CXX_FLAGS_CONSUMER_RELEASE: ${CMAKE_CXX_FLAGS_CONSUMER_RELEASE}" )
	MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS_DEBUG: ${CMAKE_EXE_LINKER_FLAGS_DEBUG}" )
	MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS_HYBRID: ${CMAKE_EXE_LINKER_FLAGS_HYBRID}" )
	MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS_CONSUMER_RELEASE: ${CMAKE_EXE_LINKER_FLAGS_CONSUMER_RELEASE}" )
	MESSAGE( STATUS "CMAKE_MODULE_LINKER_FLAGS: ${CMAKE_MODULE_LINKER_FLAGS}" )
	MESSAGE( STATUS "CMAKE_MODULE_LINKER_FLAGS_DEBUG: ${CMAKE_MODULE_LINKER_FLAGS_DEBUG}" )
	MESSAGE( STATUS "CMAKE_MODULE_LINKER_FLAGS_HYBRID: ${CMAKE_MODULE_LINKER_FLAGS_HYBRID}" )
	MESSAGE( STATUS "CMAKE_MODULE_LINKER_FLAGS_CONSUMER_RELEASE: ${CMAKE_MODULE_LINKER_FLAGS_CONSUMER_RELEASE}" )
	MESSAGE( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	MESSAGE( STATUS "CMAKE_SHARED_LINKER_FLAGS_DEBUG: ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" )
	MESSAGE( STATUS "CMAKE_SHARED_LINKER_FLAGS_HYBRID: ${CMAKE_SHARED_LINKER_FLAGS_HYBRID}" )
	MESSAGE( STATUS "CMAKE_SHARED_LINKER_FLAGS_CONSUMER_RELEASE: ${CMAKE_SHARED_LINKER_FLAGS_CONSUMER_RELEASE}" )
	MESSAGE( STATUS "CMAKE_STATIC_LINKER_FLAGS: ${CMAKE_STATIC_LINKER_FLAGS}" )
	MESSAGE( STATUS "CMAKE_STATIC_LINKER_FLAGS_DEBUG: ${CMAKE_STATIC_LINKER_FLAGS_DEBUG}" )
	MESSAGE( STATUS "CMAKE_STATIC_LINKER_FLAGS_HYBRID: ${CMAKE_STATIC_LINKER_FLAGS_HYBRID}" )
	MESSAGE( STATUS "CMAKE_STATIC_LINKER_FLAGS_CONSUMER_RELEASE: ${CMAKE_STATIC_LINKER_FLAGS_CONSUMER_RELEASE}" )
	MESSAGE( STATUS "CMAKE_RC_FLAGS: ${CMAKE_RC_FLAGS}" )
ENDIF()

MARK_AS_ADVANCED(
	CMAKE_C_FLAGS_DEBUG
	CMAKE_C_FLAGS_HYBRID
	CMAKE_C_FLAGS_CONSUMER_RELEASE
	CMAKE_CXX_FLAGS_DEBUG
	CMAKE_CXX_FLAGS_HYBRID
	CMAKE_CXX_FLAGS_CONSUMER_RELEASE
	CMAKE_EXE_LINKER_FLAGS_DEBUG
	CMAKE_EXE_LINKER_FLAGS_HYBRID
	CMAKE_EXE_LINKER_FLAGS_CONSUMER_RELEASE
	CMAKE_MODULE_LINKER_FLAGS_DEBUG
	CMAKE_MODULE_LINKER_FLAGS_HYBRID
	CMAKE_MODULE_LINKER_FLAGS_CONSUMER_RELEASE
	CMAKE_SHARED_LINKER_FLAGS_DEBUG
	CMAKE_SHARED_LINKER_FLAGS_HYBRID
	CMAKE_SHARED_LINKER_FLAGS_CONSUMER_RELEASE
	CMAKE_STATIC_LINKER_FLAGS_DEBUG
	CMAKE_STATIC_LINKER_FLAGS_HYBRID
	CMAKE_STATIC_LINKER_FLAGS_CONSUMER_RELEASE
	CMAKE_RC_FLAGS
)
