cmake_minimum_required(VERSION 3.24)
project(ClashMeta VERSION 1.0.0.0)

get_directory_property(_HAS_PARENT PARENT_DIRECTORY)
if(_HAS_PARENT)
    set(IS_ROOT_PROJECT OFF)
else()
    set(IS_ROOT_PROJECT ON)
endif()

if(IS_ROOT_PROJECT)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OUTPUT_DIR_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Debug)
        set(OUTPUT_DIR_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Release)
    else () 
        set(OUTPUT_DIR_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/x86/Debug)
        set(OUTPUT_DIR_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/x86/Release)
    endif()

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR_DEBUG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR_DEBUG})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR_DEBUG})

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR_RELEASE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR_RELEASE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR_RELEASE})

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif(IS_ROOT_PROJECT)


set(QAPPLICATION_CLASS QApplication)
set(QT_DEFAULT_MAJOR_VERSION 6)

set(QCORO_BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)
set(QTMWIDGETS_BUILD_TESTS OFF)
set(QWINDOWKIT_INSTALL OFF)


add_subdirectory(3rdparty/qtmwidgets)
add_subdirectory(3rdparty/qcoro)
add_subdirectory(3rdparty/SingleApplication)
add_subdirectory(3rdparty/qwindowkit)
add_subdirectory(3rdparty/qt-material-widgets)


add_subdirectory(src)
