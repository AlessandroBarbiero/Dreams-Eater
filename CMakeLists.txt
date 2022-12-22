# Add as a subdirectory SimpleRenderEngineProject using add_subdirectory
IF (NOT DEFINED all_libs)
    message( FATAL_ERROR "This CMakeLists.txt must be included using the command add_subdirectory(name)" )
ENDIF (NOT DEFINED all_libs)
# Ensure SimpleRenderEngineProject is up-to-date
IF (WIN32)
    IF (NOT DEFINED DLLFileList)
        message( FATAL_ERROR "The SimpleRenderEngineProject has been updated. Clone the project (https://github.com/mortennobel/SimpleRenderEngineProject) again the project again and start over." )
    ENDIF(NOT DEFINED DLLFileList)
ENDIF(WIN32)


file(GLOB_RECURSE DreamsEater
        "Src/*.cpp"
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Used to call the include statements without referring to the folder structure
file(GLOB INCLUDE_DIRECTORIES
        "Header" 
        "rapidjson"
        "Header/*")

include_directories( ${INCLUDE_DIRECTORIES} )


add_executable(DreamsEaterProject ${DreamsEater})
target_link_libraries(DreamsEaterProject ${all_libs})

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
# Those are the folder displayed in visual studio.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DreamsEater})

# copy files to dest
file(COPY Sprites DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Sprites DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY Sprites DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
    file(COPY ${DLLFileList} DESTINATION .)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(DreamsEaterProject PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
