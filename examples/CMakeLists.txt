file(GLOB_RECURSE TEST_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Make test executable

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} maf)
    add_test(NAME ${test_name}
             COMMAND mpiexec -n 2 ${test_name}
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

