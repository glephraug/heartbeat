
cmake_minimum_required(VERSION 3.18.4)

project(heartbeat)

include(ExternalProject)

# Get dependencies
ExternalProject_Add(opencv
   GIT_REPOSITORY https://github.com/opencv/opencv.git
   GIT_TAG 4.7.0
)

find_package(OpenCV REQUIRED PATHS ${CMAKE_CURRENT_BINARY_DIR}/opencv-prefix/src/opencv-build/install NO_DEFAULT_PATH)

ExternalProject_Add(pocketfft
   GIT_REPOSITORY https://gitlab.mpcdf.mpg.de/mtr/pocketfft.git
   GIT_TAG cpp
   BUILD_COMMAND ""
   INSTALL_COMMAND ""
   CONFIGURE_COMMAND ""
)

set(pocketfft_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/pocketfft-prefix/src/pocketfft)

add_subdirectory(src)
