cmake_minimum_required(VERSION 3.16)

project(qml_client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Network)

qt_standard_project_setup()

set(RESOURCE_FILES
    res.qrc
)

qt_add_resources(RESOURCES ${RESOURCE_FILES})

set(SOURCE_FILES
    main.cpp
    mytimer.h mytimer.cpp
    myclient.h myclient.cpp
)

qt_add_executable(appqml_client
    ${SOURCE_FILES}
    ${RESOURCES}
)

qt_add_qml_module(appqml_client
    URI qml_client
    VERSION 1.0
    QML_FILES Main.qml
)

set_target_properties(appqml_client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqml_client
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Network
)

install(TARGETS appqml_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
