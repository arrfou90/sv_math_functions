#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <limits>
using namespace std;
using namespace cv;
 
Mat src; Mat dst(src.rows, src.cols, CV_16SC1);
 
/**
 * function main
 */
 int main( int argc, char** argv )
 {
 

  /// Load the source image
  src = imread( "../images/view2.png", 1 );
  
  /// Applying Bilateral Filter 
  
  int start2 = clock();
 
  
  int MAX_KERNEL_LENGTH =31 ;
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
  { bilateralFilter ( src, dst, i, i*2, i/2 );  }
   int end2 = clock();
   std::cout << "--bilateralFilter took: " << ((float)end2 - start2)/CLOCKS_PER_SEC  <<"seconds." << std::endl;

  return 0;
 }
 

