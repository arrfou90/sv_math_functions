#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/cudawarping.hpp>
using namespace std;
using namespace cv;

void  stereoCamParametres(Mat &omegaL, Mat &omegaR,
                          Mat &distCoeffsL, Mat &distCoeffsR,
                          Mat &R12, Mat &t12, float &focal,
                          float &depth_scale)
{
    const char* calibrationFile =
    "/home/user/work/pg_stereovision_subpixel/calibrationFiles/calibrationOutputs/calibrationData.xml";
    FileStorage cdfs(calibrationFile, FileStorage::READ);
    cdfs["leftCamMat"] >> omegaL;
    cdfs["leftDistCoeffs"] >> distCoeffsL;
    cdfs["rightCamMat"] >> omegaR;
    cdfs["rightDistCoeffs"] >> distCoeffsR;
    cdfs["stereoRotMat"] >> R12;
    cdfs["stereoTranslVect"] >> t12;
    // print baseline,focal length in pixels
    focal = omegaL.at<double>(0,0);
    float baseline = (-R12*t12).dot((cv::Mat_<double>(3,1) << 1, 0, 0));
    //baseline= 0.0443493;
    //focal =691.189;
    std::cout<<"baseline = "<< baseline<<std::endl;
    std::cout<<"focal_length_pixel = "<< focal<<std::endl;
    depth_scale = baseline * focal*1000;
}

void remap_CPU(cv::Mat leftImage_,cv::Mat rightImage_, cv::Mat leftStereoUndistorted_, cv::Mat rightStereoUndistorted_ )
{
    Mat omegaL, omegaR, distCoeffsL, distCoeffsR, R12, t12;

    Mat R1, R2, P1, P2, Q, map1l, map2l, map1r, map2r;
    Mat leftStereoUndistorted, rightStereoUndistorted;
    // stereo rectification

    stereoRectify(omegaL,distCoeffsL,omegaR,distCoeffsR,Size(leftImage_.size().width,leftImage_.size().height),R12,t12,R1,R2,P1,P2,Q);


    // camera parameter correction and remapping
    initUndistortRectifyMap(omegaL,distCoeffsL,R1,P1,Size(leftImage_.size().width,leftImage_.size().height),CV_32FC2,map1l,map2l);
    remap(leftImage_,leftStereoUndistorted,map1l,map2l,INTER_LINEAR);

    // camera parameter correction and remapping
    initUndistortRectifyMap(omegaR,distCoeffsR,R2,P2,Size(rightImage_.size().width,rightImage_.size().height),CV_32FC2,map1r,map2r);
    remap(rightImage_,rightStereoUndistorted,map1r,map2r,INTER_LINEAR);
}


