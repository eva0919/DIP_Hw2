//
//  main.cpp
//  diphw2p1
//
//  Created by chen on 13/4/1.
//  Copyright (c) 2013年 chen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;
#define IMGL 512
#define TS 50 // Thresholding
#define OUTFILE "sample1_1storder_50.raw"
#define K 1

int Unsharp(int a[IMGL][IMGL] ,int Row, int Column );
int low_pass(int a[IMGL][IMGL] ,int Row, int Column);

void Detection_2nd_Order(int a[IMGL][IMGL]);
int Laplacian(int a[IMGL][IMGL] ,int Row, int Column);
bool EdgeMap(int a[IMGL][IMGL],int Row, int Column );

int main(int argc, const char * argv[])
{

    
    char c;
    fstream fin ;
    ofstream ofin("./sample1_1storder_50.raw");
    fin.open("./sample1.raw",ios::in);
    int image_matrix[IMGL][IMGL]={0};  // 轉換圖片成為矩陣的陣列變數
    int edge_matrix[IMGL][IMGL]={0};
    int j = 0 , k = 0 ;
    int Gr = 0 , Gc = 0 ; // 算Row gradient 和 Column gradient 時的暫存變數
    if(!fin){    cout << "讀檔失敗" << endl;   }
    else{
        
        // do read image
        
    
        
        do
        {
            fin.get(c);
            image_matrix[j][k] = (((int)c+256)%256);
            k = k + 1;
            if(k >= IMGL)
            {
                k = 0;
                j = j + 1;
                
            }
        }while(!fin.eof());
    
        //Do 2nd Ordet
        Detection_2nd_Order(image_matrix);
        
        // do Unsharp Mask
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
                image_matrix[j][k] = Unsharp(image_matrix, j,k);
            }
            
        }
        
        
        
        // do 1st order 
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
                if( (k-1) < 0 || (j+1) >= IMGL || (j-1) < 0 || (k+1) >= IMGL)
                {
                    continue;
                }
                else{
                    Gr =(double)( image_matrix[j-1][k+1]+(K*image_matrix[j][k+1])+image_matrix[j+1][k+1]-image_matrix[j-1][k-1]-(K*image_matrix[j][k-1])-image_matrix[j+1][k-1] )/(double)(K+2);
                    Gc = (double)( image_matrix[j-1][k-1]+(K*image_matrix[j-1][k])+image_matrix[j-1][k+1]-image_matrix[j+1][k-1]-(K*image_matrix[j+1][k])-image_matrix[j+1][k+1] )/(double)(K+2);
                    edge_matrix[j][k] = sqrt( pow( Gr , 2 )+ pow( Gc , 2 ) );
                    if(edge_matrix[j][k] >= TS )
                    {
                        edge_matrix[j][k]= 255;
                    }
                    else{
                        edge_matrix[j][k]=0;
                    }
                
                }
            }
        
        }
    
        
        // do output to file
        for(j = 0 ; j < IMGL ; j++)
        {
            for(k = 0 ; k < IMGL ; k ++)
            {
                ofin << (unsigned char)edge_matrix[j][k];
            }
            
        }

        
        
        cout<<"圖檔處理成功....."<<endl;
        cout<<"輸出檔名為 ： "<<OUTFILE<<endl;
    
    }
    fin.close();
    ofin.close();
    return 0;
}


// This is unsharp mask function 用來做edge detection的前置處理
int Unsharp(int a[IMGL][IMGL] ,int Row, int Column )
{
    int result = 0;
    int Fl = low_pass(a, Row,Column);
    double c =  0.6;   //   Unsharp 中的參數 c
    //cout << " Matrix : " << a[Row][Column] <<endl ;
    //cout << " Fl :" << Fl <<endl;
    result = ( 3 ) * a[Row][Column] - ( 2 )*Fl ;
    return result;
}

// This is low pass filter function 
int low_pass(int a[IMGL][IMGL] ,int Row, int Column)
{
    int result = 0;
    int mask_L = 3 ; // low pass mask length
    int gap = (mask_L -1) / 2 ;
    int temp = 0 ;
    
    for(int i = Row - gap ; i <= Row + gap ; i ++){
        for(int j = Column -  gap ; j <= Column + gap ;  j ++)
        {
            //cout << " i : " << i <<"  j :" << j << endl;
            if( i < 0 || j < 0 || i >= IMGL || j >= IMGL)
            {
                continue;
            }
            else
            {
                temp = temp + a[i][j];
                
            }
        }
    }
    result = temp / 9 ;
    return result ;
}

