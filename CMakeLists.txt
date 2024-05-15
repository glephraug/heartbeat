
cmake_minimum_required(VERSION 3.24.3)

project(heartbeat LANGUAGES CXX)

# Get dependencies

include(FetchContent)

# OpenCV doesn't work well with FetchContent, as far as I can tell: https://github.com/opencv/opencv/issues/20548
set(GET_LIBRARY_DIRECTORY ${CMAKE_BINARY_DIR} CACHE PATH "Location to keep prebuilt libraries")

if(NOT DEFINED ${OPENCV_VERSION})
   set(OPENCV_VERSION 4.9.0)
endif()

set(OPENCV_INSTALL_DIR ${CMAKE_BINARY_DIR}/opencv)
set(OPENCV_SOURCE_DIR ${OPENCV_INSTALL_DIR}-src)
set(OPENCV_BUILD_DIR ${OPENCV_INSTALL_DIR}-build)

set(OPENCV_BUILD_OPTIONS 
   -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF 
   -DBUILD_EXAMPLES=OFF -DBUILD_opencv_apps=OFF
   -DBUILD_SHARED_LIBS=OFF -DWITH_EIGEN=OFF
)

if(NOT EXISTS ${OPENCV_INSTALL_DIR})
   if(NOT EXISTS ${OPENCV_SOURCE_DIR})
      execute_process(COMMAND git clone --depth=1 -b 4.7.0 https://github.com/opencv/opencv.git ${OPENCV_SOURCE_DIR})
   endif()
   
   file(MAKE_DIRECTORY ${OPENCV_BUILD_DIR})
   file(MAKE_DIRECTORY ${OPENCV_INSTALL_DIR})
   execute_process(COMMAND ${CMAKE_COMMAND} -S ${OPENCV_SOURCE_DIR} -B ${OPENCV_BUILD_DIR} -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR} ${OPENCV_BUILD_OPTIONS})
   execute_process(COMMAND ${CMAKE_COMMAND} --build ${OPENCV_BUILD_DIR} --config Release)
   execute_process(COMMAND ${CMAKE_COMMAND} --install ${OPENCV_BUILD_DIR})
endif()

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED PATHS ${OPENCV_INSTALL_DIR} NO_DEFAULT_PATH)

FetchContent_Declare(
   pocketfft
   GIT_REPOSITORY https://gitlab.mpcdf.mpg.de/mtr/pocketfft.git
   GIT_TAG cpp
   GIT_SHALLOW TRUE
   GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(pocketfft)

set(pocketfft_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/_deps/pocketfft-src)


add_subdirectory(src)
