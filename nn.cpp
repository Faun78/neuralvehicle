//TODO:create more activasion function
//TODO: more hidden layers 
//TODO: #1 Figure out why is NN with bias not working at all and without bias working correctly  
#include <iostream>
#include <fstream>
#include "matrix.cpp"
#include <random>
#include <ctime>
using namespace std;

mt19937 mt3(time(0));

double sigmoid(double x){
    return (1/(1+exp(-x)));
}
double dsigmoid2(double y){
    //return sigmoid(x)*(1-sigmoid(x));
    return y*(1-y);
}
double dsigmoid(double x){
    return sigmoid(x)*(1-sigmoid(x));
    //return y*(1-y);
}
double randval(int min=0,int max=1){
    static std::uniform_real_distribution<> dis(min, max); // rage 0 - 1
    return dis(mt3);
    }
double mutate3(double z,double rate){
    double ran =randval(-1,1);
    //cerr<<ran<<endl;
    if(ran<rate){
    //cerr<<"Mutated\n";
    return z+randval(-1,1);
    }else{
    return z;
    }}
class NeuralNetwork{
    public:
        int input_nodes;
        int hidden_nodes;
        int output_nodes;
        bool novector;
        double learningrate;
        Matrix weights_ih;
        Matrix weights_ho;
        Matrix bias_h;
        Matrix bias_o;
        NeuralNetwork(){
            
        }
        NeuralNetwork(int numI,int numH,int numO,bool vec=true){
        input_nodes =numI;
        hidden_nodes=numH;
        output_nodes=numO;
        learningrate=0.1;
        novector = vec;
        weights_ih.resize1(hidden_nodes,input_nodes,vec);
        weights_ho.resize1(output_nodes,hidden_nodes,vec);
        bias_h.resize1(hidden_nodes,1,vec);//check if i need to randomize it //CHECKED I NEED//
        bias_o.resize1(output_nodes,1,vec);//check if i need to randomize it //CHECKED I NEED//
        }
         double* feedforward(double input[],double arr[]){
            //Generate Hidden layer
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            // cerr<<"inputs done\n";
            Matrix hidden =Matrix::multiply(weights_ih,inputs);
            // cerr<<"hidden done\n";

            hidden.add(bias_h);
            hidden.map1(sigmoid);
            Matrix output =Matrix::multiply(weights_ho,hidden);
            // cerr<<"output done\n";

            output.add(bias_o);
            output.map1(sigmoid);
            // cerr<<"returning done\n";
            // cerr<<output.cols<<endl;
            return output.toarray(arr);
        }
        void train(double input[], double target[]){
            //FeedForward Start
            
            //creating input and hidden matrix
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden = Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_h);
            hidden.map1(sigmoid);
            //creating outputs from hidden
            Matrix outputs =Matrix::multiply(weights_ho,hidden);
            outputs.add(bias_o);
            outputs.map1(sigmoid);
            
            //FeedForwardEnd
            
            //creating targets Matrix
            Matrix targets=Matrix::fromarray(target,output_nodes);
            //calculating errors
            //ERROR =targets -outputs
            Matrix errorofuotput=Matrix::subtract(targets,outputs);
            //maping outputs problems

            Matrix gradients = Matrix::map1(outputs,dsigmoid2);
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

            
            Matrix hidden_gradient = Matrix::map1(hidden,dsigmoid2);
            hidden_gradient.multiply(hidden_errors);
            hidden_gradient.multiply(learningrate);

            Matrix inputs_t = Matrix::transpose(inputs);
            Matrix weight_ih_deltas =Matrix::multiply(hidden_gradient,inputs_t);

            weights_ih.add(weight_ih_deltas); 
            bias_h.add(hidden_gradient);
        //done
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
            //weights_hh.map1(mutate3,rate);
            weights_ho.map1(mutate3,rate);
            //bias_hh.map1(mutate3,rate);
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
        double* feederror(double input[],double target[],double arr[]){
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden =Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_h);
            hidden.map1(sigmoid);
            Matrix output =Matrix::multiply(weights_ho,hidden);
            output.add(bias_o);
            output.map1(sigmoid);
            Matrix targets=Matrix::fromarray(target,output_nodes);
            Matrix errorofuotput=Matrix::subtract(targets,output);
            return errorofuotput.toarray(arr);


        }
        static NeuralNetwork mutate4(NeuralNetwork b,double rate=0.1){
            NeuralNetwork a =b;
            a.weights_ih.map1(mutate3,rate);
            //weights_hh.map1(mutate3,rate);
            a.weights_ho.map1(mutate3,rate);
            //bias_hh.map1(mutate3,rate);
            a.bias_h.map1(mutate3,rate);
            a.bias_o.map1(mutate3,rate);
            return a;
        }
};


