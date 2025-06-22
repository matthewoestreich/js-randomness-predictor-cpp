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

target_link_libraries(JSRandomnessPredictorTests
  Catch2::Catch2WithMain
  # NEED TO CHANGE THIS FOR OTHER PLATFORMS
  ${CMAKE_CURRENT_LIST_DIR}/../third_party/z3/lib/darwin-x64/libz3.a  
)

# Register with CTest
include(CTest)
include(Catch)
catch_discover_tests(JSRandomnessPredictorTests)
