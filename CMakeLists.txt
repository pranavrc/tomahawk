PROJECT( tomahawk )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

IF( ${CMAKE_VERSION} VERSION_GREATER 2.8.3 )
  CMAKE_POLICY(SET CMP0017 NEW)
ENDIF( ${CMAKE_VERSION} VERSION_GREATER 2.8.3 )

###
### Tomahawk application info
###
SET( TOMAHAWK_ORGANIZATION_NAME "Tomahawk" )
SET( TOMAHAWK_ORGANIZATION_DOMAIN "tomahawk-player.org" )
SET( TOMAHAWK_APPLICATION_NAME  "Tomahawk" )
SET( TOMAHAWK_VERSION "0.0.3" )


# set paths
SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/CMakeModules" )
SET( THIRDPARTY_DIR  ${CMAKE_SOURCE_DIR}/thirdparty )

# Check if we need qtgui:
IF( "${gui}" STREQUAL "no" )
    ADD_DEFINITIONS( -DENABLE_HEADLESS )
    MESSAGE( STATUS "Building in HEADLESS mode ***" )
	FIND_PACKAGE( Qt4 4.7.0 COMPONENTS QtCore QtXml QtNetwork REQUIRED )
ELSE()
    MESSAGE( STATUS "Building full GUI version ***" )
	FIND_PACKAGE( Qt4 4.7.0 COMPONENTS QtGui QtCore QtXml QtNetwork REQUIRED )
ENDIF()

#deps
INCLUDE( MacroOptionalFindPackage )
INCLUDE( MacroLogFeature )

# required
#While we distribute our own liblastfm2, don't need to look for it
#macro_optional_find_package(LibLastFm 0.3.3)
#macro_log_feature(LIBLASTFM_FOUND "LastFm" "Qt library for the Last.fm webservices" "https://github.com/mxcl/liblastfm" FALSE "" "liblastfm is needed for scrobbling tracks to Last.fm and fetching cover artwork")
set(LIBLASTFM_FOUND true)

macro_optional_find_package(LibEchonest 1.1.1)
macro_log_feature(LIBECHONEST_FOUND "Echonest" "Qt library for communicating with The Echo Nest" "http://projects.kde.org/libechonest" TRUE "" "libechonest is needed for dynamic playlists and the infosystem")

macro_optional_find_package(CLucene 0.9.23)
macro_log_feature(CLucene_FOUND "CLucene" "The open-source, C++ search engine" "http://clucene.sf.net" TRUE "" "CLucene is used for indexing the collection")

macro_optional_find_package(QJSON)
macro_log_feature(QJSON_FOUND "QJson" "Qt library that maps JSON data to QVariant objects" "http://qjson.sf.net" TRUE "" "libqjson is used for encoding communication between Tomahawk instances")

macro_optional_find_package(Taglib 1.6.0)
macro_log_feature(TAGLIB_FOUND "TagLib" "Audio Meta-Data Library" "http://developer.kde.org/~wheeler/taglib.html" TRUE "" "taglib is needed for reading meta data from audio files")

# this installs headers and such and should really be handled in a separate package by packagers
IF( INTERNAL_JREEN )
    ADD_SUBDIRECTORY( thirdparty/jreen )
    SET( LIBJREEN_INCLUDE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/jreen/include )

    IF( UNIX AND NOT APPLE )
        SET( LIBJREEN_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/jreen/libjreen.so )
    ENDIF( UNIX AND NOT APPLE )
    IF( APPLE )
        SET( LIBJREEN_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/jreen/libjreen.dylib )
    ENDIF( APPLE )
    IF( WIN32 )
        SET( LIBJREEN_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/jreen/libjreen.dll )
    ENDIF( WIN32 )

    SET( LIBJREEN_FOUND  true )
    MESSAGE(STATUS "INTERNAL libjreen: ${LIBJREEN_INCLUDE_DIR}, ${LIBJREEN_LIBRARY}")
ELSE( INTERNAL_JREEN )
    macro_optional_find_package(Jreen)
ENDIF( INTERNAL_JREEN )
macro_log_feature(LIBJREEN_FOUND "Jreen" "Qt XMPP Library" "http://gitorious.org/jreen/jreen" FALSE "" "Jreen is needed for the Jabber SIP plugin. \n\n     Use -DINTERNAL_JREEN=ON to build the git submodule inside Tomahawk \n     Be aware this installs a full jreen with headers and everything!")

# we need pthreads too
find_package(Threads)

find_package(KDE4)
IF(KDE4_FOUND)
    #KDE4 adds and removes some compiler flags that we don't like
    STRING( REPLACE "-std=iso9899:1990" "" CLEAN_C_FLAGS ${CMAKE_C_FLAGS} )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions" )
ELSE()
    SET( CLEAN_C_FLAGS ${CMAKE_C_FLAGS} )
ENDIF()

include( CheckTagLibFileName )
check_taglib_filename( COMPLEX_TAGLIB_FILENAME )

IF( WIN32 )
  find_library(QTSPARKLE_LIBRARIES qtsparkle)
ENDIF( WIN32 )

#show dep log
macro_display_feature_log()
MESSAGE("WARNING!")
MESSAGE("Phonon is going to replace our own audio abstraction layer soon so we didn't")
MESSAGE("add checks for libmad, libvorbis and libflac. Make sure they are installed!")
MESSAGE("")
MESSAGE("-----------------------------------------------------------------------------")

SET( INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" )

# make uninstall support
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

# KDE4 defines an uninstall target for us automatically
IF( NOT KDE4_FOUND )
    ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
ENDIF()

IF( NOT APPLE )
    # Make linking as strict on linux as it is on osx. Then we don't break linking on mac so often
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined" )
ENDIF( NOT APPLE )

ADD_SUBDIRECTORY( thirdparty )
ADD_SUBDIRECTORY( src/libtomahawk )
ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( admin )
