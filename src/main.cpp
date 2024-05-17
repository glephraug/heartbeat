
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

   std::vector<double> samples;

   // sample forehead
   cv::Mat3b frame;
   while(video.read(frame))
   {
      double sample = 0.0;
      for(int r = roi.y; r < roi.y+roi.width; ++r)
         for(int c = roi.x; c < roi.x+roi.height; ++c)
         {
            sample += frame(r,c)(1);
         }
      samples.push_back(sample);

      //cv::imshow("frame", frame);
      //cv::waitKey(0);
   }
   std::cout << "obtained " << signal.size() << " samples" << std::endl;

   shape_t shape_in{samples.size()};
   stride_t stride_in{sizeof(float)};

   std::vector<complex<double>> fourier(samples.size());
   r2c(shape_t{samples.size()}, stride_t{sizeof(double)}, stride_t{sizeof(complex<double>)}, shape_t{0}, true, samples.data(), fourier.data(), ?1.0?, 1);

   // pass to fft

   // get reasonable frequency

   return 0;
}
