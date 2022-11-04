#include "functions.cpp"
class Layer{
    public:
        double (*activasion)(double);
        double (*deactivasion)(double);
        Matrix weights;
        Matrix bias;
        int innodes;
        int ounodes;

        Layer(){
            
        }
        Layer(int inp,int out,double (*activasionin)(double),double (*deactivasionin)(double),bool randomize=true){
            weights.resize1(inp,out,randomize);
            activasion=activasionin;
            deactivasion=deactivasionin;
            innodes=inp;
            ounodes=out;
        }
        Matrix feed(Matrix inputs,bool activ=true){
            Matrix outputs =Matrix::multiply(weights,inputs);
            outputs.add(bias);
            if(activ){
                outputs.map1(activasion);
            }
            return outputs;
        }
        Matrix train(Matrix inputs,Matrix errorofuotput,double learningrate){
            Matrix outputs=feed(inputs,false);
            Matrix gradients = Matrix::map1(outputs,deactivasion);
            gradients.multiply(errorofuotput);
            gradients.multiply(learningrate);
            inputs = Matrix::transpose(inputs);
            Matrix weights_deltas = Matrix::multiply(gradients,inputs);
            Matrix wh_t =Matrix::transpose(weights);
            Matrix hidden_errors = Matrix::multiply(wh_t,errorofuotput);
            weights.add(weights_deltas);
            bias.add(gradients);
            return hidden_errors;
        } 
        
};