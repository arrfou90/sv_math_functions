#include <iostream>
#include <numeric>
#include <sys/time.h>
#include <vector>
#include <stdlib.h>
#include <typeinfo>

#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
//pcl/ 
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <numeric>
#include <stdlib.h>
#include <ctime>
#include <sys/types.h>
#include <stdint.h>
#include <linux/limits.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
 

using namespace cv;
	
 
void disparityToPointcloud (const int &width,
                            const int &height,
                            const float &focal,
                            const float &depth_scale,
                            Mat &leftStereoUndistorted,
                            Mat &filtered_disp,Mat &depthMap,
                            pcl::PointCloud<pcl::PointXYZ> &cloud)
{
//int sparsityValue_=100;
//int windowSize_=7;
    float z ;

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        { 
             z= depth_scale/filtered_disp.at<unsigned char>(i,j); 
  
            // std::cout<< "z "<<z<<std::endl;
 
            
            // fill in spatial information
            cloud(j,i).y = (i-height/2)*z/focal;
            cloud(j,i).x = (j-width/2)*z/focal;
            cloud(j,i).z = z; 
        }
    }

     // write PCD file
   pcl::io::savePCDFileASCII ("/home/user/disToPCDoffline/build/cloud.pcd", cloud);
    std::cout<<"point cloud IS SAVED "<<".width: "<<cloud.width <<"cloud.height: "
    << cloud.height<<"cloud size: "<<cloud.points.size()<< std::endl;
}

int main(int argc, char** argv) { 

	float baseline=0.1750092;
	float focal =1392;

	float depth_scale = baseline*focal*1000; 
	cv::Mat disparity_im = cv::imread("/home/user/Downloads/sgm/example/left/0000000015.png"); 
	cv::Mat leftStereoUndistorted = cv::imread("/home/user/Downloads/sgm/example/disparities/0000000015.png"); 
	std::cout<<"baseline = "<< baseline<<std::endl;
	std::cout<<"focal_length_pixel = "<< focal<<std::endl;
	Mat depthMap(disparity_im.rows, disparity_im.cols, CV_16SC1);

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ> &cloud=*cloud_ptr ;
	cloud.width    = disparity_im.cols;
	cloud.height   = disparity_im.rows;
	cloud.is_dense = true;
	cloud.points.resize (disparity_im.cols * disparity_im.rows);



	disparityToPointcloud (cloud.width, cloud.height, focal, depth_scale, leftStereoUndistorted, disparity_im, depthMap, cloud );

	return 0;
}
