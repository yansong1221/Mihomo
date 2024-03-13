set(MODULE ClashMeta-GUI)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick Qml LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Quick Qml LinguistTools)
find_package(yaml-cpp CONFIG REQUIRED)


include(qrc)
#qt_add_executable(${MODULE} MANUAL_FINALIZATION ${MODULE_SOURCES}) 
qt_add_executable(${MODULE} ${CXX_SOURCE_FILES} ${CXX_HEADER_FILES}) 

target_compile_definitions(${MODULE}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

#translations----------
list(APPEND TS_FILES ${PROJECT_SOURCE_DIR}/i18n/${MODULE}_zh_CN.ts)

set_source_files_properties(${TS_FILES}
PROPERTIES OUTPUT_LOCATION "${PROJECT_SOURCE_DIR}/i18n/")

qt6_add_lupdate(${MODULE} TS_FILES ${TS_FILES})   
qt6_add_lrelease(${MODULE} TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE qm_files)
#translations----------

target_link_libraries(${MODULE} 
PRIVATE Qt${QT_VERSION_MAJOR}::Core  
PRIVATE Qt${QT_VERSION_MAJOR}::Gui  
PRIVATE Qt${QT_VERSION_MAJOR}::Quick
PRIVATE Qt${QT_VERSION_MAJOR}::Qml
PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
PRIVATE SingleApplication::SingleApplication
PRIVATE QWKQuick
PRIVATE yaml-cpp::yaml-cpp
PRIVATE fluentuiplugin fluentui
PRIVATE QCoro${QT_VERSION_MAJOR}Network
)

qt_add_qml_module(${MODULE}
    URI "Clash.Meta.GUI"
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES
        ${QML_FILES}
    )

qt_add_resources(${MODULE} assets
    PREFIX "/assets/"
    BASE "${PROJECT_SOURCE_DIR}/assets"
    FILES
    ${RESOURCE_FILES}
    )

qt_import_qml_plugins(${MODULE})

set_target_properties(${MODULE}
PROPERTIES
  CMAKE_AUTOUIC ON
  CMAKE_AUTOMOC ON
  CMAKE_AUTORCC ON
  CMAKE_INCLUDE_CURRENT_DIR ON
  CMAKE_CXX_STANDARD 20
  CMAKE_CXX_STANDARD_REQUIRED ON
)

if(FLUENTUI_BUILD_STATIC_LIB)
    target_compile_definitions(${MODULE} PRIVATE FLUENTUI_BUILD_STATIC_LIB)
endif()

target_include_directories(${MODULE} PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/SingleApplication)
target_include_directories(${MODULE} PRIVATE
        ${PROJECT_SOURCE_DIR}/3rdparty/FluentUI/src
        ${PROJECT_SOURCE_DIR}/src
    )
# add_custom_command(TARGET ${MODULE} POST_BUILD
#     COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${MODULE}>
#     COMMENT "Running windeployqt to deploy Qt dependencies"
# )