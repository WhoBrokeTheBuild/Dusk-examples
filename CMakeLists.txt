
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

    IF(UNIX)
        ADD_CUSTOM_TARGET(
            run-${name}
            COMMAND $<TARGET_FILE:${${name}_OUT}>
            DEPENDS ${${name}_OUT}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    ENDIF()

ENDMACRO()

ADD_SUBDIRECTORY(Cube)
