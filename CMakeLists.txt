cmake_minimum_required(VERSION 3.14)

# Set toolchain for Windows
project(MTGCollectionManager)

# Set C++17 as the standard and enforce it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED tesseract)
find_library(PIGPIO_LIBRARY pigpio)
if (NOT PIGPIO_LIBRARY)
    message(FATAL_ERROR "pigpio library not found")
endif()
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Enable AUTOMOC and AUTOUIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)

# Recursively find all .cpp and .ui files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")
message(STATUS "SOURCES: ${SOURCES}")

file(GLOB_RECURSE UI_FILES "src/*.ui")

# set(UI_FILES 
#   src/interface/MainWindow.ui
#   src/interface/filter.ui
#   src/interface/rarity.ui
#   src/interface/search.ui
#   src/interface/scan.ui
#   src/interface/type.ui
#   src/interface/collection.ui
# )

foreach(UI_FILE ${UI_FILES})
  get_filename_component(UI_FILENAME ${UI_FILE} NAME_WE)
  set_source_files_properties(src/interface/${UI_FILENAME}.cpp PROPERTIES SKIP_AUTOUIC ON)
endforeach()

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
# Define the executable and source files
add_executable(${PROJECT_NAME} ${SOURCES} ${UI_HEADERS})

foreach(HEADER ${UI_HEADERS})
  message(STATUS "UI header: ${HEADER}")
endforeach()

# Link libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${OpenCV_LIBS}
  ${Tesseract_LIBRARIES}
  sqlite3  
  /home/pi/SQLiteCpp/build/libSQLiteCpp.a 
  SQLiteCpp 
  ${PIGPIO_LIBRARY}
  Qt5::Core 
  Qt5::Gui 
  Qt5::Widgets
)

# Include directories for the project
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  include/common 
  include/cv 
  include/database
  include/feeder
  ${OpenCV_INCLUDE_DIRS}
  ${Tesseract_INCLUDE_DIRS}
  /home/pi/SQLiteCpp/include
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include
  ${CMAKE_CURRENT_SOURCE_DIR}/interface
  ${CMAKE_CURRENT_BINARY_DIR}

)

set_target_properties(${PROJECT_NAME} PROPERTIES
  AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

# ONLY FUNCTIONAL ON THE PI WITH THAT EXACT PATH 

