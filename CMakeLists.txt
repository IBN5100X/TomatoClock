cmake_minimum_required(VERSION 3.5)

project(TomatoClock LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Charts模块
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Charts Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Charts Multimedia REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    focuswindow.cpp
    focuswindow.h
    addtododialog.cpp
    addtododialog.h
    todoitemwidget.cpp
    todoitemwidget.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TomatoClock ${PROJECT_SOURCES})
else()
    add_executable(TomatoClock ${PROJECT_SOURCES})
endif()

target_link_libraries(TomatoClock PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Multimedia
)
