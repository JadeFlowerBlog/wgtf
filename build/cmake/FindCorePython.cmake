# Use FIND_PACKAGE( CorePython ) to run this script.
INCLUDE( FindPackageHandleStandardArgs )

IF( NOT CORE_PYTHON_SUPPORT )
	# Package disabled
	SET( CORE_PYTHON_FOUND FALSE )
	RETURN()
ENDIF()

SET( BW_NO_UNICODE ON ) # Only required for Python to include WGToolsProject
INCLUDE( WGToolsProject )
SET( BW_NO_UNICODE OFF )

IF( NOT Python_FIND_VERSION )
    SET( Python_FIND_VERSION "2.7.11" )
ENDIF()

SET( CORE_PYTHON_CMAKE_DIR ${WG_TOOLS_THIRD_PARTY_DIR}/python )
SET( CORE_PYTHON_SRC_DIR ${CORE_PYTHON_CMAKE_DIR}/Python-${Python_FIND_VERSION} )
SET( CORE_PYTHON_INCLUDE_DIR ${CORE_PYTHON_SRC_DIR}/Include )
SET( CORE_PYTHON_LIB_DIR ${PYTHON_SRC_DIR}/Lib )
IF( EXISTS ${CORE_PYTHON_INCLUDE_DIR} )
	SET( CORE_PYTHON_EXISTS TRUE )
ELSE()
	SET( CORE_PYTHON_EXISTS FALSE )
ENDIF()

# Handle the QUIETLY and REQUIRED arguments and set PYTHON_FOUND to TRUE
# if all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Core_Python
	DEFAULT_MSG
	CORE_PYTHON_EXISTS
)

# Definitions, libraries and include dirs should be output for each FIND_PACKAGE script
# No definitions
SET( CORE_PYTHON_DEFINITIONS )
# No dependencies
SET( CORE_PYTHON_LIBRARIES )
# Includes
SET( CORE_PYTHON_INCLUDE_DIRS ${CORE_PYTHON_INCLUDE_DIR} )
# Extra includes for Windows
IF( WIN32 )
	LIST( APPEND CORE_PYTHON_INCLUDE_DIRS ${CORE_PYTHON_SRC_DIR}/PC )
ENDIF()

MARK_AS_ADVANCED( CORE_PYTHON_EXISTS
	CORE_PYTHON_CMAKE_DIR
	CORE_PYTHON_SRC_DIR
	CORE_PYTHON_INCLUDE_DIR
	CORE_PYTHON_LIB_DIR
)

IF( CORE_PYTHON_FOUND )
	MESSAGE( STATUS "Enabled ${PROJECT_NAME}. Python found in ${CORE_PYTHON_SRC_DIR}." )
ELSE()
	MESSAGE( STATUS "Disabled ${PROJECT_NAME}. Python not found in ${CORE_PYTHON_SRC_DIR}." )
ENDIF()

