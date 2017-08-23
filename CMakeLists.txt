
MACRO(DUSK_EXAMPLE name)

    PROJECT(${name})

    IF(NOT WIN32)
        SET(EXT ".run")
    ENDIF()

    SET(${name}_OUT ${name}${EXT})
    SET(${name}_SOURCES ${ARGN})
    FILE(GLOB_RECURSE ${name}_ASSETS "assets/*")

    ADD_EXECUTABLE(${${name}_OUT} ${${name}_SOURCES} ${${name}_ASSETS})
    TARGET_LINK_LIBRARIES(${${name}_OUT} Dusk)

    # Set IDE folders
    SET_TARGET_PROPERTIES(${${name}_OUT} PROPERTIES FOLDER "examples")
    SOURCE_GROUP("src" FILES ${${name}_SOURCES})
    SOURCE_GROUP("assets" FILES ${${name}_ASSETS})

    # Copy assets to build directory
    ADD_CUSTOM_TARGET(copy-${name}-assets)
    ADD_DEPENDENCIES(${${name}_OUT} copy-${name}-assets copy-example-assets)
    ADD_CUSTOM_COMMAND(
        OUTPUT
        TARGET copy-${name}-assets PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
            copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    )

    ADD_CUSTOM_TARGET(
        run-${name}
        COMMAND $<TARGET_FILE:${${name}_OUT}>
        DEPENDS ${${name}_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    SET_TARGET_PROPERTIES(
        copy-${name}-assets
        run-${name}
        PROPERTIES FOLDER "automation"
    )

    SET_TARGET_PROPERTIES(${${name}_OUT}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

ENDMACRO()

# Copy example projects shared assets folder
ADD_CUSTOM_TARGET(copy-example-assets)
ADD_CUSTOM_COMMAND(
    OUTPUT
    TARGET copy-example-assets PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

SET_TARGET_PROPERTIES(
    copy-example-assets
    PROPERTIES FOLDER "automation"
)

ADD_SUBDIRECTORY(Textured)
#ADD_SUBDIRECTORY(Flocking)
#ADD_SUBDIRECTORY(Checkers)
