//
//  main.cpp
//  diphw2p2
//
//  Created by chen on 13/4/9.
//  Copyright (c) 2013年 chen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;
#define IMGL 512
double image_matrix[IMGL][IMGL]={0};  // 轉換圖片成為矩陣的陣列變數
double x_normalize[IMGL][IMGL] = {0};
double y_normalize[IMGL][IMGL] = {0};

double x_pron_normalize[IMGL][IMGL] = {0};
double y_pron_normalize[IMGL][IMGL] = {0};

double r_normalize[IMGL][IMGL] = {0};
double r_pron_normalize[IMGL][IMGL] = {0};
//    double angle_normalize[IMGL][IMGL] = {0};
double final_matrix[IMGL][IMGL]={0};

int main(int argc, const char * argv[])
{

    char c;
    fstream fin ;
    ofstream ofin("./fisheye_output.raw");
    fin.open("./lenna512.raw",ios::in);
//    double image_matrix[IMGL][IMGL]={0};  // 轉換圖片成為矩陣的陣列變數
//    double x_normalize[IMGL][IMGL] = {0};
//    double y_normalize[IMGL][IMGL] = {0};
//    
//    double x_pron_normalize[IMGL][IMGL] = {0};
//    double y_pron_normalize[IMGL][IMGL] = {0};
//    
//    double r_normalize[IMGL][IMGL] = {0};
//    double r_pron_normalize[IMGL][IMGL] = {0};
////    double angle_normalize[IMGL][IMGL] = {0};
//    double final_matrix[IMGL][IMGL]={0};
    int k = 0;
    int j = 0;
    
    if(!fin){    cout << "讀檔失敗" << endl;   }
    else{
        do
        {
            fin.get(c);
            image_matrix[j][k] = ((double)c);
            k = k + 1;
            if(k >= IMGL)
            {
                k = 0;
                j = j + 1;
                
            }
        }while(!fin.eof());
        cout<<"讀檔成功....."<<endl;
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
                x_normalize[j][k] = ((double)k)/ ((double)(IMGL-1)/2.0) - 1.0 ;
                y_normalize[j][k] = (((double)j)/((double)(IMGL-1)/2.0) - 1.0 )*(-1.0);
            }
            
        }
        cout<<"（x,y）normalize成功....."<<endl;
//        cout << y_normalize[511][511] << " : " << x_normalize[511][511] <<endl;
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
                r_normalize[j][k] = sqrt( pow(x_normalize[j][k],2) + pow(y_normalize[j][k],2) );
                r_pron_normalize[j][k] = ( r_normalize[j][k] / sqrt(2.0) )- ( 1 - sqrt(1.0-pow( (r_normalize[j][k] / sqrt(2.0)) ,2))) / 2.0 ;
                x_pron_normalize[j][k] = x_normalize[j][k] * r_pron_normalize[j][k] / r_normalize[j][k] ;
                x_pron_normalize[j][k] = ( x_pron_normalize[j][k] + 1.0 ) * ((double)(IMGL-1)/2.0);
                y_pron_normalize[j][k] = y_normalize[j][k] * r_pron_normalize[j][k] / r_normalize[j][k] ;
                y_pron_normalize[j][k] = ( y_pron_normalize[j][k]*(-1.0) + 1.0 ) * ((double)(IMGL-1)/2.0) ;
            }            
            
        }
        cout<<"極座標處理成功....."<<endl;
  //      cout << y_pron_normalize[511][510] << " : " << x_pron_normalize[511][510] <<endl;
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
                
                int temp_y = (int)y_pron_normalize[j][k];
                int temp_x = (int)x_pron_normalize[j][k];
//                cout << j <<" :: "<< k <<endl;
//                cout << temp_y << " : " << temp_x <<endl;
                final_matrix[temp_y][temp_x] = image_matrix[j][k];
            }
            
        }
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
    //            cout<<final_matrix[j][k]<<endl;
                ofin << (unsigned char)final_matrix[j][k];
            }
            
        }
        
        
        fin.close();
        ofin.close();
        
       cout<<"圖檔處理成功....."<<endl;
       cout<<"輸出檔名為 ： fisheye_output.raw"<<endl;

    
    
    }
    
    return 0;
}

