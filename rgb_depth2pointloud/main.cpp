#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <limits>

#include <opencv2/core/core.hpp>
#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <opencv2/photo/cuda.hpp>
#include "opencv2/core/cuda.hpp"
 //pcl/
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>


using namespace std;
using namespace cv;
void disparityToPointcloud (const int &width,
                            const int &height,
                            const float &focal,
                            const float &depth_scale,
                            cv::Mat  depthMap,
                            cv::Mat* leftStereoUndistorted,
                            const cv::Mat* filtered_disp,
                            pcl::PointCloud<pcl::PointXYZRGB>::Ptr& cloud_ptr)
{
    pcl::PointXYZRGB newPoint;
    float z ;
    float d ; //disparity variable float
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
        /*if(d<0) {z=3500.0f; }*/
        d=  filtered_disp->at<uint8_t>(i,j) ;
        {z= depth_scale/d;}

        if (z == z && z<1000 && z>20)  // z != z if depth Value is NaN
        {

            // fill in spatial information
            newPoint.y = (i-height/2)*z/focal; // X'= (X-Cx)*Z/f ; Cx is the focal center in the image
            newPoint.x = (j-width/2)*z/focal;
            newPoint.z = z;

            Vec3b & rgb = leftStereoUndistorted->at<Vec3b>(i,j); // exract color bgr from the left image
            //fill in color information
            newPoint.r = static_cast<uint8_t> (rgb[2]); //bgr 2 rgb
            newPoint.g = static_cast<uint8_t> (rgb[1]);
            newPoint.b = static_cast<uint8_t> (rgb[0]);

        }

        else
        {
            newPoint.x = newPoint.y = newPoint.z = std::numeric_limits<double>::quiet_NaN();

        }
         cloud_ptr->push_back(newPoint);
        }
    }

    // write PCD file
    /*pcl::io::savePCDFileASCII ("/home/user/work/rgb_depth2pointloud/build/cloud_afterDenois.pcd",  *cloud_ptr);
    std::cout<<"point cloud IS SAVED "<<".width: "<< cloud_ptr->width <<"cloud.height: "
    <<  cloud_ptr->height<<"cloud size: "<< cloud_ptr->points.size()<< std::endl;*/

}

int main(int argc, char** argv) {

    if( argc != 3)
    {
        cout <<" Usage: leftImage disparityImage" << endl;
        return -1;
    }


    cv::Mat leftStereoUndistorted;
    cv::Mat disparity_im;


    leftStereoUndistorted= imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the left image
    disparity_im = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);   // Read the disparity image

    if(! leftStereoUndistorted.data || !disparity_im.data) // Check for invalid input
    {
        cout <<  "Could not open or find the images" << std::endl ;
        return -1;
    }

    const float baseline = 0.0443493f;
    const float focal = 691.189f;
    const float depth_scale = baseline * focal*1000.0f;
    const int type = disparity_im.type();
    //normalize(disparity_im, disparity_im, 0, 255, CV_MINMAX, CV_8UC1);

    std::cout <<"disparity_im.type()"<< type<<std::endl;

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZRGB> &cloud= *cloud_ptr;
    cloud.width    = disparity_im.cols;
    cloud.height   = disparity_im.rows;
    cloud.is_dense = false;
    cloud.points.resize (disparity_im.cols * disparity_im.rows);
    cv::Mat depthMap(disparity_im.rows, disparity_im.cols, CV_16SC1);
    disparityToPointcloud (cloud.width, cloud.height, focal, depth_scale,  depthMap, &leftStereoUndistorted, &disparity_im,  cloud_ptr );

    //namedWindow( "Display window", WINDOW_AUTOSIZE );
    //imshow( "Display window", leftStereoUndistorted );
    normalize(depthMap, depthMap, 0, 255, CV_MINMAX, CV_8UC1);
    
    
    namedWindow("depthMap", WINDOW_AUTOSIZE);
    moveWindow("depthMap",100,400);
    imshow("depthMap", depthMap);
    cv::waitKey();


    return 0;
}