int main(int argc, char** argv) {

    if( argc != 3)
    {
        std::cout <<" Usage: leftImage disparityImage" << std::endl;
        return -1;
    }


    cv::Mat leftImage, rightImage;


    leftImage= cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the left image
    rightImage = cv::imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);   // Read the disparity image

    if(! leftImage.data || !rightImage.data) // Check for invalid input
    {
        std::cout <<  "Could not open or find the images" << std::endl ;
        return -1;
    }

    Mat omegaL, omegaR, distCoeffsL, distCoeffsR, R12, t12;
    float focal, depth_scale;
    stereoCamParametres(omegaL, omegaR, distCoeffsL, distCoeffsR, R12, t12, focal, depth_scale);

    cv::Mat leftStereoUndistorted, rightStereoUndistorted;
    //remap_CPU(leftImage,rightImage,  leftStereoUndistorted,  rightStereoUndistorted );


    ////////////////////////////////////
    // CPU


    Mat R1, R2, P1, P2, Q, map1l, map2l, map1r, map2r;
    int size =leftImage.size().width*leftImage.size().width ;
    //map1l.create(leftImage.size() , CV_32F);
    //map2l.create(leftImage.size() , CV_32F);
     // stereo rectification

    stereoRectify(omegaL,distCoeffsL,omegaR,distCoeffsR,Size(leftImage.size().width,leftImage.size().width),R12,t12,R1,R2,P1,P2,Q);


    // camera parameter correction and remapping
    initUndistortRectifyMap(omegaL,distCoeffsL,R1,P1,Size(leftImage.size().width,leftImage.size().height),CV_32F,map1l,map2l);


    // camera parameter correction and remapping
    initUndistortRectifyMap(omegaR,distCoeffsR,R2,P2,Size(rightImage.size().width,rightImage.size().height),CV_32F,map1r,map2r);
    remap(leftImage,leftStereoUndistorted,map1l,map2l,INTER_LINEAR);

    /*int start  = clock();

    //remap(rightImage,rightStereoUndistorted,map1r,map2r,INTER_LINEAR);
    int end   = clock();
    std::cout << "--remap "<<"(width,height) = "<<" ("<<leftImage.size().width<<","<<leftImage.size().height<< ") in a "  << end  - start  << "ticks, or " << ((float)end - start )/CLOCKS_PER_SEC  <<"seconds." << std::endl;
*/
/////////////////////////////
    int  width = 640;
    int height = 480;
    cv::Mat in(height,width, CV_8U);
    cv::Mat out(height,width, CV_8U);
    cv::Mat m1(height,width, CV_32F);
    cv::Mat m2(height,width, CV_32F);
    int start  = clock();

    remap(in,out,m1,m2,INTER_LINEAR);
    remap(in,out,m1,m2,INTER_LINEAR);

     int end   = clock();
    std::cout << "--remap "<<"(width,height) = "<<" ("<<leftImage.size().width<<","<<leftImage.size().height<< ") in a "  << end  - start  << "ticks, or " << ((float)end - start )/CLOCKS_PER_SEC *1000.0f  <<" m seconds." << std::endl;
/////////////////////////////////
    //////////////
    //GPU
    cv::cuda::GpuMat d_leftImage(leftImage);
    cv::cuda::GpuMat d_leftStereoUndistorted ;
    cv::cuda::GpuMat d_m1(map1l) ;
    cv::cuda::GpuMat d_m2(map2l) ;

     //d_m1.upload(map1l);
     //d_m2.upload(map2l);


    int start1  = clock();

    cv::cuda::remap(d_leftImage,d_leftStereoUndistorted,d_m1,d_m2,INTER_LINEAR);
   // cv::cuda::remap(d_in,d_out,d_m1,d_m2,INTER_LINEAR);

    // remap(leftImage,d_leftStereoUndistorted,map1l,map2l,INTER_LINEAR);
    //cv::cuda::remap(d_leftImage, m_outputGPU, matXGPU, matYGPU, CV_INTER_LINEAR, cv::BORDER_CONSTANT);
    //remap(rightImage,rightStereoUndistorted,map1r,map2r,INTER_LINEAR);

    int end1   = clock();
    std::cout << "--remap "<<"(width,height) = "<<" ("<<leftImage.size().width<<","<<leftImage.size().height<< ") in a "  << end1  - start1  << "ticks, or " << ((float)end1 - start1 )/CLOCKS_PER_SEC*1000.0f  <<"m seconds." << std::endl;
    Mat h_leftStereoUndistorted(d_leftStereoUndistorted);
    //outputCpu.download(d_leftStereoUndistorted);

    //

    std::cout << "speed up factor" <<(((float)end - start )/CLOCKS_PER_SEC)/(((float)end1 - start1 )/CLOCKS_PER_SEC) <<std::endl;


    namedWindow("leftStereoUndistorted", WINDOW_NORMAL);
    moveWindow("leftStereoUndistorted",100,400);
    imshow("leftStereoUndistorted", h_leftStereoUndistorted);
    cv::waitKey(50000);
    return 0;
}