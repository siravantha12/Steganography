/*
 Crypto.cpp
 Ben Siravantha
 
 Driver code to read in command line arguments to decide encode or decode
 with ppm file
 
 */
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

#include "support.h"

//sets what pixels to alter/encode the message into
void set_pixel_list(ppm &img, vector<pixel> &a){
    //build histogram of pixels to see which color occurs most
    vector<int> histogram;
    histogram.resize(4096);
    for(int i = 0; i < img.get_Nrows(); i++){
        for(int j = 0; j < img.get_Ncols(); j++){
            //make 12 bit color to put in histogram
            int color = ((img[i][j].R>>4)<<8) | ((img[i][j].G>>4)<<4) | ((img[i][j].B>>4));
            histogram[color]++;
            //add even number indices to pixel vector
            if(i%2 == 0 && j%2 == 0){
                pixel x(i,j);
                a.push_back(x);
            }
        }
    }
    
    //random number generation
    rnumgen RNG(0);
    RNG.pdf(histogram);
    //12 bit random numbers
    int rand1 = RNG.rand();
    int rand2 = RNG.rand();
    //24 bit random number
    int r24 = (rand1<<12)|rand2;
    
    //transforming and randomizing pixel vector
    for(int k = a.size()-1; k > 0; --k){
        swap(a[k], a[r24 % (k+1)]);
    }
}

//places bits of the message into pixels of the ppm file
void encode(string filename, ppm &img) {
    ifstream fin(filename.c_str());
    vector<pixel> a;
    set_pixel_list(img, a);
    int color = 0;
    string message;
    char c;
    
    while(cin.get(c)){
        message += c;
    }
    
    message += ETX;
    //pixel vector index
    int p = 0;
    //go through each character in the message
    for(unsigned int i = 0; i < message.size(); i++){
        for(int j = 0; j < 8; j++){
            //get index of matrix to alter
            int x = a[p].getRow();
            int y = a[p].getCol();
            //extract least significant bit of current character
            char bit = (message[i] >> j) & 0x1;
            //choose color of pixel to alter
            if(color == 0){
                img[x][y].R &= 0xFE;
                img[x][y].R |= bit;
                color++;
            }
            else if(color == 1){
                img[x][y].G &= 0xFE;
                img[x][y].G |= bit;
                color++;
            }
            else if(color == 2){
                img[x][y].B &= 0xFE;
                img[x][y].B |= bit;
                color = 0;
            }
            p++;
        }
    }
    
    //write to output file
    img.write(filename);
    
}

//takes a ppm file and gets a message out of the pixels
void decode(ppm &img) {
    vector<pixel> a;
    set_pixel_list(img, a);
    int color = 0;
    int p = 0;
    string message;
    
    while(1){
        char c = 0;
        //loop through 8 bits in the char
        for(int i = 0; i < 8; i++){
            int x = a[p].getRow();
            int y = a[p].getCol();
            char bit;
            //loop through each color throughout the pixels
            if(color == 0){
                //extracting LSB
                bit = img[x][y].R & 0x1;
                color++;
            }
            else if(color == 1){
                bit = img[x][y].G & 0x1;
                color++;
            }
            else if(color == 2){
                bit = img[x][y].B & 0x1;
                color = 0;
            }
            p++;
            //shift bit spot
            bit = (bit<<i);
            //add bit to char
            c |= bit;
        }
        message += c;
        
        //once ETX char is read, done decoding
        if(c == ETX) break;
    }
    cout << message << endl;
    
}

int main(int argc, char *argv[]) {
    string choice;
    string filename;
    //parse command line arguments
    if(argc != 3){
        cerr << "Usage: ./crypto -encode|-decode image.ppm"<< endl;
        return -1;
    }
    choice = argv[1];
    filename = argv[2];
    
    //looks for encode or decode
    if(choice != "-encode" && choice != "-decode"){
        cerr << "Usuage: ./crypto -encode|-decode image.ppm"<< endl;
        return -1;
    }
    
    ifstream fin(filename.c_str());
    
    ppm img;
    img.read(filename);

    if (choice == "-encode") encode(filename, img);
    else if (choice == "-decode") decode(img);
}
