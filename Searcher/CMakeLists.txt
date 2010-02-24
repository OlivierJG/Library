project(Searcher)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories("/usr/include/xapian/" ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Searcher_SRCS
    searchtreeview.cpp
    mylineedit.cpp
    searchmodel.cpp
    filebrowser.cpp
    queryresultview.cpp
    searcher.cpp
    main.cpp
    searchthread.cpp
)
qt4_automoc(${Searcher_SRCS})
add_executable(Searcher ${Searcher_SRCS})
target_link_libraries(Searcher xapian ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
