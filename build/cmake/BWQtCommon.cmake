#
# Common directory structure for Qt 5.1.1 Projects built with CMake 2.8.11 (or later)
#

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )

IF( NOT ${BW_PLATFORM} STREQUAL "win64" )
	MESSAGE( FATAL_ERROR "Qt build for ${QT_VERSION} with ${BW_PLATFORM} platform is not supported." )
ENDIF()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Setup Qt5 Build Paths

SET (Qt5_DIR "${BW_SOURCE_DIR}/core/third_party/Qt/${QT_VERSION}" )

IF( NOT EXISTS "${Qt5_DIR}/" )
	MESSAGE( FATAL_ERROR "Please clone Qt third party repository into ${Qt5_DIR} for Qt ${QT_VERSION} build." )
ENDIF()

IF ( CMAKE_GENERATOR STREQUAL "Visual Studio 11 Win64" )
	SET( Qt5_DIR "${Qt5_DIR}/msvc2012_64" )
ELSEIF( CMAKE_GENERATOR STREQUAL "Visual Studio 12 Win64" )
	SET( Qt5_DIR "${Qt5_DIR}/msvc2013_64" )
ELSE()
	SET( Qt5_DIR "${Qt5_DIR}/___unsupported___" )
ENDIF()

IF( NOT EXISTS "${Qt5_DIR}/" )
	MESSAGE( FATAL_ERROR "Qt build for \"Qt ${QT_VERSION}\" with \"${CMAKE_GENERATOR}\" is not supported." )
ENDIF()


SET( CMAKE_PREFIX_PATH ${Qt5_DIR} CMAKE_PREFIX_PATH )

# Setup Post-buildQt5  paths
SET( Qt5Bin_DIR "${Qt5_DIR}/bin" )
SET( Qt5Plugins_DIR "${Qt5_DIR}/plugins" )

# Include the required Qt5 Packages
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

