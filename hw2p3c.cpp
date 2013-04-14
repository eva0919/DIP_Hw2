//
//  main.cpp
//  diphw2p3c
//
//  Created by chen on 13/4/9.
//  Copyright (c) 2013年 chen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
using namespace std;
#define IMGL 512
int image_matrix[IMGL][IMGL] = {0};
int label = 0;
int label_matrix[IMGL][IMGL] = {0};
int label_same[100] ={0};



int make_label(int a[IMGL][IMGL],int row,int column);

int main(int argc, const char * argv[])
{

    char c;
    fstream fin ;
    ofstream ofin("./sample2L.raw");
    fin.open("./sample2M.raw",ios::in);
    int j = 0 ;
    int k = 0 ;
    int label_num = 1;
    for(int i = 0 ; i < 100 ; i ++)
    {
       label_same[i] = i;
    }
    for(int i = 0 ; i < IMGL ; i++)
    {
        for(int j = 0 ; j < IMGL ; j++){
            fin.get(c);
            image_matrix[i][j]=( ( (int)c ) + 256 )% 256 ;
        }
    }
    for(int i = 0 ; i < IMGL ; i++)
    {
        for(int j = 0 ; j < IMGL ; j++){
            if(image_matrix[i][j] == 0)
            {
                label_matrix[i][j] = 0 ;
            }
            else
            {
                label_matrix[i][j] = make_label(image_matrix,i,j);
            }
        }
    }
    for(int i = 0 ; i < IMGL ; i++)
    {
        for(int j = 0 ; j < IMGL ; j++){
            label_matrix[i][j] = label_same[label_matrix[i][j]];
        }
    }
    for(int i = 0 ; i < IMGL ; i++)
    {
        for(int j = 0 ; j < IMGL ; j++){
            if(label_matrix[i][j]==0)
            {
                 ofin << (unsigned char) 0;
            }
            else{
            ofin << (unsigned char) (label_matrix[i][j]*3+5);
            }
        }
    }
    for(int i = 1 ; i < 100 ; i ++)
    {
    
        if(label_same[i]!=label_same[i-1] && label_same[i]!=i)
        {
            label_num ++;
        }
    }
    cout<<label_num<<endl;
    cout<<"圖檔處理完成......"<<endl;
    return 0;
}

int make_label(int a[IMGL][IMGL],int row,int column)
{
    int b[4] = {0};
    int connect = 0 ;
    if( (row -1 >= 0) && (column-1 >=0) && a[row -1][column-1]>0 )
    {
        b[0] = label_matrix[row-1][column-1];
        connect ++ ;
    }
    if( (row -1 >= 0)  && a[row -1][column]>0 )
    {
        b[1] = label_matrix[row-1][column];
        connect ++ ;
    }
    if( (row -1 >= 0) && (column+1 < IMGL ) && a[row -1][column+1]>0 )
    {
        b[2] = label_matrix[row-1][column+1];
        connect ++ ;
    }
    if( (column-1 >= 0 ) && a[row][column-1]>0 )
    {
        b[3] = label_matrix[row][column-1];
        connect ++ ;
    }
    if(connect == 0)
    {
        label = label + 1 ;
        return label ;
    }
    else if (connect == 1)
    {
        for(int i = 0 ; i < 4 ; i ++)
        {
            if(b[i]!=0)
            {
                return b[i];
            }
        }
        label = label + 1 ;
        return label;
    
    }
    else
    {
        int label_max = 0;
        for(int i = 0 ; i < 4 ; i ++)
        {
            if(b[i]!=0)
            {
                if ( label_max == 0 )
                {
                    label_max = label_same[ b[i] ];
                }
                else if ( label_max > b[i] )
                {
                    label_max = label_same[ b[i] ];
                }
            }
        }
        for(int i = 0 ; i < 4 ; i ++)
        {
            if(b[i]!=0)
            {
                label_same[b[i]] = label_max;
            }
        }
        return label_max;
        
    }

}



