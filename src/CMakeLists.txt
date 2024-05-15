
add_executable(heartbeat
   main.cpp
)

if(MSVC)
   set_property(TARGET heartbeat PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

target_include_directories(heartbeat PRIVATE
	${OpenCV_INCLUDE_DIRS}
   ${pocketfft_INCLUDE_DIRS}
)

target_link_libraries(heartbeat
	${OpenCV_LIBS}
)
