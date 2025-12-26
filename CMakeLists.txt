CMAKE_MINIMUM_REQUIRED(VERSION 4.0.0)
PROJECT(lambda LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "-O0 -g3 -DDEBUG=1")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
  set(CMAKE_C_FLAGS "-Os")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelDebInfo")
  set(CMAKE_C_FLAGS "-O2 -g3")
else() # Release
  set(CMAKE_C_FLAGS "-O2")
endif()

find_package(JNI REQUIRED)
add_subdirectory(src)

target_include_directories(lambda PRIVATE ${JNI_INCLUDE_DIRS})

message(STATUS "JNI include dirs: ${JNI_INCLUDE_DIRS}")

target_include_directories(
  lambda
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

install(
  EXPORT LibLambda
  FILE LibLambda.cmake
  DESTINATION lib/cmake/liblambda
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/LibLambdaConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/liblambda"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/LibLambdaConfigVersion.cmake"
  VERSION "${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/LibLambdaConfig.cmake
  DESTINATION lib/cmake/liblambda
)
