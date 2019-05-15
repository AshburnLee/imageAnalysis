#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
// #define PRINT(X) std::cout<<X<<std::endl
using namespace std;

void writeLineByLine(cv::Mat&, std::string);
void showImage(cv::Mat, std::string);

cv::Mat image;

int main(int argc, char const *argv[])
{
    /******************************
     * user input
     * ***************************/
    string imgPath;
    std::cout<<"Source Image Path: \n";
    getline (cin, imgPath);
    string filePath;
    std::cout<<"Target File Path: \n";
    getline(cin, filePath);

    /****************************************
     * read an image and get image property: size
     * ***************************************/
    image = cv::imread(imgPath, 1);

    // PRINT("image 1st row:");
    // PRINT(image.rowRange(0, 1)); //print the 1st row

    cv::Size size1 = image.size();
    std:cout<<size1<<"\n";
    int h = size1.height;
    int w = size1.width;
    int c = image.cols;
    int r = image.rows;


    /****************************************
     * convert to grayscale and get the min & max values
     * *****************************************/
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    // cv::Mat tmp = grayImage.rowRange(0,1);
    double min, max;
    cv::minMaxLoc(grayImage, &min, &max);
    std::cout<<"max: "<<max<<"\n";
    std::cout<<"min: "<<min<<"\n";


    /*****************************************
     * show images
     * **************************************/
    showImage(image, "original");
    showImage(grayImage, "grayscale");

    writeLineByLine(grayImage, filePath);
    
    return 0;
}

void showImage(cv::Mat image, std::string name)
{
    cv::namedWindow(name, CV_WINDOW_AUTOSIZE);
    imshow(name, image);
    cv::waitKey();
}

/***********************************
 * create an outstream iterator(pointer) that 
 * goes through all elements of vector.
 * then convert elements to char.
 * fianlly copy them to a output file.
 * *********************************/
void writeLineByLine(cv::Mat& image, std::string file)
{
    for(int row=1; row<image.rows; row++){
        //read only one line
        cv::Mat tmp = image.rowRange(row, row+1);  
        std::vector<int> vecInt = (std::vector<int>)tmp;
        
        //TODO: change int to char:
        std::vector<char> vecChar(vecInt.size(), ' ');

        // set tierators for each vector
        auto itrA = vecInt.begin();
        auto itrB = vecChar.begin();
        while(itrA != vecInt.end() && itrB != vecChar.end()){
            if(*itrA > 0 && *itrA <= 15){
                *itrB = ';';
            }
            else if(*itrA > 15 && *itrA <=30){
                *itrB = '+';
            }
            else if(*itrA > 30 && *itrA <= 60){
                *itrB = '=';
            }
            else if(*itrA > 60 && *itrA <= 100){
                *itrB = '<';
            }
            else if(*itrA > 100 && *itrA <=130){
                *itrB = 'n';
            }
            else if(*itrA > 130 && *itrA <=160){
                *itrB = '$';
            }
            else if(*itrA >= 160 && *itrA <= 200){
                *itrB = '@';
            }
            else if(*itrA > 200 && *itrA <=255){
                *itrB = 'm';
            }
            ++itrA;
            ++itrB;
        }
        // write it to std::cout
        for_each(vecChar.begin(), vecChar.end(), [](char x){ cout<<x<<" "; });

        // write it to file:
        std::ofstream outputFile(file, std::fstream::app);
        std::ostream_iterator<char> output_iterator(outputFile, "");
        std::copy(vecChar.begin(), vecChar.end(), output_iterator);
        outputFile<<"\n";  //add new line to file
        outputFile.close();
    }


    // for_each(vecInt.begin(), vecInt.end(), [](int x){ std::cout<<x<<" "; });
    // for_each(vecInt.begin(), vecInt.end(), func);
    // std::cout<<"\n";

    //get the max and min value:
    // std::cout<<"The largest element is "<< *std::max_element(vecInt.begin(), vecInt.end())<<'\n';
    // std::cout<<"The smallest element is "<< *std::min_element(vecInt.begin(), vecInt.end())<<"\n";

    // void write2File(std::vector<int> vecInt)
    // {
    //     std::ofstream outputFile("./aaa.txt", std::fstream::app);
    //     std::ostream_iterator<int> output_iterator(outputFile, "\t");
    //     std::copy(vecInt.begin(), vecInt.end(), output_iterator);
    //     outputFile<<"\n";  //add new line to file
    // }
}



