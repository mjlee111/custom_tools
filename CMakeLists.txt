##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 3.5.0)
project(custom_tools)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(UI_FILES
    ui/tools.ui
)
SET(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qwt REQUIRED)

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.h)

QT5_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT5_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(custom_tools ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(custom_tools Qt5::Widgets Qt5::Gui Qwt::Qwt ${QT_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS custom_tools RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "github.com/mjlee111 <menggu1234@naver.com>")
set(CPACK_PACKAGE_DESCRIPTION "Ubuntu System Custom Tools")
set(CPACK_PACKAGE_VERSION "1.0.0")  
set(CPACK_PACKAGE_CONTACT "github.com/mjlee111")
set(CPACK_PACKAGE_NAME "custom_tools")  

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")  
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")  

install(TARGETS custom_tools RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

include(CPack)



