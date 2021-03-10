#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


class my_NN {
private:
    class neuron;
    class link{
    public:
        double weight;
        neuron *l_n;
    };
    class neuron {
    public:
        double input;
        double error;
        double activation;
        vector<link> in_links;
        vector<link> out_links;
    };
    class train_data {
    public:
        vector<double> in;
        vector<int> out;
    };
    vector<int> l_size;
    vector<vector<neuron>> layers;//
    double sig(double x);//sigmod
    double sig_d(double x);//sigmod'
public:
    my_NN(ifstream &load);
    void train(ifstream &tr_data, double lr, int epoch,ostream &fout);
    void test(ifstream &ts_data, ofstream &fout);
};

