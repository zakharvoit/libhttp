cmake_minimum_required(VERSION 2.8)
project(http)

set(CXX_DEBUG "-Wall -Wextra -g -D_GLIBCXX_DEBUG -fsanitize=undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_DEBUG} -std=c++14 ")

include_directories(libtcp/include)
include_directories(include)
#set(SOURCES     include/http/client.hh
		#include/http/request.hh
		#include/http/response.hh
		#include/http/uri.hh
#
		#src/http/http.cc)
set(SOURCES	include/http/impl/parser.hh
		src/http/impl/parser.cc)


add_subdirectory(libtcp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} tcp)

find_package(GTest)

set(TEST_SOURCES	src/test/gtest_main.cc

			src/test/parser_test.cc)
                    	
if(GTEST_FOUND)
	add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
	target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}
						   ${GTEST_LIBRARIES}
						   tcp)
endif(GTEST_FOUND)
