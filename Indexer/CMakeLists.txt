PROJECT(indexer)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(indexer_SRCS
    main.cpp
    indexer.cpp
)

qt4_automoc(${indexer_SRCS})

ADD_EXECUTABLE(indexer ${indexer_SRCS})

TARGET_LINK_LIBRARIES(
    indexer
    xapian
    ${QT_QTCORE_LIBRARIES}
    kio
)

add_subdirectory(TextFilters)