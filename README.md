# heartbeat
Estimate average heart rate from a video of a face.

## Usage
```
heartbeat <video_file> <x> <y> <width> <height>
```
x, y, width, and height specify the region within the video to sample when detecting the heart rate.
Options:
 --help         Display a help message.
 --default      Use the default video CodingTest.mov with roi x=400 y=150 width=110 height=80.
 --frequencies  Show all detected frequencies.
 --timing       Show run time of different parts of the program.


## Build
To build, open a terminal in the project's root directory and run:
```
mkdir build
cd build
cmake ..
cmake --build . --target heartbeat
```
CMake will automatically download all dependencies at configure time. If you would like to use a prebuild version of OpenCV, pass in the OpenCV_DIR variable.
```
cmake -DOpenCV_DIR="/path/to/your/opencv"
```

## Analysis Future Improvements
I've timed the program running on a video with 1822 frames of data. On my machine, sampling all frames takes a total of 28 milliseconds. The fourier transform takes 274 microseconds. Reading the video data with OpenCV is by far the slowest part of the program, taking about 10 seconds.

As far as run time is concerned, faster fourier transform algorithms exist, such as FFTW or CuFFT, but the impact on run time would be negligible. Likewise, sampling could be done much faster using Cuda or SIMD instructions, but the absolute difference in speed would be small.

The algorithm itself could be vastly improved by removing the need to specify a region of interest. A standard face detection algorithm (such as provided by MediaPipe) would give all the information necessary to fully automate the algorithm.

