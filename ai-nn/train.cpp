#include <iostream>
#include <fstream>
#include "./NN.h"
using namespace std;
int main(){
    int epoch;
    double lr;
    string temp;
    ifstream ini, ftrain;
    ofstream output;
    cout<<"Enter intial weight file: ";
    cin>> temp;
    ini.open(temp);
    temp.clear();
    cout<<"Enter training set: ";
    cin>>temp;
    ftrain.open(temp);
    temp.clear();
    cout<<"Enter name for trained output file: ";
    cin>>temp;
    output.open(temp);
    cout<<"Enter Epochs: ";
    cin>>epoch;
    cout << "Enter learning rate: ";
    cin >>lr;
    if(ini.is_open()&&ftrain.is_open()&&output.is_open()){
        my_NN *NN = new my_NN(ini);
        NN->train(ftrain,lr,epoch,output);
    } else  cerr << "File Errror" << endl;
    return 0;
}