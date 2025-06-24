cmake_minimum_required(VERSION 3.16)
project(CPPTests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.4
)
FetchContent_MakeAvailable(Catch2)

file(GLOB CPP_SOURCES ../src/*.cpp)

add_executable(JSRandomnessPredictorTests
  ${CPP_SOURCES}
  V8.test.cpp
  Chrome.test.cpp
  Firefox.test.cpp
)

target_include_directories(JSRandomnessPredictorTests PRIVATE
  ../src
  ../third_party/z3/include
)

# Determine platform and architecture so we know which libs to link.
set(Z3_LIB_PATH "")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(Z3_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../third_party/z3/lib/darwin-arm64/libz3.a")
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(Z3_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../third_party/z3/lib/darwin-x64/libz3.a")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(Z3_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../third_party/z3/lib/linux-x64/libz3.a")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(Z3_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../third_party/z3/lib/win32-x64/libz3.lib")
endif()

if(Z3_LIB_PATH STREQUAL "")
  message(FATAL_ERROR "Unsupported platform and/or architecture: ${CMAKE_SYSTEM_NAME}, ${CMAKE_SYSTEM_PROCESSOR}")
endif()
# END Determine platform and architecture

target_link_libraries(JSRandomnessPredictorTests
  Catch2::Catch2WithMain
  ${Z3_LIB_PATH}
)

# Register with CTest
include(CTest)
include(Catch)
catch_discover_tests(JSRandomnessPredictorTests)
