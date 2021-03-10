#include <iostream>
#include <iomanip>
#include <cmath>
#include "./NN.h"
#define l_num 3
using namespace std;
my_NN::my_NN(ifstream& load) {
    //load
    l_size.resize(l_num);
    layers.resize(l_num);
    for(int i=0; i < l_num; i++) {
        load>>l_size[i];
        layers[i].resize(++(l_size[i]));
    }
    for(int i = 0; i < l_num; i++) {
        layers[i][0].activation = -1;//using -1 bias
    }
    double temp;
    link in,out;
    for(int i=0; i<l_num-1; i++) {
        for(int j=1; j<l_size[i+1]; j++) {
            for(int k=0; k<l_size[i]; k++) {
                load>>temp;
                in.weight=temp;
                out.weight=temp;
                in.l_n=&layers[i][k];
                out.l_n=&layers[i+1][j];
                layers[i+1][j].in_links.push_back(in);
                layers[i][k].out_links.push_back(out);
            }
        }
    }
}

void my_NN::train(ifstream &tr_data,double lr,int epoch, ostream &fout) {
    //load
    fout<<setprecision(3)<<fixed;
    int cols,in_num,out_num;
    tr_data>>cols>>in_num>>out_num;
    const int out_id = l_num-1;
    vector<train_data> trains(cols);
    for(int i = 0; i < cols; i++) {
        trains[i].in.resize(in_num);
        trains[i].out.resize(out_num);
        for(int j=0; j < in_num; j++) {
            tr_data>>trains[i].in[j];
        }
        for(int j=0; j < out_num; j++) {
            tr_data>>trains[i].out[j];
        }
    }
    //back propogation
    for(int i=0;i<epoch;i++) {
        for(int j=0; j<cols; j++) {
            for(int k=0;k<in_num;k++){
                layers[0][k+1].activation=trains[j].in[k];//copy
            }
            for(int k = 1; k<l_num; k++) {
                for(int m = 1; m<l_size[k]; m++) {
                    layers[k][m].input=0;
                    for(auto &it:layers[k][m].in_links) {//c++11
                        layers[k][m].input+=it.weight*it.l_n->activation;
                    }
                    layers[k][m].activation=this->sig(layers[k][m].input);
                }
            }
            //error back prop
            for(int k=1;k<l_size[out_id];k++){
                layers[out_id][k].error=this->sig_d(layers[out_id][k].input)*(trains[j].out[k-1]-layers[out_id][k].activation);
            }
            double sum;
            for(int k=out_id-1;k>0;k--){
                for(int m=1; m<l_size[k]; m++){
                    sum=0;
                    for(auto &it:layers[k][m].out_links){
                        sum+=it.weight*it.l_n->error;
                    }
                    layers[k][m].error=sum*this->sig_d(layers[k][m].input);
                }
            }
            for(int k=1; k<l_num; k++){
                for(int m=1; m<l_size[k]; m++){
                    for(auto &it:layers[k][m].in_links){
                        it.weight=lr*it.l_n->activation*layers[k][m].error+it.weight;
                        it.l_n->out_links[m-1].weight=it.weight;
                    }
                }
            }
        }
    }
    int index=0;
    for (int i=0;i<l_num;i++) {
        if(index++) fout<<" ";
        fout<<this->l_size[i]-1;
    }
    fout<<endl;
    for(int i=1; i<l_num; i++){
        for(int j=1; j<this->l_size[i]; j++){
            index=0;
            for(auto &it:layers[i][j].in_links){
                if(index) fout<<" ";
                fout<<it.weight;
                index++;
            }
            fout<<endl;
        }
    }
}

void my_NN::test(ifstream& ts_data, ofstream& fout) {
    fout<<setprecision(3)<<fixed;
    int cols,in_num,out_num;
    ts_data>>cols>>in_num>>out_num;
    const int out_id=l_num-1;
    vector<train_data> tests(cols);
    vector<vector<double>> predic(out_num);//double for divisions at end of test
    for(int i=0;i<cols;i++){
        tests[i].in.resize(in_num);
        tests[i].out.resize(out_num);
        for(int j=0;j<in_num;j++){
            ts_data>>tests[i].in[j];
        }
        for(int j=0;j<out_num;j++){
            ts_data>>tests[i].out[j];
        }
    }
    for(int i=0;i<out_num;i++){
        predic[i].resize(4);
        for(int j=0;j<4;j++)    predic[i][j]=0;
    }
    
    for(int i=0;i<cols;i++){
        //copy
        for(int j=0;j<in_num;j++){
            layers[0][j+1].activation = tests[i].in[j];
        }
        //get predicted
        for(int j=1;j<l_num;j++){
            for(int k = 1; k<l_size[j]; k++){
                layers[j][k].input = 0;
                vector<link>::iterator it;
                for(auto &it:layers[j][k].in_links)
                    layers[j][k].input += it.weight*it.l_n->activation;
                layers[j][k].activation = this->sig(layers[j][k].input);
            }
        }
        //confusion matrix
        for(int j=0;j<(l_size[out_id]-1);j++){
            if(layers[out_id][j+1].activation>=0.5){//using >= as specified in pdf
                if(tests[i].out[j])
                    predic[j][0]++;//A
                else
                    predic[j][1]++;//B
            } 
            else{
                if(tests[i].out[j])
                    predic[j][2]++;//C
                else
                    predic[j][3]++;//D
            }
        }
    }
    double A=0,B=0,C=0,D=0;
    double accu, prec, recall, f1,avg_accu, avg_prec, avg_recall, avg_f1;
    for(int i=0; i<out_num; i++){
        A+=predic[i][0];
        B+=predic[i][1];
        C+=predic[i][2];
        D+=predic[i][3];
        accu=(predic[i][0]+predic[i][3])/(predic[i][0]+predic[i][1]+predic[i][2]+predic[i][3]);//(1)
        prec=predic[i][0]/(predic[i][0]+predic[i][1]);//(2)
        recall=predic[i][0]/(predic[i][0]+predic[i][2]);//(3)
        f1=(2*prec*recall)/(prec+recall);//(4)
        //avoid number err (nan)
        if(accu!=accu)  accu=0;
        if(prec!=prec)  prec=0;
        if(recall!=recall)  recall=0;
        if(f1!=f1)  f1=0;
        avg_accu+=accu;
        avg_prec+=prec;
        avg_recall+=recall;
        fout<<(int)predic[i][0]<<" "<<(int)predic[i][1]<<" "<<(int)predic[i][2]<<" "<<(int)predic[i][3]<<" "<<accu<<" "<<prec<<" "<<recall<<" "<<f1<<endl;
    }
    //micro
    accu =(A+D)/(A+B+C+D);
    prec =A/(A+B);
    recall=A/(A+C);
    f1=(2*prec*recall)/(prec+recall);
    if(accu!=accu) accu=0;
    if(prec!=prec) prec=0;
    if(recall!=recall) recall=0;
    if(f1!=f1) f1=0;
    //macro
    avg_accu/=out_num;
    avg_prec/=out_num;
    avg_recall/=out_num;
    avg_f1=(2*avg_prec*avg_recall)/(avg_prec+avg_recall);
    if(avg_f1!=avg_f1) avg_f1=0;

    fout<<accu<<" "<<prec<<" " <<recall<< " "<<f1<<endl;
    fout<<avg_accu<<" "<<avg_prec<<" "<<avg_recall<<" "<<avg_f1<<endl;
}

double my_NN::sig(double x) {
    return 1.0/(1.0+exp(-x));
}
double my_NN::sig_d(double x) {
    return this->sig(x)*(1.0-(this->sig(x)));
}