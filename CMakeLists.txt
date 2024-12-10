cmake_minimum_required(VERSION 3.12) # Wersja minimalna CMake
project(Bober)              # Nazwa projektu

# find proper  version of root
find_package(ROOT 6.28 REQUIRED)
include(${ROOT_USE_FILE})

# modify standard in case of problems
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRECTORIES
gui
style
)
Include_Directories(
 ${INCLUDE_DIRECTORIES}
)

set(SOURCES
    gui/AxisEditor.cxx
    gui/FillEditor.cxx
    gui/HistogramEditor.cxx
    gui/LineEditor.cxx
    gui/MainEditor.cxx
    gui/MarkerEditor.cxx
    gui/PadEditor.cxx
    gui/SuperEditor.cxx
    
    style/AxisStyle.cxx
    style/FillStyle.cxx
    style/HistoStyle.cxx
    style/LineStyle.cxx
    style/MarkerStyle.cxx
    style/PadStyle.cxx
    style/XMLNode.cxx
    style/Style.cxx
)

set(HEADERS)
foreach(SOURCE ${SOURCES})
    get_filename_component(DIR ${SOURCE} PATH)
    get_filename_component(FILE_NAME ${SOURCE} NAME_WE)
    list(APPEND HEADERS "${CMAKE_SOURCE_DIR}/${DIR}/${FILE_NAME}.h")
endforeach()

set(LINKDEF BoberLinkDef.h)

# generate dictionary
ROOT_GENERATE_DICTIONARY(G__Bober ${HEADERS} LINKDEF ${LINKDEF})

# make dictionary
add_library(Bober SHARED ${SOURCES} G__Bober.cxx)
target_link_libraries(Bober ${ROOT_LIBRARIES} Ged Gui XMLParser XMLIO)

# make example executable
add_executable(demo.exe demo/create.cpp)
target_link_libraries(demo.exe Bober ${ROOT_LIBRARIES} Ged Gui XMLParser XMLIO)

#install in proper places
install(TARGETS demo.exe
    RUNTIME DESTINATION bin
)

install(TARGETS Bober
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${CMAKE_BINARY_DIR}/libBober_rdict.pcm ${CMAKE_BINARY_DIR}/libBober.rootmap
    DESTINATION lib
)

