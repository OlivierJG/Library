ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(HtmlViewCreator_SRCS
    ./htmlviewcreator.cpp
)

qt4_automoc(
    ${HtmlViewCreator_SRCS}
)

ADD_LIBRARY(HtmlViewCreator SHARED ${HtmlViewCreator_SRCS})
