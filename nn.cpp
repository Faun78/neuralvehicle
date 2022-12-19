#include <iostream>
#include <fstream>
#include "Layer.cpp"
#include <random>
#include <algorithm>

#include <ctime>
#include <math.h> 
using namespace std;


class NeuralNetwork{
    public:
        int input_nodes;
        int hidden_nodes;
        int output_nodes;
        bool randomizer;
        double learningrate;
        Matrix weights_ih;
        Matrix weights_ho;
        Matrix bias_h;
        Matrix bias_o;
        double (*activasionh)(double);
        double (*deactivasionh)(double);
        double (*activasiono)(double);
        double (*deactivasiono)(double);
        NeuralNetwork(){
            
        }
        NeuralNetwork(int numI,int numH,int numO,bool randomize=true){
        input_nodes =numI;
        hidden_nodes=numH;
        output_nodes=numO;
        learningrate=0.14495;
        randomizer = randomize;
        activasionh=sigmoid;
        deactivasionh=dsigmoid;
        activasiono=sigmoid;
        deactivasiono=dsigmoid;
        weights_ih.resize1(hidden_nodes,input_nodes,randomize);
        weights_ho.resize1(output_nodes,hidden_nodes,randomize);
        bias_h.resize1(hidden_nodes,1,randomize);
        bias_o.resize1(output_nodes,1,randomize);
        }
        
        double* feedforward(double input[],double arr[]){
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden =Matrix::multiply(weights_ih,inputs);

            hidden.add(bias_h);
            hidden.map1(activasionh);
            Matrix output =Matrix::multiply(weights_ho,hidden);

            output.add(bias_o);
            output.map1(activasiono);
            return output.toarray(arr);
        }
        
        void train(double input[], double target[]){
            //FeedForward Start
            //creating input and hidden matrix
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden = Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_h);
            hidden.map1(activasionh);
            //creating outputs from hidden
            Matrix outputs =Matrix::multiply(weights_ho,hidden);
            outputs.add(bias_o);
            outputs.map1(activasiono);
            //FeedForwardEnd
            //creating targets Matrix
            Matrix targets=Matrix::fromarray(target,output_nodes);
            //calculating errors
            //ERROR =targets - outputs
            Matrix errorofuotput=Matrix::subtract(targets,outputs);
            //maping outputs problems
            outputs =Matrix::multiply(weights_ho,hidden);
            outputs.add(bias_o);
            Matrix gradients = Matrix::map1(outputs,deactivasiono);
            gradients.multiply(errorofuotput);
            gradients.multiply(learningrate);//TO DO learning_rate
            
            //transposing hidden for next multiplication if this was bad i cannot multiply matrixes
            Matrix hidden_T = Matrix::transpose(hidden);
            //creating corrections 
            Matrix weight_ho_deltas = Matrix::multiply(gradients,hidden_T);
            
            //Starting backpropagations https://en.wikipedia.org/wiki/Backpropagation
            Matrix who_t =Matrix::transpose(weights_ho);
            Matrix hidden_errors = Matrix::multiply(who_t,errorofuotput);
            //corecting bias and weights of hidden 
            weights_ho.add(weight_ho_deltas);
            bias_o.add(gradients);
            
            //Correcting Inputs hidden nodes

