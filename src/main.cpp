
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <pocketfft_hdronly.h>

int main(int argc, char ** argv)
{
   const std::string video_file = "CodingTest.mov";
   cv::Rect roi(400, 150, 110, 80);

   // open video
   cv::VideoCapture video(video_file);

   if(!video.isOpened()){
      std::cerr << "Failed to open "  << video_file << std::endl;
      return 0;
   }

   double fps = video.get(cv::CAP_PROP_FPS);

   std::vector<double> signal;

   cv::Mat3b frame;
   while(video.read(frame))
   {
      double sum = 0.0;
      for(int r = roi.y; r < roi.y+roi.width; ++r)
         for(int c = roi.x; c < roi.x+roi.height; ++c)
         {
            sum += frame(r,c)(1);
         }
      signal.push_back(sum);

      //cv::imshow("frame", frame);
      //cv::waitKey(0);
   }
   std::cout << "obtained " << signal.size() << " samples" << std::endl;

   // sample forehead

   // aggregate in a reasonable manner

   // pass to fft

   // get reasonable frequency

   return 0;
}
