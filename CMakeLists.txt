
MACRO(DUSK_EXAMPLE name)

    PROJECT(${name})

    IF(NOT WIN32)
        SET(EXT ".run")
    ENDIF()

    SET(${name}_OUT ${name}${EXT})
    SET(${name}_SOURCES ${ARGN})

    ADD_EXECUTABLE(${${name}_OUT} ${${name}_SOURCES})
    TARGET_LINK_LIBRARIES(${${name}_OUT} Dusk)

    SET_TARGET_PROPERTIES(${${name}_OUT} PROPERTIES FOLDER "examples")
    SOURCE_GROUP("src" FILES ${${name}_SOURCES})

    # Copy assets to build directory
    ADD_CUSTOM_TARGET(copy-${name}-assets)
    ADD_DEPENDENCIES(${${name}_OUT} copy-${name}-assets copy-example-assets)
    ADD_CUSTOM_COMMAND(
        TARGET copy-${name}-assets PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
            copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    )

    IF(UNIX)
        ADD_CUSTOM_TARGET(
            run-${name}
            COMMAND $<TARGET_FILE:${${name}_OUT}>
            DEPENDS ${${name}_OUT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    ENDIF()

ENDMACRO()

# Copy example projects shared assets folder
ADD_CUSTOM_TARGET(copy-example-assets)
ADD_CUSTOM_COMMAND(
    TARGET copy-example-assets PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

ADD_SUBDIRECTORY(Cube)
ADD_SUBDIRECTORY(Lighting)
ADD_SUBDIRECTORY(Textured)
