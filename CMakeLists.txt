cmake_minimum_required(VERSION 3.14)

project(MTGCollectionManager)

# Set C++17 as the standard and enforce it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
set(SERVICE_COMPONENTS s3 rekognition)
# set(SERVICE_COMPONENTS s3)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED tesseract)
pkg_check_modules(Leptonica REQUIRED lept)
find_library(PIGPIO_LIBRARY pigpio)
if (NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found")
endif()
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
# find_package(AWSSDK REQUIRED COMPONENTS rekognition)
# find_package(aws-cpp-sdk-core REQUIRED)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})
find_package(CURL REQUIRED)


# Enable AUTOMOC, AUTORCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

# Recursively find all .cpp and .ui files in the src directory
# file(GLOB_RECURSE SOURCES "src/*.cpp" "include/cv/*.cpp" "include/database/*.cpp" "include/feeder/*.cpp")
set(MOC "include/cv/CardOCR.hpp" "include/database/DatabaseManager.hpp" "include/feeder/Feeder.hpp" "src/main.cpp")
# file(GLOB_RECURSE UI_FILES "src/*.ui")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE UI_FILES "src/*.ui")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# Define the executable and source files
add_executable(${PROJECT_NAME} ${SOURCES} ${UI_HEADERS} ${MOC})

# Link libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${OpenCV_LIBS}
  ${Tesseract_LIBRARIES}
  ${Leptonica_LIBRARIES}
  sqlite3  
  /home/pi/SQLiteCpp/build/libSQLiteCpp.a 
  SQLiteCpp 
  ${PIGPIO_LIBRARY}
  Qt5::Core 
  Qt5::Gui 
  Qt5::Widgets
  ${AWSSDK_LINK_LIBRARIES}
  CURL::libcurl
)

# Include directories for the project
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/cv 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/database
  ${CMAKE_CURRENT_SOURCE_DIR}/include/feeder
  ${CMAKE_CURRENT_SOURCE_DIR}/include/common
  ${OpenCV_INCLUDE_DIRS}
  ${Tesseract_INCLUDE_DIRS}
  ${Leptonica_INCLUDE_DIRS}
  #LEPT_INCLUDE_DIRS
  /home/pi/SQLiteCpp/include
  /home/pi/.local/bin
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src/interface
  ${CMAKE_CURRENT_BINARY_DIR}
  ${AWSSDK_INCLUDE_DIRS}
  
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/interface
)
