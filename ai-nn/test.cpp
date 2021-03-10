#include <iostream>
#include <fstream>
#include "./NN.h"
using namespace std;
int main(){
    string temp;
    ifstream trained, ftest;
    ofstream output;
    cout<<"Enter trained NN file: ";
    cin>> temp;
    trained.open(temp);
    temp.clear();
    cout<<"Enter test set: ";
    cin>>temp;
    ftest.open(temp);
    temp.clear();
    cout<<"Enter name for output file: ";
    cin>>temp;
    output.open(temp);
    if(trained.is_open()&&ftest.is_open()&&output.is_open()){
        my_NN *NN = new my_NN(trained);
        NN->test(ftest,output);
    } else  cerr << "File Errror" << endl;
    return 0;
}