project(Reading)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Reading_SRCS Reading.cpp main.cpp)
qt4_automoc(${Reading_SRCS})
add_executable(Reading ${Reading_SRCS})
target_link_libraries(Reading ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
