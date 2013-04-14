//
//  main.cpp
//  diphw2p3a
//
//  Created by chen on 13/4/9.
//  Copyright (c) 2013年 chen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;
#define IMGL 512
int image_matrix[IMGL][IMGL] = {0};
int final_matrix[IMGL][IMGL] = {0};
int temp_matrix[IMGL][IMGL] = {0};


int extract_boundary(int a[IMGL][IMGL],int row,int column);

int main(int argc, const char * argv[])
{
    
    char c;
    fstream fin ;
    ofstream ofin("./sample2M2.raw");
    fin.open("./sample2.raw",ios::in);
    int j = 0 ;
    int k = 0 ;
    int min = 0;
    int max = 0;
    do
    {
        fin.get(c);
        
        image_matrix[j][k] = (((int)c+256)%256);
        
        if(k == 0 && j == 0 )
        {
            min = image_matrix[j][k];
            max = image_matrix[j][k];
            
        }
        else
        {
            if (image_matrix[j][k] < min ) {min = image_matrix[j][k];}
            if (image_matrix[j][k] > max ) {max = image_matrix[j][k];}
            
        }
        k = k + 1;
        if(k >= IMGL)
        {
            k = 0;
            j = j + 1;
            
        }
    }while(!fin.eof());
    int gap = ( min + max ) / 2 ;
    
    for(j = 0 ; j < IMGL ; j++)
    {
        for(k = 0 ; k < IMGL ; k ++)
        {
            if( image_matrix[j][k] > 10 )
            {
                final_matrix[j][k] = 0;
            }
            else
            {
                final_matrix[j][k] = 1;
            }
        }
        
    }
    for(j = 0 ; j < IMGL ; j++)
    {
        for(k = 0 ; k < IMGL ; k ++)
        {
            temp_matrix[j][k] = extract_boundary(final_matrix, j, k);
        }
        
    }
    for(j = 0 ; j < IMGL ; j++)
    {
        for(k = 0 ; k < IMGL ; k ++)
        {
            final_matrix[j][k] = final_matrix[j][k] - temp_matrix[j][k];
        }
        
    }
    for(j = 0 ; j < IMGL ; j++)
    {
        for(k = 0 ; k < IMGL ; k ++)
        {
            if(final_matrix[j][k] == 1 ){
                ofin << (unsigned char) 255;
            }
            else{
                ofin << (unsigned char) 0;
            }
        }
        
    }
    ofin.close();
    fin.close();
    
    cout<<"圖檔處理成功....."<<endl;
    
    return 0;
}

int extract_boundary(int a[IMGL][IMGL],int row,int column){
    int result = 0 ;
    int mask_L = 3 ; // low pass mask length
    int gap = (mask_L -1) / 2 ;
    int temp = 0 ;
    
    for(int i = row - gap ; i <= row + gap ; i ++){
        for(int j = column -  gap ; j <= column + gap ;  j ++)
        {
            //cout << " i : " << i <<"  j :" << j << endl;
            if( i < 0 || j < 0 || i >= IMGL || j >= IMGL)
            {
                continue;
            }
            else
            {
                if(a[i][j] == 0)
                {
                    temp ++ ;
                }
                
            }
        }
    }
    if(temp > 0){ result = 0; }
    else{result=1;}
    
    return result ;
}