# heartbeat
Estimate average heart rate from a video of a face.

## Usage

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

## Future Improvements

