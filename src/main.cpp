
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <pocketfft_hdronly.h>


class TimeKeeper
{
public:

   TimeKeeper() : total(0) {}

   void Start(){
      start = std::chrono::high_resolution_clock::now();
   }

   void Stop(){
      total += std::chrono::high_resolution_clock::now()-start;
   }

   std::chrono::high_resolution_clock::duration Total() const {
      return total;
   }

private:
   std::chrono::high_resolution_clock::time_point start;
   std::chrono::high_resolution_clock::duration total;
};


int main(int argc, char ** argv)
{
   bool show_frequencies = false;
   bool default_data = false;
   bool show_help = false;
   bool show_timing = false;

   std::string video_file;
   cv::Rect roi;

   if(argc < 2){
      show_help = true;
   }else{
      int i;
      for(i = 1; i < argc && argv[i][0] == '-'; ++i){
         std::string arg(argv[i]);
         if(arg == "--help"){
            show_help = true;
         }else if(arg == "--default"){
            default_data = true;
         }else if(arg == "--frequencies"){
            show_frequencies = true;
         }else if(arg == "--timing"){
            show_timing = true;
         }else{
            std::cerr << "Unknown option " << arg << std::endl;
            show_help = true;
         }
      }

      if(default_data){
         video_file = "CodingTest.mov";
         roi = cv::Rect(400, 150, 110, 80);
      }else if(i+4 >= argc){
         std::cerr << "Not enough arguments" << std::endl;
         show_help = true;
      }else{
         try{
            video_file = argv[i];
            roi.x = std::stoi(argv[i+1]);
            roi.y = std::stoi(argv[i+2]);
            roi.width = std::stoi(argv[i+3]);
            roi.height = std::stoi(argv[i+4]);
         }catch(std::invalid_argument & e){
            std::cerr << "Invalid ROI argument" << std::endl;
            show_help = true;
         }
      }
   }
   
   if(show_help){
      std::cout << "Usage: " << std::endl 
                << argv[0] << " <video_file> <x> <y> <width> <height>" << std::endl
                << "x, y, width, and height specify the region within the video to sample when detecting the heart rate." << std::endl
                << "Options:" << std::endl
                << " --help         Display this help message." << std::endl
                << " --default      Use the default video CodingTest.mov with roi x=400 y=150 width=110 height=80." << std::endl
                << " --frequencies  Show all detected frequencies." << std::endl
                << " --timing       Show run time of different parts of the program." << std::endl;
      return 0;
   }

   // Minimum and maximum possible heart rates.
   double minimum_bpm = 20.0;
   double maximum_bpm = 150.0;

   TimeKeeper video_time;
   TimeKeeper sample_time;
   TimeKeeper fft_time;
   TimeKeeper search_time;


   // open video
   cv::VideoCapture video(video_file);

   if(!video.isOpened()){
      std::cerr << "Failed to open "  << video_file << std::endl;
      return 0;
   }

   double fps = video.get(cv::CAP_PROP_FPS);

   // sample forehead
   std::vector<double> samples;
   cv::Mat3b frame;
   video_time.Start();
   while(video.read(frame))
   {
      video_time.Stop();

      sample_time.Start();
      double sample = 0.0;
      for(int r = roi.y; r < roi.y+roi.width; ++r)
         for(int c = roi.x; c < roi.x+roi.height; ++c)
         {
            sample += frame(r,c)(1);
         }
      sample /= roi.area();
      samples.push_back(sample);
      sample_time.Stop();

      //cv::imshow("frame", frame);
      //cv::waitKey(0);
      video_time.Start();
   }
   video_time.Stop();
   std::cout << "obtained " << samples.size() << " samples" << std::endl;

   if(samples.empty()){
      std::cerr << "There were no samples within the video file" << std::endl;
      return 0;
   }
   

   // Run fourier transform

   double total_time = samples.size()/fps;
   double frequency_interval = 1.0/total_time;

   pocketfft::shape_t shape_in{samples.size()};
   pocketfft::stride_t stride_in{sizeof(double)};
   pocketfft::stride_t stride_out{sizeof(std::complex<double>)};
   pocketfft::shape_t axes{0};

   std::vector<std::complex<double>> fourier(samples.size());
   fft_time.Start();
   pocketfft::r2c(shape_in, stride_in, stride_out, axes, true, samples.data(), fourier.data(), 1.0, 1);
   fft_time.Stop();


   // Display any requested debug info

   if(show_timing){
      std::cout << "Video reading time: " << std::chrono::duration_cast<std::chrono::microseconds>(video_time.Total()).count() << " us" << std::endl;
      std::cout << "Sampling time: " << std::chrono::duration_cast<std::chrono::microseconds>(sample_time.Total()).count() << " us" << std::endl;
      std::cout << "FFT time: " << std::chrono::duration_cast<std::chrono::microseconds>(fft_time.Total()).count() << " us" << std::endl;
   }

   if(show_frequencies){
      for(int i = 0; i < fourier.size()/2; ++i){
         std::cout << (i*frequency_interval) << " Hz (" << (i*frequency_interval*60.0) << " bpm) : " << fourier[i].real() << std::endl;
      }
   }


   // Find most likely heartrate

   double max_bpm = 0.0;
   double max_value = -1.0;
   int start = minimum_bpm/(frequency_interval*60.0);
   int end = maximum_bpm/(frequency_interval*60.0)+1;
   if(start < 0) start = 0;
   if(end > fourier.size()/2) end = fourier.size()/2;
   for(int i = start; i < end; ++i){
      double bpm = (i*frequency_interval*60.0);
      double value = std::abs(fourier[i].real());
      if(value > max_value){
         max_value = value;
         max_bpm = bpm;
      }
   }

   if(max_value < 0.0){
      std::cerr << "No valid human heartbeats found. Video sequence may be too short." << std::endl;
      return 0;
   }

   std::cout << "Estimated heartrate is " << max_bpm << " beats per minute" << std::endl;

   return 0;
}
