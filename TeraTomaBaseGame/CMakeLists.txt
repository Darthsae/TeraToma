cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
project(TeraTomaBaseGame VERSION 0.1.0  LANGUAGES C CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(a_outputDirectory "Mods")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(__DEBUG_ASSERTS__)
endif()

file(GLOB_RECURSE "${PROJECT_NAME}SourceFiles" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE "${PROJECT_NAME}HeaderFiles" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE "${PROJECT_NAME}AssetFiles" RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*" CMAKE_CONFIGURE_DEPENDS)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}SourceFiles} ${${PROJECT_NAME}HeaderFiles})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PRIVATE 
    TeraTomaLibrary CStandardLibrary)

target_compile_definitions(${PROJECT_NAME} PRIVATE "DLL_EXPORTS")

source_group(
    TREE "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}"
    PREFIX "Header Files"
    FILES "${PROJECT_NAME}HeaderFiles")

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/${a_outputDirectory}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/${a_outputDirectory}"
    PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/${a_outputDirectory}"
    PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/${a_outputDirectory}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/${a_outputDirectory}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/${a_outputDirectory}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/${a_outputDirectory}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/${a_outputDirectory}")

foreach(AssetFile IN LISTS "${PROJECT_NAME}AssetFiles")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${AssetFile}" "${CMAKE_BINARY_DIR}/Debug/${AssetFile}" COPYONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${AssetFile}" "${CMAKE_BINARY_DIR}/Release/${AssetFile}" COPYONLY)
endforeach(AssetFile)