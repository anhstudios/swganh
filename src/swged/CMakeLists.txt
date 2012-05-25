
file(GLOB_RECURSE SOURCES *.h *.cpp *.cc *.rc)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2) 

include_directories(${Boost_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

add_executable(swged WIN32 
    ${SOURCES}
)
target_link_libraries(swged
    swganh_lib
    anh_lib
    ${Boost_LIBRARIES}
    debug ${ZLIB_LIBRARY_DEBUG}
    optimized ${ZLIB_LIBRARY_RELEASE}
)
