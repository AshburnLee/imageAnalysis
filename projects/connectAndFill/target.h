//
// Created by junhui on 14/05/19.
//

#ifndef MISSION1SRC_TARGET_H
#define MISSION1SRC_TARGET_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class target{
public:
    void mainFunc(){
//        Mat original = imread("../mission1Data/1.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/2.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/3.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/4.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian1980_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian9660_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian12180_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian14520_label.png", CV_LOAD_IMAGE_COLOR);
        Mat original = imread("../mission1Data/ian14760_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian15000_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian15720_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian16740_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian15780_label.png", CV_LOAD_IMAGE_COLOR);
//        Mat original = imread("../mission1Data/ian19620_label.png", CV_LOAD_IMAGE_COLOR);
        Mat result;
        original.copyTo(result);


        int rows = result.rows;
        int cols = result.cols;

        int startR = -1;
        int startC = -1;
        int numberOnes = 0;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));

//        showVector(visited);
        for (int i=0;i<rows; i++){
            for (int j=0; j<cols; j++){
                if ( result.at<Vec3b>(i,j)[0] == 0 ) //  black pixel
                    numberOnes++;
            }
        }
        int tmpOnes = numberOnes;
        cout<<"tmpOnes: "<<tmpOnes<<endl;



        ///find the first black pixel as (startR, startC)
        for (int r=0; r<rows; r++){
            for (int c=0; c<cols; c++){
                if (result.at<Vec3b>(r,c)[0] == 0){
                    tmpOnes--;
                    visited[r][c] = true;
                    startR = r;
                    startC = c;
                    break;
                }
            }
            if (tmpOnes == numberOnes-1)
                break;
        }

        cout<<"Start: "<<startR<<","<<startC<<endl;


        ///
        int tailR = -1, tailC = -1;
        int headR = startR, headC = startC;

        while(tmpOnes > 33 ){
            for (int i=1; i<max(rows, cols); i++){
                if (Scan(headR, headC, tailR, tailC, i, rows, cols, result, visited)) {
                    tmpOnes--;

                    drawPath(headR, headC, tailR, tailC, result, visited);
                    headR = tailR;
                    headC = tailC;
                    cout << "(Main) find a  new head: " << headR << "," << headC <<"---"<<tmpOnes<< endl;
                    break;
                }
            }
        }

        cout<<"Final had and tail"<<endl;
        cout<<"new tail: "<<tailR<<","<<tailC<<endl;
        cout<<"start   : "<<startR<<","<<startC<<endl;

        /// connect head and tail
//        drawPath(startR, startC, tailR, tailC, result, visited);
        drawPath(startR, startC, headR, headC, result, visited);


        /// restore image
        showImage(original, "orihinal");
        showImage(result, "resulut");
//        showVector(visited);

        cv::imwrite("output14760.png", result);




    }

private:
    /**
     * @brief
     * @param r
     * @param c
     * @param tailR
     * @param tailC
     * @param l
     * @param rows
     * @param cols
     * @param result
     * @param visited
     * @return
     */

    bool Scan(int& r, int& c, int& tailR, int& tailC, int l, int rows, int cols,
            cv::Mat& result, vector<vector<bool>>& visited){

        vector<vector<int>> move;

        /// scan all viable pixels clock-wise
        for (int j=c-l+1; j<=c+l; j++){
            vector<int> tmp;
            if (r-l>=0 && r-l<rows && j>=0 && j<cols){
                tmp.push_back(r-l);
                tmp.push_back(j);
                move.push_back(tmp);
            }
        }

        for (int i=r-l+1; i<=r+l; i++){
            vector<int> tmp;
            if (i>=0 && i<rows && c+l>=0 && c+l<cols){
                tmp.push_back(i);
                tmp.push_back(c+l);
                move.push_back(tmp);
            }
        }

        for (int j =c+l-1; j>=c-l; j--){
            vector<int> tmp;
            if(r+l>=0 && r+l<rows && j>=0 && j<cols){
                tmp.push_back(r+l);
                tmp.push_back(j);
                move.push_back(tmp);
            }
        }

        for (int i=r+l-1; i>=r-l; i--){
            vector<int> tmp;
            if (i>=0 && i<rows && c-l>=0 && c-l<cols){
                tmp.push_back(i);
                tmp.push_back(c-l);
                move.push_back(tmp);
            }

        }

        /// for each pixel, check if it is black
        int moveSize = move.size();
        int count = 0;
        for (auto& step: move){

            if (result.at<Vec3b>(step[0], step[1])[0] != 0){
                count++;
                if (count == moveSize)
                    return false;
            }
            else if (result.at<Vec3b>(step[0], step[1])[0] == 0 && visited[step[0]][step[1]] == true ){
                count++;
                if (count == moveSize)
                    return false;
            }

            else if (result.at<Vec3b>(step[0], step[1])[0] == 0 && visited[step[0]][step[1]] == false){
                tailR = step[0];
                tailC = step[1];
                cout<<"(Scan) new tail: "<<tailR<<","<<tailC<<endl;
                visited[step[0]][step[1]] = true;
                return true;
            }

        }
        move.clear();
    }

    /**
     * @brief
     * @param hr
     * @param hc
     * @param tr
     * @param tc
     * @param result
     * @param visited
     */
    void drawPath(int hr, int hc, int tr, int tc,
            cv::Mat& result ,vector<vector<bool>>& visited){

        if (hr<tr) {
            for (int r = hr; r <= tr; r++) {
                result.at<Vec3b>(r, hc)[0] = 0;
                result.at<Vec3b>(r, hc)[1] = 0;
                result.at<Vec3b>(r, hc)[2] = 0;
                visited[r][hc] = true;
            }
        }
        else if (hr>tr){
            for (int r=tr; r<=hr; r++){
                result.at<Vec3b>(r, hc)[0] = 0;
                result.at<Vec3b>(r, hc)[1] = 0;
                result.at<Vec3b>(r, hc)[2] = 0;
                visited[r][hc] = true;
            }
        }
        if (tc>hc){
            for (int c=hc; c<=tc; c++){
                result.at<Vec3b>(tr,c)[0] = 0;
                result.at<Vec3b>(tr,c)[1] = 0;
                result.at<Vec3b>(tr,c)[2] = 0;
                visited[tr][c] = true;
            }
        }
        else if (tc<hc){
            for (int c=tc; c<=hc; c++){
                result.at<Vec3b>(tr,c)[0] = 0;
                result.at<Vec3b>(tr,c)[1] = 0;
                result.at<Vec3b>(tr,c)[2] = 0;
                visited[tr][c] = true;
            }
        }
        return;
    }





private:
    void showImage(cv::Mat& image, string name){
        cv::namedWindow(name, CV_WINDOW_AUTOSIZE);
        cv::imshow(name, image);
        cv::waitKey();
    }

    void showImageInfo(cv::Mat& image){
        cout<<"# of rows: "<<image.rows<<endl;
        cout<<"# of columns: "<<image.cols<<endl;
        cout<<"# dim: "<<image.dims<<endl;
        cout<<"size: "<<image.size<<endl;
        cout<<"flags: "<<image.flags<<endl;
        cout<<"step: "<<image.step<<endl;

    }

    void showVector(vector<vector<bool>>& visited){
        for (auto& i: visited){
            for (auto ii: i)
                cout<<ii<<" ";
            cout<<endl;
        }
    }
};


#endif //MISSION1SRC_TARGET_H
