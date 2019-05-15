//
// Created by junhui on 14/05/19.
//

#ifndef MISSION1SRC_HELPER_H
#define MISSION1SRC_HELPER_H

#include <iostream>
#include <vector>
using namespace std;

class helper{
public:

    /**
     * @brief: The entrance of the whole process
     */
    void mainFunc() {
        // 14 ones                    0  1  2  3  4   5  6  7  8  9
        vector<vector<int>> image = {{0, 0, 0, 0, 0,  0, 0, 0, 0,0}, // 0
                                     {0, 0, 0, 0, 1,  1, 1, 0, 0,0}, // 1
                                     {0, 1, 0, 1, 0,  0, 0, 0, 1,0}, // 2
                                     {0, 1, 0, 0, 0,  0, 0, 0, 1,0}, // 3
                                     {0, 1, 0, 0, 0,  0, 0, 0, 1,0}, // 4
                                     {0, 1, 0, 0, 0,  0, 0, 0, 1,0}, // 5
                                     {0, 1, 0, 0, 0,  0, 0, 0, 0,0}, // 6
                                     {0, 0, 0, 0, 0,  0, 1, 1, 0,0}, // 7
                                     {0, 1, 0, 1, 1,  1, 0, 0, 0,0}, // 8
                                     {0, 0, 0, 0, 0,  0, 0, 0, 0,0}}; // 9


        int rows = image.size();
        int cols = image[0].size();

        int startR = -1;
        int startC = -1;
        int numberOnes = 0;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));

        /// number of '1'
        for (auto &i: image) {
            for (auto &ii:i)
                if (ii == 1)
                    numberOnes++;
        }
        int tmpOnes = numberOnes;
        cout<<"tmpOnes: "<<tmpOnes<<endl;



        /// fins the first '1' as (startR, startC)
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                // if '1'
                if (image[r][c] == 1) {
                    tmpOnes--;
                    visited[r][c] = true;
                    startR = r;
                    startC = c;
                    break;
                }
            }
            if (tmpOnes == numberOnes - 1)
                break;
        }


        ///
        int tailR = -1, tailC = -1;
        int headR = startR, headC = startC;

        cout<<"start: "<<headR<<", "<<headC<<endl;
        while(tmpOnes!=0){
            for (int i = 1; i < max(rows, cols); i++) {
                if (Scan(headR, headC, tailR, tailC, i, rows, cols, image, visited)) {
                    tmpOnes--;

                    drawPath(headR, headC, tailR, tailC, image, visited);
                    headR = tailR;
                    headC = tailC;
                    cout<<"(Main) new head: "<<headR<<","<<headC<<endl;
                    break;  ///!!!!!!(when found one break!!!!!)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
            }
        }

        /// the final tail
        cout<<"The final tail and head:"<<endl;
        cout<<"new tail: "<<tailR<<","<<tailC<<endl;
        cout<<"start: "<<startR<<","<<startC<<endl;

        /// connect head and tail
        drawPath(startR, startC, tailR, tailC, image, visited);

        for (auto& i: image){
            for (auto ii:i)
                cout<<ii<<", ";
            cout<<endl;
        }

    }


    /**
     * @brief: Scan points around head point(r,c) with diameter 1,2,3..., find the next black point: '1',
     *          When the next black point is found, set it as tail(tailR, tail C).
     * @param r: Current head R
     * @param c: Current head C
     * @param tailR:
     * @param tailC:
     * @param l: Diameters with central as current head
     * @param rows: The number of rows of the image
     * @param cols: The number of cols of the image
     * @param image: The object that is being processed
     * @param visited: A 2D vector to store flags of those black points. If this black point has been visited,
     *                  set the corresponding visited vector true, otherwise stay the initial value false
     * @return: If the next black point is found, return true, otherwise return false
     */
    bool Scan(int& r, int& c, int& tailR, int& tailC, int l, int rows, int cols,
            vector<vector<int>>& image, vector<vector<bool>>& visited){

        vector<vector<int>> move;
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

        int moveSize = move.size();
        int count =0;
        for (auto& step: move){

            if (image[step[0]][step[1]] != 1){
                count++;
                if (count == moveSize)
                    return false;  // not found
            }
            else if(image[step[0]][step[1]] == 1 && visited[step[0]][step[1]]==true){
                count++;
                if (count == moveSize)
                    return false;  // not found
            }
            else if(image[step[0]][step[1]] == 1 && visited[step[0]][step[1]]==false){
                // update head
                tailR = step[0];
                tailC = step[1];
                cout<<"(Scan) new tail: "<<tailR<<","<<tailC<<endl;
                visited[step[0]][step[1]] = true;
                return true;   // found one
            }
        }
        move.clear();
    }


    /**
     * @brief :Draw a path from head(hr, hc) to tail(tr,tc), set color and flag true for all points alone this path
     * @param hr: head R
     * @param hc: head C
     * @param tr: tail R
     * @param tc: tail C
     * @param image: The image that is being processed
     * @param visited: A 2D vector to store flags of those black points. If this black point has been visited,
     *                  set the corresponding visited vector true, otherwise stay the initial value false
     * @return: Nothing
     */
    void drawPath(int hr, int hc, int tr, int tc, vector<vector<int>>& image ,vector<vector<bool>>& visited){
        if (hr<tr) {
            for (int r = hr; r <= tr; r++) {
                image[r][hc] = 1;
                visited[r][hc] = true;
            }
        }
        else if (hr>tr){
            for (int r=tr; r<=hr; r++){
                image[r][hc] = 1;
                visited[r][hc] = true;
            }
        }
        if (tc>hc){
            for (int c=hc; c<=tc; c++){
                image[tr][c] = 1;
                visited[tr][c] = true;
            }
        }
        else if (tc<hc){
            for (int c=tc; c<=hc; c++){
                image[tr][c] = 1;
                visited[tr][c] = true;
            }
        }
        return;
    }


};




#endif //MISSION1SRC_HELPER_H
