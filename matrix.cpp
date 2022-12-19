#include <iostream>
#include <ctime>
#include <random>
#include <vector>
using namespace std;
mt19937 mt2(time(0));
class Matrix{
    public:
        int rows;
        int cols;
        vector<vector<double>> data;
        Matrix(){
        }
        Matrix(int rows1,int cols1,bool dorandom=true){
            rows=rows1;
            cols=cols1;
            data.resize(rows, vector<double>(cols,0));
            if(dorandom){
                mutate1();
            }
        }
        void mutate1(double min =0,double max=10){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    double randNum=10;
                    static std::uniform_real_distribution<double> dis(-1.0, 1.0); // rage -1 - 1
                    while(randNum<-1 || randNum>1){
                        randNum =dis(mt2);
                    }
                    data[i][j]=randNum;
                }
            }
        }
        void multiply(double scaler){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    data[i][j]*=scaler;
                }
            }
        }
        void multiply(Matrix scaler){
            if(scaler.rows!=rows||scaler.cols!=cols){
                cerr<<"Nesedi "<<scaler.rows<<"!="<<rows<<" alebo "<<scaler.cols<<"!="<<cols<<endl;
            }
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    data[i][j]*=scaler.data[i][j];
                }
            }
        }
        void map1( double (*f)(double)){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    double h=data[i][j];
                    data[i][j]=f(h);
                }
            }
        }
        
        void map1( double (*f)(double,double),double r){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    double h=data[i][j];
                    data[i][j]=f(h,r);
                }
            }
        }
        
        static Matrix map1(Matrix a ,double (*f)(double)){
            Matrix result(a.rows,a.cols,false);
            for(int i=0;i<a.rows;i++){
                for(int j=0;j<a.cols;j++){
                    double h=a.data[i][j];
                    h=f(h);
                    
                    if(isnan(h)){
                        h=1;
                        cerr<<"NAN"<<endl;
                    }
                    result.data[i][j]=h;
                }
            }
            return result;
        }
         void add(Matrix scaler1){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    data[i][j]+=scaler1.data[i][j];
                }
            }
        }
        void subtract(Matrix scaler1){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    data[i][j]-=scaler1.data[i][j];
                }
            }
        }
         void add(double scaler1){
            for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                    data[i][j]+=scaler1;
                }
            }
        }
        static Matrix fromarray(double arr[],int size){
            Matrix m(size,1,false);
            //cout<<(sizeof(arr)/sizeof(*arr))<<endl;
            for(int i=0;i<size;i++ ){
                m.data[i][0]=arr[i];
            }
            return m;
        }
        static Matrix subtract(Matrix scaler1,Matrix scaler2){
            Matrix m(scaler1.rows,scaler1.cols,false);
            for(int i=0;i<scaler1.rows;i++){
                for(int j=0;j<scaler1.cols;j++){
                    cout.precision(20);
                    m.data[i][j]=scaler1.data[i][j]-scaler2.data[i][j];
                }}
            return m;
        }
        static Matrix fromarray(vector<vector<double>> arr){
            Matrix m(arr.size(),arr[0].size(),false);
            for(int i=0;i<arr.size();i++ ){
                for(int j=0;j<arr[0].size();j++){
                    m.data[i][j]=arr[i][j];
                }
            }
            return m;
        }
        double* toarray(double arr[]){//I need to pass an array to function
            for(int i=0;i<rows;i++ ){
                    arr[i]=data[i][0];
            }
            return arr;
        }
        static Matrix multiply(Matrix scaler1,Matrix scaler2){
            if(scaler1.cols!=scaler2.rows){
                cerr<<"Error:Get out of here your rows must equal my cols\n";
                scaler1.print();
                scaler2.print();
                return scaler2;
            }
            Matrix result(scaler1.rows,scaler2.cols,false);
            for(int i = 0; i < result.rows; ++i){
                for(int j = 0; j < result.cols; ++j){
                    for(int k = 0; k < scaler1.cols; ++k){
                        result.data[i][j] += scaler1.data[i][k] * scaler2.data[k][j];
                    }
                }
            }
            return result;
        }
        void resize1(int rows1,int cols1, bool dorandom1=true){
            data.resize(rows1, vector<double>(cols1,0));
            rows=rows1;
            cols=cols1;
            if(dorandom1){
                mutate1();
            }
        }
        void resize2(int rows2,int cols2){
            data.resize(rows2, vector<double>(cols2));
            rows=rows2;
            cols=cols2;
            cerr<<rows<<" "<<cols<<endl;
            mutate1();
        }
        static Matrix transpose(Matrix a){
            Matrix result(a.cols,a.rows);
            for(int i=0;i<a.rows;i++){
                for(int j=0;j<a.cols;j++){
                    result.data[j][i]=a.data[i][j];
                }
            }
            return result;
        }
        void print(){
            cout<<data.size()<<" "<<data[0].size()<<endl;
            for(int i=0;i<data.size();i++){
                for(int j=0;j<data[i].size();j++){
                    cout<<data[i][j]<<" ";
                }
            cout<<endl;
            }
            cout<<endl;
        }
        static Matrix import1(){
            int a,b;
            cin>>a>>b;
            Matrix result(a,b,false);
            for(int i=0;i<a;i++){
                for(int j=0;j<b;j++){
                    cin>>result.data[i][j];
                }
            }
            return result;
        }
};