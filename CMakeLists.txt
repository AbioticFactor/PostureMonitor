# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MTGCollectionManager VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
# include_directories(src/common/cpp src/anotherDirectory)
include_directories(common/cpp)

# Add subdirectories that contain CMakeLists.txt for different modules
# This is where you specify other CMakeLists.txt files in subdirectories of your project
# add_subdirectory(src/ui/cpp)
# add_subdirectory(src/cv/cpp)
# Add other subdirectories as needed

# Add the executable for the project
# Specify the main source file and any additional files required
add_executable(MTGCollectionManager src/cpp/main.cpp)
# If you have more source files, list them here separated by spaces

# Link libraries if there are any
# target_link_libraries(MyProject SomeLibrary)
# Uncomment and replace 'SomeLibrary' with actual libraries if needed

# Additional configuration options can go here
# For example, compiler options, linking options, etc.
