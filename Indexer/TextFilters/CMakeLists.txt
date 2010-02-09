ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(FilterPlainText_SRCS
    ./filterplaintext.cpp
    ./filterplaintext.h
)

set(FilterHtml_SRCS
    ./filterhtml.cpp
    ./filterhtml.h
)

qt4_automoc(
    ${FilterPlainText_SRCS}
    ${FilterHtml_SRCS}
)

ADD_LIBRARY(FilterPlainText SHARED ${FilterPlainText_SRCS})
ADD_LIBRARY(FilterHtml SHARED ${FilterHtml_SRCS})
