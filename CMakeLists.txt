cmake_minimum_required(VERSION 3.14)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(MTGCollectionManager)

# Set C++17 as the standard and enforce it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(Leptonica REQUIRED)
find_package(Tesseract CONFIG REQUIRED)
# find_package(unofficial-sqlite3 CONFIG REQUIRED)
# find_package(SQLiteCpp REQUIRED)


# # Define the executable and source files
# add_executable(${PROJECT_NAME}
#   src/main.cpp
#   src/cv/cardCondition.cpp
#   src/cv/cardOCR.cpp
#   src/feeder/feeder.cpp
#   src/ui/MainMenu.ui
#   src/ui/MainMenu.ui

# )

# Recursively find all .cpp and .ui files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE UI_FILES "src/*.ui")

# Define the executable and source files
add_executable(${PROJECT_NAME} ${SOURCES} ${UI_FILES})

# Link libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${OpenCV_LIBS}
  Tesseract::libtesseract 
  ${Leptonica_LIBRARIES} 
  # SQLite::SQLite3
  # unofficial::sqlite3::sqlite3
  # SQLiteCpp
  # ${SQLite3_LIBRARY}
)

# Include directories for the project
target_include_directories(${PROJECT_NAME} PRIVATE 
  include/common 
  include/cv 
  include/database
  include/feeder
  ${OpenCV_INCLUDE_DIRS}
  ${Tesseract_INCLUDE_DIRS}
  # ${SQLite3_INCLUDE_DIR}

)
