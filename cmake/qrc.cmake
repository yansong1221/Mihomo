file(GLOB_RECURSE QML_FILES ${PROJECT_SOURCE_DIR}/qml/*.qml)
file(GLOB_RECURSE JS_FILES ${PROJECT_SOURCE_DIR}/qml/*.js)

list(APPEND QML_FILES
    ${JS_FILES}
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${QML_FILES})

foreach(qml ${QML_FILES})
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" qml_alias ${qml})
    set_source_files_properties(${qml} PROPERTIES QT_RESOURCE_ALIAS ${qml_alias})
endforeach(qml)

# ----------------------------
file(GLOB_RECURSE RESOURCE_FILES ${PROJECT_SOURCE_DIR}/assets/*.svg)
list(APPEND RESOURCE_FILES ${PROJECT_SOURCE_DIR}/assets/meta.png)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${RESOURCE_FILES})

# ---------------------------
set(CXX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE CXX_SOURCE_FILES ${CXX_SOURCE_DIR}/*.cpp ${CXX_SOURCE_DIR}/*.c)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${CXX_SOURCE_FILES})

file(GLOB_RECURSE CXX_HEADER_FILES ${CXX_SOURCE_DIR}/*.h ${CXX_SOURCE_DIR}/*.hpp)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${CXX_HEADER_FILES})
