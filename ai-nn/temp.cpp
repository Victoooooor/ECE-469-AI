#include <iostream>
#include <fstream>
#include "./NN.h"
using namespace std;
int main(){
    int epoch=100;
    double lr=0.1;
    string temp;
    ifstream ini, ftrain;
    ofstream output;
    temp="./iris/my.init";
    ini.open(temp);
    temp.clear();
    temp="./iris/train.csv";
    ftrain.open(temp);
    temp.clear();
    temp="./iris/my.trained";
    output.open(temp);
    if(ini.is_open()&&ftrain.is_open()&&output.is_open()){
        my_NN *NN = new my_NN(ini);
        NN->train(ftrain,lr,epoch,output);
    } else  cerr << "File Errror" << endl;

    output.close();
    ifstream trained, ftest;
    temp="./iris/my.trained";
    trained.open(temp);
    temp.clear();
    temp="./iris/test.csv";
    ftest.open(temp);
    temp.clear();
    temp="./iris/my.results";
    output.open(temp);
    if(trained.is_open()&&ftest.is_open()&&output.is_open()){
        my_NN *NN = new my_NN(trained);
        NN->test(ftest,output);
    } else  cerr << "File Errror" << endl;

    return 0;
}