cmake_minimum_required(VERSION 3.14)

# Set toolchain for Windows
if(WIN32)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
endif()

project(MTGCollectionManager)

# Set C++17 as the standard and enforce it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED tesseract)
# find_package(Tesseract REQUIRED)
# If Leptonica is a direct dependency, uncomment the next line
# find_package(Leptonica REQUIRED)
find_library(PIGPIO_LIBRARY pigpio)
if (NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found")
endif()

# Recursively find all .cpp and .ui files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE UI_FILES "src/*.ui")

# Define the executable and source files
add_executable(${PROJECT_NAME} ${SOURCES} ${UI_FILES})

# Link libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${OpenCV_LIBS}
  ${Tesseract_LIBRARIES}
  sqlite3  
  /home/pi/SQLiteCpp/build/libSQLiteCpp.a  
  ${PIGPIO_LIBRARY}
  # If you directly use Leptonica, uncomment the next line
  # ${Leptonica_LIBRARIES}
)

# Include directories for the project
target_include_directories(${PROJECT_NAME} PRIVATE 
  include/common 
  include/cv 
  include/database
  include/feeder
  ${OpenCV_INCLUDE_DIRS}
  ${Tesseract_INCLUDE_DIRS}
  /home/pi/SQLiteCpp/include
  # If you directly use SQLite3 (not via SQLiteCpp), include its directory
  # ${SQLite3_INCLUDE_DIR}
)

# ONLY FUNCTIONAL ON THE PI WITH THAT EXACT PATH 
