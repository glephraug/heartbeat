
#include <iostream>
#include <vector>
#include <complex>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <pocketfft_hdronly.h>

int main(int argc, char ** argv)
{
   bool show_frequencies = true;

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
   std::cout << "obtained " << samples.size() << " samples" << std::endl;

   pocketfft::shape_t shape_in{samples.size()};
   pocketfft::stride_t stride_in{sizeof(double)};
   pocketfft::stride_t stride_out{sizeof(std::complex<double>)};
   pocketfft::shape_t axes{0};

   std::vector<std::complex<double>> fourier(samples.size());
   pocketfft::r2c(shape_in, stride_in, stride_out, axes, true, samples.data(), fourier.data(), 1.0, 1);

   if(show_frequencies){
      for(int i = 0; i < fourier.size(); ++i){
         std::cout << i << ": " << fourier[i] << std::endl;
      }
   }

   // get reasonable frequency

   return 0;
}