// This is a function to process edge detection use 2nd order
void Detection_2nd_Order(int a[IMGL][IMGL])
{
    ofstream ofin("./sample1_2nd_Order.raw");
   // ofstream oftext("./histogram_2nd_order.txt");
    int edge_matrix[IMGL][IMGL]={0};
    int temp_matrix[IMGL][IMGL]={0};
    int G_pron[IMGL][IMGL] = { 0 };
    int histogram[256] = {0};
    
    // do Low-pass Filter
    for(int j = 0 ; j < IMGL ; j++)
    {
        for(int k = 0 ; k < IMGL ; k ++)
        {
            edge_matrix[j][k] = low_pass(a, j, k) ;
        }
        
    }
    
    for(int j = 0 ; j < IMGL ; j++)
    {
        for(int k = 0 ; k < IMGL ; k ++)
        {
            temp_matrix[j][k] = Laplacian(edge_matrix, j, k) ;
        }
        
    }
    
//    for(int j = 0 ; j < IMGL ; j++)
//    {
//        for(int k = 0 ; k < IMGL ; k ++)
//        {
//            ofin << (unsigned char)temp_matrix[j][k] ;
//            cout<<temp_matrix[j][k]<<endl;
//            histogram[ temp_matrix[j][k]+128 ] ++ ;
//        }
//        
//    }
//    for(int i = 0 ; i < 256 ; i ++)
//    {
//        oftext << histogram[i] << endl;
//    }
    for(int j = 0 ; j < IMGL ; j++)
    {
        for(int k = 0 ; k < IMGL ; k ++)
        {
           if (  temp_matrix[j][k] < 3 || temp_matrix[j][k] > (-3)  )
           {
               G_pron[j][k] = 0 ;
              // ofin <<(unsigned char) 0 ;
           }
           else
           {
               G_pron[j][k] = 1 ;
               //ofin <<(unsigned char) 255 ;
           }
        }
        
    }
    for(int j = 0 ; j < IMGL ; j++)
    {
        for(int k = 0 ; k < IMGL ; k ++)
        {
            if( G_pron[j][k]==0 && EdgeMap( temp_matrix, j, k) ){
                ofin <<(unsigned char) 255 ;
            }
            else{
                ofin << (unsigned char) 0 ;
            }
        }
        
    }
    
    cout<<"圖檔(2nd_Order)處理成功....."<<endl;
    cout<<"輸出檔名為 ： sample1_2nd_Order.raw"<<endl;
    ofin.close();
    
}

int Laplacian(int a[IMGL][IMGL],int Row, int Column)
{
    int result = 0 ;
    int a0,a1,a2,a3,a4,a5,a6,a7,a8;
    
    if(  (Row - 1) >= 0 && (Column - 1) >= 0 )
    {
        a0 = a[Row-1][Column-1];
    }
    else{
         a0 = a[Row+1][Column+1];
        
    }
    
    if(  (Row - 1) >= 0  )
    {
        a1 = a[Row-1][Column];
    }
    else{
        a1 = a[Row+1][Column];
        
    }
    
    if(  (Row - 1) >= 0 && (Column + 1) < IMGL )
    {
        a2 = a[Row-1][Column+1];
    }
    else{
        a2 = a[Row+1][Column-1];
        
    }
    
    if(   (Column - 1) >= 0 )
    {
        a3 = a[Row][Column-1];
    }
    else{
        a3 = a[Row][Column+1];
        
    }
    
    a4 = a[Row][Column];
    
    if(   (Column + 1) < IMGL )
    {
        a5 = a[Row][Column+1];
    }
    else{
        a5 = a[Row][Column-1];
        
    }
    
    if(  (Row + 1) < IMGL && (Column - 1) >= 0 )
    {
        a6 = a[Row+1][Column-1];
    }
    else{
        a6 = a[Row-1][Column+1];
        
    }
    
    if(  (Row + 1) < IMGL )
    {
        a7 = a[Row+1][Column];
    }
    else{
        a7 = a[Row-1][Column];
        
    }
    
    if(  (Row + 1) < IMGL && (Column + 1) < IMGL )
    {
        a8 = a[Row+1][Column+1];
    }
    else{
        a8 = a[Row-1][Column-1];
        
    }
    
    
    result =( (-2)*(a0+a2+a6+a8) +(a1+a3+a5+a7) +4*a4 ) / 8 ;
    
    
    return result ;
}


bool EdgeMap(int a[IMGL][IMGL],int Row, int Column )
{
    int num = 0;
    int num2 = 0;
    int temp_array[9] = {0};
    int index = 0;

    for(int i = Row -1 ; i < Row+2 ; i ++)
    {
        for(int j = Column -1 ; j < Column+2 ; j ++)
        {
            if( i < 0 || j < 0 || i >= IMGL || j > IMGL)
            {
                temp_array[index] = 0 ;
            }
            else{
//                if(a[i][j]-a[Row][Column] == 0)
//                {
//                    num ++;
//                }
//                if(a[i][j] == 1)
//                {
//                    num2 ++;
//                }
                temp_array[index] = a[i][j] ; 
            }
            index = index + 1 ;
        }

    
    }
        if(temp_array[0]*temp_array[8] < 0 ){num++;}
        if(temp_array[2]*temp_array[6] < 0 ){num++;}
        if(temp_array[1]*temp_array[7] < 0 ){num++;}
        if(temp_array[3]*temp_array[5] < 0 ){num++;}
        
//    cout<<num<<endl;
    if(num > 0 ){
    
        return true;
    }
    else{
    
        return false;
    }
    }



