set(BUILD_SHARED_LIBS False)

add_subdirectory(sfml EXCLUDE_FROM_ALL)

set(INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/sfml/include"
)

set(LINK_LIBRARIES
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
)

set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)
