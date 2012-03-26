
function(GroupSourcesByFilesystem SOURCES)
    foreach(__source_file ${SOURCES})
        string(REGEX REPLACE "(${CMAKE_CURRENT_SOURCE_DIR}/)((.*/)*)(.*)" "\\2" __source_dir "${__source_file}")
        string(REGEX REPLACE "(${CMAKE_CURRENT_SOURCE_DIR}/${__source_dir})(.*)" "\\2" __source_filename "${__source_file}")
            
        string(REPLACE "/" "\\\\" __source_group "${__source_dir}")
        source_group("${__source_group}" FILES ${__source_file})
    endforeach()
endfunction()
