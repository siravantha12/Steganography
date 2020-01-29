/*
 support.cpp
 Ben Siravantha
 
 Implementation of read and write functions for ppm images
 
 */
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <numeric>
using namespace std;

#include "support.h"

const char ETX = 0x3;

//create the output file name
string output_filename(string filename){
    filename.insert(filename.find('.'), "_wmsg2");
    return filename;
}

//reads in ppm data from a file
void ppm::read(string filename) {
    ifstream fin(filename.c_str());
    //check if file opened correctly
    string p6;
    int col, row, maxpixel;
    if(fin.is_open()){
        //read in header
        fin >> p6;
        fin >> col >> row >> maxpixel;
        if(p6 != "P6"){
            cerr << "Error: wrong file format. Please enter PPM file"<< endl;
            exit(0);
        }
        if(maxpixel != 255){
            cerr << "Error: max pixel value is not 255" << endl;
            exit (0);
        }
        
        //sets up matrix
        img.assign(row, col);
        
        //read in binary data
        while(fin.get() != '\n'){}
        fin.read((char*)img.data(), 3*row*col);
        
        
        if((3*row*col) < fin.gcount()){
            cerr << "Error: unexpected bytes in file"<< endl;
            exit(0);
        }
    }
    fin.close();
}

//writes ppm data to a file
void ppm::write(string filename) {
    //makes the output file name
    string outputfile = output_filename(filename);
    //output file
    ofstream fout(outputfile.c_str());
    int row = get_Nrows();
    int col = get_Ncols();
    if(fout.is_open()){
        //writes header
        fout << "P6" << endl;
        fout << col << " " << row << endl;
        fout << 255 << endl;
        
        //write pixel data
        fout.write((char*)img.data(), 3*row*col);
    }
    
    fout.close();
}


void rnumgen::pdf(const vector<int> &v){
    F.resize(v.size());
    partial_sum(v.begin(), v.end(), F.begin());
    transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(),*(F.end()-1)));
}

int rnumgen::rand() const{
    const float randnorm = RAND_MAX+1.0f;
    const float p = (float)std::rand()/randnorm;
    return upper_bound(F.begin(), F.end(), p) - F.begin();
}
