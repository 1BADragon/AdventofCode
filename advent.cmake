
function(add_day NUMBER)
    add_library(${PROJECT_NAME}-day${NUMBER} SHARED ${ARGN})
    target_include_directories(${PROJECT_NAME}-day${NUMBER} PUBLIC ${CMAKE_SOURCE_DIR}/runner)
    set_target_properties(${PROJECT_NAME}-day${NUMBER}
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME day${NUMBER}
        )
endfunction()
