cmake_minimum_required(VERSION 3.19)
project(mars VERSION 1.0.0)

## C language configuration boilerplate

if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

## Let MARS_SHARED_LIBS override BUILD_SHARED_LIBS

if (DEFINED MARS_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${MARS_SHARED_LIBS}")
endif ()

## Specify library output directory

if (BUILD_SHARED_LIBS)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/shared")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/shared")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/shared")
else()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/static")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/static")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/static")
endif()

## Create the main mars library target

file(GLOB_RECURSE libsrc "src/*.c")
add_library(mars ${libsrc})
add_library(mars::mars ALIAS mars)
set_target_properties(mars PROPERTIES
                      VERSION ${mars_VERSION}
                      SOVERSION ${mars_VERSION_MAJOR})
target_include_directories(
    mars PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_compile_features(mars PUBLIC c_std_11)

## Generate the export header for mars and attach it to the target

include(GenerateExportHeader)
generate_export_header(mars EXPORT_FILE_NAME include/exports/export.h)
target_compile_definitions(
    mars PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:MARS_STATIC_DEFINE>")
target_include_directories(
    mars PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

## Include the install rules if the user wanted them (included by default when top-level)

#string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
#option(mars_INCLUDE_PACKAGING "Include packaging rules for mars" "${is_top_level}")
#if (mars_INCLUDE_PACKAGING)
#    add_subdirectory(packaging)
#endif ()