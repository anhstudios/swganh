
file(GLOB_RECURSE RESOURCES *.bmp *.ico *.mfcribbon-ms *.rc)
file(GLOB_RECURSE HEADERS *.h *.hpp)
file(GLOB_RECURSE SOURCES *.c *.cc *.cpp)

source_group("Header Files" FILES ${HEADERS})
source_group("Resource Files" FILES ${RESOURCES})
source_group("Source Files" FILES ${SOURCES})

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2) 

include_directories(${Boost_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

add_executable(swged WIN32
    ${HEADERS}
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
