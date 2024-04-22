# Works with 3.11 and tested through 3.24
cmake_minimum_required(VERSION 3.11...3.24)

# Project name and a few useful settings. Other commands can pick up the results
project(
  TPX3Functions
  VERSION 0.1
  DESCRIPTION "Functions for TPX3 data manipulation and processing"
  LANGUAGES CXX)

# Set standard build flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS "-Werror -Wfatal-errors") # Enabled for debugging
SET(CMAKE_CXX_FLAGS  "-Wall")
# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(lib)

find_package(RooUnfold)
if(RooUnfold_FOUND)
message("-- RooUnfold - found")
else()
message("RooUnfold not found Deconvoluter can't be built")
endif()