            hidden = Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_h);
            Matrix hidden_gradient = Matrix::map1(hidden,deactivasionh);
            hidden_gradient.multiply(hidden_errors);
            hidden_gradient.multiply(learningrate);

            Matrix inputs_t = Matrix::transpose(inputs);
            Matrix weight_ih_deltas =Matrix::multiply(hidden_gradient,inputs_t);

            weights_ih.add(weight_ih_deltas); 
            bias_h.add(hidden_gradient);
            return;

        }
        void exportBrain(){
            cout<<input_nodes<<" "<<hidden_nodes<<" "<<output_nodes<<endl;
            weights_ih.print();
            weights_ho.print();
            bias_h.print();
            bias_o.print();
            return;
        }
        
        void mutate(double rate){
            weights_ih.map1(mutate3,rate);
            weights_ho.map1(mutate3,rate);
            bias_h.map1(mutate3,rate);
            bias_o.map1(mutate3,rate);
        }
        static NeuralNetwork importbrain(){
            int input1,hiden1,output1;cin>>input1>>hiden1>>output1;
            NeuralNetwork a(input1,hiden1,output1,false);
            a.weights_ih= Matrix::import1();
            a.weights_ho= Matrix::import1();
            a.bias_h= Matrix::import1();
            a.bias_o= Matrix::import1();
            return a;
        }
        
        double feederror(double input[],double target[]){
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden =Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_h);
            hidden.map1(sigmoid);
            Matrix output =Matrix::multiply(weights_ho,hidden);
            output.add(bias_o);
            output.map1(sigmoid);
            Matrix targets=Matrix::fromarray(target,output_nodes);
            Matrix errorofuotput=Matrix::subtract(targets,output);
            //arr=errorofuotput.toarray(arr);
            double answ=0;
            for(int i=0;i<errorofuotput.rows;i++){
                answ+=abs(errorofuotput.data[i][0]);
            }
            return answ;
        }
        
        static NeuralNetwork mutate4(NeuralNetwork b,double rate=0.1){
            NeuralNetwork a =b;
            a.weights_ih.map1(mutate3,rate);
            a.weights_ho.map1(mutate3,rate);
            a.bias_h.map1(mutate3,rate);
            a.bias_o.map1(mutate3,rate);
            return a;
        }
};
class NeuralNetworkLayers{
    public:
        double learningratelayer;
        vector<Layer>layers;
        NeuralNetworkLayers(){
            learningratelayer=0.003595;
        }
        void addlayer(int inp,int out,string activ,bool randomize=true){
            layers.push_back(Layer(inp,out,activ,randomize));
        }
        void exportBrainLayer(){
            cout<<learningratelayer<<endl;
            cout<<layers.size()<<endl;
            for(Layer l : layers){
                l.exportlayer();
            }
            return;
        }
        static NeuralNetworkLayers importbrainLayer(){
            double learning1;cin>>learning1;
            NeuralNetworkLayers a=NeuralNetworkLayers();
            a.learningratelayer=learning1;
            int temp;
            cin>>temp;
            
            for(int i=0;i<temp;i++){
                a.layers.push_back(Layer::importlayer());
            }
            return a;
        }
        double feederrorlayer(double input[],double target[]){
            Matrix temp = Matrix::fromarray(input,layers[0].ounodes);
            for(int i=0;i<layers.size();i++){
                temp=(layers[i].feed(temp));
            }
            Matrix targets=Matrix::fromarray(target,temp.rows);
            Matrix errorofuotput=Matrix::subtract(targets,temp);
            //arr=errorofuotput.toarray(arr);
            double answ=0;
            for(int i=0;i<errorofuotput.rows;i++){
                answ+=abs(errorofuotput.data[i][0]);
            }
            return answ;
        }
        
        static NeuralNetworkLayers mutate4(NeuralNetworkLayers b,double rate=0.1){
            NeuralNetworkLayers a =b;
            for(int i=0;i<a.layers.size();i++){
                a.layers[i].mutate(rate);
            }
            return a;
        }
        double* feedforwardLayer(double input[],double arr[]){
            Matrix temp = Matrix::fromarray(input,layers[0].ounodes);
            for(int i=0;i<layers.size();i++){
                temp=(layers[i].feed(temp));
            }
            return temp.toarray(arr);
        }
        void trainLayer(double input[], double target[]){
            Matrix inputs = Matrix::fromarray(input,layers[0].ounodes);
            vector<Matrix>temp;
            temp.push_back(inputs);
            for(int i=0;i<layers.size();i++){
                temp.push_back(layers[i].feed(temp[i]));
            }
            Matrix targets=Matrix::fromarray(target,layers[layers.size()-1].innodes);
            Matrix errorofuotput=Matrix::subtract(targets,temp[temp.size()-1]);
            vector<Matrix>temp2;
            temp2.push_back(errorofuotput);
            for(int i=layers.size()-1;i>=0;i--){
                temp2.push_back(layers[i].train(temp[i],temp2[temp2.size()-1],learningratelayer));
            }
        }
};