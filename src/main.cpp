
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <pocketfft_hdronly.h>

int main(int argc, char ** argv)
{
   const std::string video_file = "CodingTest.mov";

   // open video
   cv::VideoCapture video(video_file);

   if(!video.isOpened()){
      std::cout << "Failed to open "  << video_file << std::endl;
      return 0;
   }

   cv::Mat frame;
   while(video.read(frame))
   {
      cv::imshow("frame", frame);
      cv::waitKey(10);
   }

   // sample forehead

   // aggregate in a reasonable manner

   // pass to fft

   // get reasonable frequency

   return 0;
}