class NeuralNetwork2{
    public:
        int input_nodes;
        int hidden_nodes;
        int hiddenH_nodes;
        int output_nodes;
        bool novector;
        double learningrate;
        Matrix weights_ih;
        Matrix weights_hh;
        Matrix weights_ho;
        Matrix bias_h;
        Matrix bias_hh;
        Matrix bias_o;
        NeuralNetwork2(int numI,int numH,int numHH,int numO,bool vec=true){
        input_nodes =numI;
        hidden_nodes=numH;
        hiddenH_nodes=numHH;
        output_nodes=numO;
        learningrate=0.1;
        novector = vec;
        weights_ih.resize1(hiddenH_nodes,input_nodes,vec);
        weights_hh.resize1(hidden_nodes,hiddenH_nodes,vec);
        weights_ho.resize1(output_nodes,hidden_nodes,vec);
        bias_h.resize1(hidden_nodes,1,vec);//check if i need to randomize it //CHECKED I NEED//
        bias_hh.resize1(hiddenH_nodes,1,vec);//check if i need to randomize it //CHECKED I NEED//
        bias_o.resize1(output_nodes,1,vec);//check if i need to randomize it //CHECKED I NEED//
        }
        NeuralNetwork2(){
            
        }
         double* feedforward(double input[],double arr[]){
            //Generate Hidden layer
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden =Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_hh);
            hidden.map1(sigmoid);
            Matrix hiddenO =Matrix::multiply(weights_hh,hidden);
            hiddenO.add(bias_h);
            hiddenO.map1(sigmoid);
            Matrix output =Matrix::multiply(weights_ho,hiddenO);
            output.add(bias_o);
            output.map1(sigmoid);
            return output.toarray(arr);
        }
        double* feederror(double input[],double target[],double arr[]){
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden =Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_hh);
            hidden.map1(sigmoid);
            Matrix hiddenO =Matrix::multiply(weights_hh,hidden);
            hiddenO.add(bias_h);
            hiddenO.map1(sigmoid);
            Matrix output =Matrix::multiply(weights_ho,hiddenO);
            output.add(bias_o);
            output.map1(sigmoid);
            Matrix targets=Matrix::fromarray(target,output_nodes);
            Matrix errorofuotput=Matrix::subtract(targets,output);
            return errorofuotput.toarray(arr);

        }
        void train(double input[], double target[]){
            //FeedForward Start
            //cerr<<"Starting FeedForward\n";
            //creating input and hidden matrix
            Matrix inputs = Matrix::fromarray(input,input_nodes);
            Matrix hidden = Matrix::multiply(weights_ih,inputs);
            hidden.add(bias_hh);
            hidden.map1(sigmoid);
            Matrix hiddenO =Matrix::multiply(weights_hh,hidden);
            hiddenO.add(bias_h);
            hiddenO.map1(sigmoid);
            //creating outputs from hidden
            Matrix outputs =Matrix::multiply(weights_ho,hiddenO);
            outputs.add(bias_o);
            outputs.map1(sigmoid);
            //cerr<<"Ending FeedForward\n";
            
            //FeedForwardEnd
            //cerr<<"Starting backpropagations\n";
            
            //creating targets Matrix
            Matrix targets=Matrix::fromarray(target,output_nodes);
            //calculating errors
            //ERROR =targets -outputs
            Matrix errorofuotput=Matrix::subtract(targets,outputs);
            //maping outputs problems

            Matrix gradients = Matrix::map1(outputs,dsigmoid2);
            gradients.multiply(errorofuotput);
            gradients.multiply(learningrate);//TO DO learning_rate
            
            //transposing hidden for next multiplication if this was bad i cannot multiply matrixes
            Matrix hiddenO_T = Matrix::transpose(hiddenO);
            //creating corrections 
            Matrix weight_ho_deltas = Matrix::multiply(gradients,hiddenO_T);
            
            //Starting backpropagations https://en.wikipedia.org/wiki/Backpropagation
            Matrix who_t =Matrix::transpose(weights_ho);
            Matrix hidden_errors = Matrix::multiply(who_t,errorofuotput);
            //corecting bias and weights of hidden 
            weights_ho.add(weight_ho_deltas);
            bias_o.add(gradients);
            //Correcting Hidden Hidden nodes
            Matrix hiddenO_gradient = Matrix::map1(hiddenO,dsigmoid2);
            hiddenO_gradient.multiply(hidden_errors);
            hiddenO_gradient.multiply(learningrate);
            Matrix hidden_T =  Matrix::transpose(hidden);
            hidden_T.map1(dsigmoid2);
            Matrix weight_hh_deltas =Matrix::multiply(hiddenO_gradient,hidden_T);
             Matrix whh_t=Matrix::transpose(weights_hh);
            Matrix hidden_errors2 = Matrix::multiply(whh_t,hidden_errors);
            weights_hh.add(weight_hh_deltas); 
            bias_h.add(hiddenO_gradient);
            //Correcting Inputs hidden nodes

            Matrix hidden_gradient = Matrix::map1(hidden,dsigmoid2);
            hidden_gradient.multiply(hidden_errors2);
            hidden_gradient.multiply(learningrate);

            Matrix inputs_t = Matrix::transpose(inputs);
            Matrix weight_ih_deltas =Matrix::multiply(hidden_gradient,inputs_t);

            weights_ih.add(weight_ih_deltas); 
            bias_hh.add(hidden_gradient);
        //done
        return;
        }
        static NeuralNetwork2 mutate4(NeuralNetwork2 b,double rate){
            NeuralNetwork2 a =b;
            a.weights_ih.map1(mutate3,rate);
            a.weights_hh.map1(mutate3,rate);
            a.weights_ho.map1(mutate3,rate);
            a.bias_hh.map1(mutate3,rate);
            a.bias_h.map1(mutate3,rate);
            a.bias_o.map1(mutate3,rate);
        return a;
        }

        void exportBrain(){
            cout<<input_nodes<<" "<<hidden_nodes<<" "<<hiddenH_nodes<<" "<<output_nodes<<endl;
            weights_ih.print();
            weights_hh.print();
            weights_ho.print();
            bias_hh.print();
            bias_h.print();
            bias_o.print();
            return;
        }
        static NeuralNetwork2 importbrain(){
            int input1,hiden1,hidden2,output1;cin>>input1>>hiden1>>hidden2>>output1;
            NeuralNetwork2 a(input1,hiden1,hidden2,output1,false);
            a.weights_ih= Matrix::import1();
            a.weights_hh= Matrix::import1();
            a.weights_ho= Matrix::import1();
            a.bias_hh= Matrix::import1();
            a.bias_h= Matrix::import1();
            a.bias_o= Matrix::import1();
            return a;
        }
       
};