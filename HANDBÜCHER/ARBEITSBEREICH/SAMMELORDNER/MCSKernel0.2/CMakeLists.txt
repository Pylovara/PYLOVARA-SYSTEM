cmake_minimum_required(VERSION 3.10)
project(PylovaraMCSKernel VERSION 4.5)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug) # Für Tests

# Dependencies (GTK4 für GUI)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK4 REQUIRED gtk4)

# Include-Dirs
include_directories(include)
include_directories(Logic)
include_directories(Lib)

# Sources aus Logic/Lib
file(GLOB LOGIC_SRCS Logic/*.c)
file(GLOB LIB_SRCS Lib/*.c)

# Executable (mcs als Interpreter)
add_executable(mcs main.c ${LOGIC_SRCS} ${LIB_SRCS})

# Link GTK4
target_link_libraries(mcs PRIVATE ${GTK4_LIBRARIES})

# Install
install(TARGETS mcs DESTINATION bin)

# Tests (aus mcscodes/)
enable_testing()
file(GLOB MCS_TESTS mcscodes/*.mcs)
foreach(TEST_FILE ${MCS_TESTS})
    add_test(NAME ${TEST_FILE} COMMAND mcs ${TEST_FILE})
endforeach()
