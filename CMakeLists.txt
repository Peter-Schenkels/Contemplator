cmake_policy(SET CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

cmake_minimum_required(VERSION 3.5)

project(Contemplator VERSION 1.0.0)  

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include(CMake/AddProjectMacro/Macros.cmake)

# FILE GROUP SRC_CONTEMPLATOR
set(SRC_CONTEMPLATOR
	"src/ast.hpp"
	"CMakeLists.txt"
	"src/contemplator.cpp"
	"include/contemplator.hpp"
	"src/language_patterns.cpp"
	"src/language_patterns.hpp"
	"src/regex_tokenizer.cpp"
	"src/regex_tokenizer.hpp"
	"src/template_token.hpp"
	"src/token_group.hpp"
)

source_group("Contemplator" FILES ${SRC_CONTEMPLATOR})

# LIBRARY GROUP Contemplator
set(SRC_LIB_CONTEMPLATOR ${SRC_CONTEMPLATOR} )

# define the target
add_project(CONTEMPLATOR
            STATIC_LIB
            SOURCES ${SRC_LIB_CONTEMPLATOR}
            DEPENDS )