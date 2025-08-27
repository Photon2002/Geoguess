cmake_minimum_required(VERSION 3.10)

project(Geoguess VERSION 1.0 LANGUAGES CXX)

add_subdirectory(dependencies)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(source)

add_executable(Geoguess
    ${SOURCE_FILES} 
)

target_link_libraries(Geoguess ${LINK_LIBRARIES})
target_include_directories(Geoguess PRIVATE 
    ${INCLUDE_DIRECTORIES}
    ${SOURCE_DIRECTORIES}
)
set(COMPILE_OPTIONS
    -W
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wunused
    -Wno-logical-op-parentheses
    -Wno-parentheses
    -Werror
    -Wno-comment
    -pedantic
    -pedantic-errors
    -D_GLIBCXX_DEBUG
    -D_GLIBCXX_DEBUG_PEDANTIC
    -D_UCRT
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(COMPILE_OPTIONS 
        ${COMPILE_OPTIONS}
        -O3
    )
    if (WIN32)
        target_link_options(Geoguess PRIVATE -mwindows)
    endif()
else()
    set(COMPILE_OPTIONS 
        ${COMPILE_OPTIONS}
        -g
    )
endif()

target_compile_options(Geoguess PUBLIC ${COMPILE_OPTIONS})

add_custom_target(filesToCopy DEPENDS
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_SOURCE_DIR}/textFiles
)

add_custom_command(TARGET filesToCopy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textFiles ${CMAKE_BINARY_DIR}/textFiles
)

add_dependencies(Geoguess filesToCopy)
