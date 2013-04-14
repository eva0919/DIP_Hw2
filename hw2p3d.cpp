//
//  main.cpp
//  diphw2p3d
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
int label_number[100] ={0};
int test_matrix[IMGL][IMGL][3];
int label_index = 0;
int isNewLabel(int data);
int label_array[256]={0};
int total_label = 0 ;
int label_normalize[10] ={0};
int label_tag = 0 ;
void normalize(int data);
int normalization(int data);
class LabelStack
{
public:
    int label;
    int label_num;
    int label_same;
    LabelStack(){
        label = 0 ;
        label_num = 0;
        label_same = 0;
    
    }
    LabelStack(int L,int num){
        label = L;
        label_num = num;
        label_same = label ;
    }
    void isLabelSame(int number,int L)
    {
        double temp =  (double)label_num / (double)number ;
        if( temp > 0.92 && temp < 1.08 )
        {
            label_same = L ;
            total_label -- ;
            normalize(label_same) ;
        }
    }
    void setNum(int number)
    {
        label_num = number;
    }
    
    void setLabel(int L)
    {
        label = L;
        label_same = label ;
    }
    
    int getLabel()
    {
        return label_same ;
    }
    int getNumber()
    {
        return label_num ;
    }
    
    int getLabel_2()
    {
        return label ;
    }
    
};

LabelStack stack[10];

int main(int argc, const char * argv[])
{

    char c;
    fstream fin ;
    ofstream ofin("./sample2R.raw");
    fin.open("./sample2L.raw",ios::in);
    int coins_num = 0 ;
    
    
    for(int i = 0 ; i < IMGL ; i++)
    {
        for(int j = 0 ; j < IMGL ; j++){
            fin.get(c);
            image_matrix[i][j]=( ( (int)c ) + 256 )% 256 ;
            label_array[image_matrix[i][j]]++;
//            if( image_matrix[i][j]!=0 && isNewLabel(image_matrix[i][j]) )
//            {
//                stack[label_index].setLabel(image_matrix[i][j]);
//                coins_num++;
//                
//            }
        }
    }
    
    for(int j = 1 ; j < 256; j ++)
    {
        if( label_array[j] > 5 ){
            stack[coins_num].setLabel(j);
            stack[coins_num].setNum(label_array[j]);
            total_label++;
            coins_num++;
            
        }
    }
    for(int i = 0 ; i < 10 ; i ++)
    {
        for(int j = i + 1 ; j < 10 ; j ++)
        {
            stack[i].isLabelSame(stack[j].getNumber(), stack[j].getLabel());
        }
    }
    for(int i = 0 ; i < IMGL ; i++)
    {
        for(int j = 0 ; j < IMGL ; j++){
            if(image_matrix[i][j] == 0)
            {
                ofin << (unsigned char) 0 ;
            }
            else {
            ofin << (unsigned char) (normalization(isNewLabel(image_matrix[i][j])) ) ;
            }
        }
    }
    
    cout<<"圖檔處理完成......"<<endl;
    cout<<"硬幣總共有 ： "<<coins_num<<"  枚"<<endl;
    cout<<"硬幣總共有 ： "<<total_label<<"  種"<<endl;
    return 0;
}

int isNewLabel(int data){

    for(int i = 0 ; i < 10 ; i ++)
    {
        if(stack[i].getLabel_2() == data)
        {
            return stack[i].getLabel();
        }
    }

    return 0;
}
void normalize(int data){
    bool d = false ;
    for(int i = 0 ; i < 10 ; i ++)
    {
        if(label_normalize[i] == data)
        {
            d = true;
        }
    }
    if (d==false)
    {
        label_normalize[label_tag] = data ;
        label_tag ++ ;
    }
    
}
int normalization(int data){
    for(int i = 0 ; i < 10 ; i ++)
    {
        if(label_normalize[i] == data)
        {
            return ( (i+1) * 256 / (total_label+1) );
        }
    }
    return 0 ;
    
}