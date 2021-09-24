# Simple CMake template
cmake_minimum_required(VERSION 3.19)
project(MyGame VERSION 1.0.0)

# Set output directory
set(OUTPUT "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT})

# Add source files
add_executable(MyGame main.c)

# Get DLL path based on debug/release config
set(LIBDIR_DEBUG "${PROJECT_SOURCE_DIR}/../../../bin/shared/AMD64/Debug")
set(LIBDIR_RELEASE "${PROJECT_SOURCE_DIR}/../../../bin/shared/AMD64/Release")
set(LIBDIR "ReleaseOnly" CACHE STRING "Library directory (ON/OFF/ReleaseOnly)")
set_property(CACHE LIBDIR PROPERTY STRINGS ON OFF ReleaseOnly)
if(LIBDIR STREQUAL "ReleaseOnly")
  set(lib_dir_opt "$<IF:$<CONFIG:Debug>,${LIBDIR_DEBUG},${LIBDIR_RELEASE}>")
elseif(LIBDIR)
  set(lib_dir_opt "${LIBDIR_DEBUG}")
else ()
  set(lib_dir_opt "${LIBDIR_RELEASE}")
endif()

# Add DLL path
target_link_directories(MyGame PUBLIC "${lib_dir_opt}")

# Link to MARS library
target_link_libraries(MyGame PUBLIC mars)

# Add MARS header files
target_include_directories(MyGame PUBLIC "${PROJECT_SOURCE_DIR}/../../../include")

# Copy DLL afterwards
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${lib_dir_opt}/mars.dll"
    $<TARGET_FILE_DIR:MyGame>)