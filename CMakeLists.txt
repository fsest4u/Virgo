########################################################
#  
#  This is a CMake configuration file.
#  To use it you need CMake which can be 
#  downloaded from here: 
#    http://www.cmake.org/cmake/resources/software.html
#
#########################################################

cmake_minimum_required( VERSION 3.0.0 ) 

project( Virgo )

set( CMAKE_DEBUG_POSTFIX "d" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR}/package )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_extras" )

set( VIRGO_MAJOR_VERSION 0 )
set( VIRGO_MINOR_VERSION 1 )
set( VIRGO_REVISION_VERSION 0 )
set( VIRGO_FULL_VERSION ${VIRGO_MAJOR_VERSION}.${VIRGO_MINOR_VERSIONVIRGO_REVISION_VERSION}.${VIRGO_REVISION_VERSION} )
set( VIRGO_CODE_NAME Virgo )

#########################################################

add_subdirectory( 3rdparty/ )
add_subdirectory( src/ )
