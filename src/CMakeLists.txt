# find source files
file(GLOB_RECURSE MAF_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/maf/*.cpp)

file(GLOB_RECURSE MAF_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/maf/*.hpp)

foreach(maf_header ${MAF_HEADERS})
    string(APPEND MAF_HEADER_LIST "\n#include \"${maf_header}\"")
endforeach()

configure_file(maf/maf.hpp.in
               maf/maf.hpp
               @ONLY)

add_library(maf ${MAF_SOURCES})
target_compile_features(maf PUBLIC cxx_std_11)

target_link_libraries(maf ${MPI_CXX_LIBRARIES})

target_include_directories(maf
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
               ${CMAKE_CURRENT_BINARY_DIR}
               )
