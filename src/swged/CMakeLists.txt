
file(GLOB_RECURSE RESOURCES *.bmp *.ico *.mfcribbon-ms *.rc *.ico *.rc2)
file(GLOB_RECURSE SOURCES *.h *.hpp *.c *.cc *.cpp)

include(GroupSourcesByFilesystem)

GroupSourcesByFilesystem("${SOURCES}")

source_group("Resource Files" FILES ${RESOURCES})

string(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

add_definitions(-D_AFXDLL)

set(CMAKE_MFC_FLAG 2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /clr /AI\"${CMAKE_CURRENT_SOURCE_DIR}/References\"")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

add_executable(swged WIN32
    ${SOURCES}
    ${RESOURCES}
)

target_link_libraries(swged
    swganh_lib
    anh_lib
    ${Boost_LIBRARIES}
    debug ${ZLIB_LIBRARY_DEBUG}
    optimized ${ZLIB_LIBRARY_RELEASE}
)

string(REPLACE "/" "\\" WIN_PROJECT_BINARY_DIR "${CMAKE_BINARY_DIR}")
string(REPLACE "/" "\\" WIN_PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(
    TARGET swged
    POST_BUILD
    COMMAND xcopy "${WIN_PROJECT_SOURCE_DIR}\\References\\*.dll" "${WIN_PROJECT_BINARY_DIR}\\bin\\$\(Configuration\)" /D /I /Y /s
    VERBATIM
)
