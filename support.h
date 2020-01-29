/*
 support.h
 Ben Siravantha
 
 Support header file including matrix, ppm, and pixel classes
 
 */

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

typedef unsigned char uchar;
extern const char ETX;

//1D representation of a 2D array for the image
template <typename T>
class matrix {
public:
    matrix(){ buf = NULL; }
    ~matrix(){
        delete [] buf;
    }
    
    void assign(int rows, int cols){
        Nrows = rows;
        Ncols = cols;
        
        buf = new T[Nrows*Ncols];
    }
    
    T *data(){ return buf; }
    
    int get_Nrows() const { return Nrows; }
    int get_Ncols() const { return Ncols; }
    
    T * operator[](int i){ return &buf[i*Ncols]; }
    
private:
    int Nrows, Ncols;
    T *buf;
};

//structure to hold pixel indices
struct pixel {
public:
    pixel(){ row = 0; col = 0; }
    pixel(int x, int y){ row = x; col = y; }
    int getRow(){ return row; }
    int getCol(){ return col; }
private:
    int row, col;
};

//a pixel with R, G, B values
struct RGB {
public:
    RGB(){ R = '0'; G = '0'; B = '0'; }
    RGB(char a, char b, char c){
        R = a;
        G = b;
        B = c;
    }
    uchar R, G, B;
};

//ppm class holding matrix of the ppm pixels
class ppm {
public:
    ppm(){}
    void read(string filename);
    void write(string filename);
    int get_Nrows(){ return img.get_Nrows(); }
    int get_Ncols(){ return img.get_Ncols(); }
    RGB * operator[](int i){ return img[i]; }
private:
    matrix<RGB> img;
};

//use for random number generation
class rnumgen{
public:
    rnumgen(int seed) { srand(seed);}
    
    void pdf(const vector<int> &v);
    int rand() const;
    
private:
    vector<float> F;
};

#endif
