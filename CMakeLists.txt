cmake_minimum_required(VERSION 3.5)
project(packet_sniffer VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES 
	"src/*.c"
)

file(GLOB HEADERS
    "include/*.h"
)

# Specify the include directories
include_directories(include)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} m)

