cmake_minimum_required(VERSION 2.8)

set(SRC_FILES
	./parser.cpp
)
set(HEADER_FILES
	./parser.h
)
add_library(parser STATIC ${SRC_FILES} ${HEADER_FILES})

#build the test application also
add_subdirectory(test ${CMAKE_CURRENT_BINARY_DIR}/test)
