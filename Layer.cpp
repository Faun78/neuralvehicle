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
        Layer(int inp,int out,string activ,bool randomize=true){
            innodes=out;
            ounodes=inp;
            weights.resize1(innodes,ounodes,randomize);
            detectfunc(activ);
            bias.resize1(innodes,1,randomize);
            
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
            Matrix inputs_t = Matrix::transpose(inputs);
            Matrix weights_deltas = Matrix::multiply(gradients,inputs_t);
            Matrix wh_t =Matrix::transpose(weights);
            Matrix hidden_errors = Matrix::multiply(wh_t,errorofuotput);
            weights.add(weights_deltas);
            bias.add(gradients);
            return hidden_errors;
        }
        void exportlayer(){
            cout<<actexport(activasion)<<" ";
            weights.print();
            bias.print();
        }
        static Layer importlayer(){
            Layer answ;
            string actfunc;
            cin>>actfunc;
            answ.detectfunc(actfunc);
            answ.weights=Matrix::import1();
            answ.bias=Matrix::import1();
            answ.innodes=answ.weights.rows;
            answ.ounodes=answ.weights.cols;
            return answ;
        }
        void mutate(double rate=0.1){
            weights.map1(mutate3,rate);
            bias.map1(mutate3,rate);
        }
    private:
        void detectfunc(string func){
            if(func=="Sigmoid"){
                activasion=sigmoid;
                deactivasion=dsigmoid;
            }else if(func =="Tanh"){
                activasion=tanh1;
                deactivasion=detanh;
            }else if(func =="SeLU"){
                activasion=SeLU;
                deactivasion=deSeLU;
            }
        } 
        
};