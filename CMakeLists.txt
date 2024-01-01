cmake_minimum_required(VERSION 3.5)

project(DAQPlot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

INCLUDE_DIRECTORIES(.)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h

        qcustomplot.cpp
        qcustomplot.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DAQPlot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DAQ-Plot APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(DAQPlot SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DAQPlot
            ${PROJECT_SOURCES}
            mainwindow.ui
            assets.qrc
        )
    endif()
endif()

set(DAQ_VERSION 2.0.0)
add_definitions(-DUSE_DAQ122)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(MSVC)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(DAQ_LIB_BIT "x64")
        else()
            set(DAQ_LIB_BIT "x86")
        endif()
        set(DAQ_ROOT_PATH "${CMAKE_SOURCE_DIR}")
        set(DAQ_THIRD_PARTY_PATH "${DAQ_ROOT_PATH}/third_party")

        set(DAQ_INCLUDE_DIRS "${DAQ_ROOT_PATH}/include" "${DAQ_THIRD_PARTY_PATH}/include")
        set(DAQ_LIBRARIES "${DAQ_ROOT_PATH}/lib/${DAQ_LIB_BIT}/libdaq-${DAQ_VERSION}.lib")
    endif()
endif()


message("HEEEEEE")

message(${DAQ_LIBRARIES})
message(${DAQ_LIB_BIT})

INCLUDE_DIRECTORIES(${DAQ_INCLUDE_DIRS})

target_link_libraries(DAQPlot
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    ${DAQ_LIBRARIES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DAQ-Plot)
endif()
set_target_properties(DAQPlot PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DAQPlot
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)




if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DAQPlot)
endif()
