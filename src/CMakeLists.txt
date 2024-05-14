
add_executable(heartbeat
   main.cpp
)

add_dependencies(heartbeat
   opencv
   pocketfft
)

target_include_directories(heartbeat PRIVATE
	${OpenCV_INCLUDE_DIRS}
   ${pocketfft_INCLUDE_DIRS}
)

target_link_libraries(heartbeat
	${OpenCV_LIBS}
)
