
#include "boundry.cpp"
#include "nn.cpp"
std::vector<Boundry> checkpoint;
double maxspeed=40;
double toradian(double degree){
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}
double bearing(double a1, double a2, double b1, double b2) {
    static const double TWOPI = 6.2831853071795865;
    static const double RAD2DEG = 57.2957795130823209;
    // if (a1 = b1 and a2 = b2) throw an error 
    double theta = atan2(b1 - a1, a2 - b2);
    if (theta < 0.0)
        theta += TWOPI;
    return RAD2DEG * theta;
}
double carwidth=8;
double carheight=16;

using namespace sf;
class Ray{
    public:
        Vector2f pos;
        Vector2f dir;
        Boundry line1;
        Ray(Vector2f pos1,double angle=0){
            pos=pos1;
            dir.x=cos(angle);
            dir.y=sin(angle);
            //std::cerr<<dir.x<<dir.y<<std::endl;
            line1=Boundry(pos.x,pos.y,pos.x+dir.x*10,pos.y+dir.y*10,0.5);
        }
        void lookat(double x55,double y55){
            dir.x=(x55-pos.x)/width;
            dir.y=(y55-pos.y)/height;
            dir= normalized(dir);
            line1=Boundry(pos.x,pos.y,pos.x+dir.x*10,pos.y+dir.y*10,1);
        }
        Vector2f cast(Boundry wall1){
            const double x1=wall1.p1.x;
            const double y1=wall1.p1.y; 
            const double x2=wall1.p2.x; 
            const double y2=wall1.p2.y;
            
            const double x3=pos.x;
            const double y3=pos.y; 
            const double x4=pos.x+dir.x; 
            const double y4=pos.y+dir.y;
            double den= (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
            if(den==0){
                //std::cerr<<"dead\n";
                return Vector2f(-10000,-10);
            }
            double t= ((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4))/den;
            double u= -((x1-x2)*(y1-y3)-(y1-y2)*(x1-x3))/den;
            if(t>0&&t<1&&u>0){
                Vector2f pt;
                pt.x=x1+t*(x2-x1);
                pt.y=y1+t*(y2-y1);
                 return pt;
            }else{

                return Vector2f(-10000,-10);
            }

        }
};
class Particle{
    public:
        Vector2f pos;
        Vector2f vel;
        Vector2f acc;
        std::vector <Ray> rays1;
        NeuralNetwork brain;
        std::vector <int> checks;
        sf::RectangleShape pp; 
        double numberofrays;
        double sight=60;
        Particle(double x,double y,double numberofray){
            pp=sf::RectangleShape(sf::Vector2f(carwidth,carheight));
            vel.x=0;
            vel.y=0;
            acc.x=0;
            acc.y=0;
            checks.resize(checkpoint.size(),0); 
            numberofrays=numberofray;
            pos=Vector2f(x,y);
           rays1.clear();
            for(double i=0;i<360;i+=numberofrays){
                rays1.push_back(Ray(pos,toradian(i)));
            }
        brain =NeuralNetwork(rays1.size(),15,1);
        }
        void applyforce(Vector2f force){
            acc+=force;
        }
        void update(){
            //cerr<<vel.y<<endl;
            if(vel.x>maxspeed){
                cerr<<"better"<<endl;
                vel.x=maxspeed;
            }else if(vel.x<-maxspeed){
                cerr<<"better"<<endl;

                vel.x=-maxspeed;
            }
            if(vel.y>maxspeed){
                vel.y=maxspeed;
                cerr<<"better"<<endl;

            }else if(vel.y<-maxspeed){
                vel.y=-maxspeed;
                cerr<<"better"<<endl;

            }
           pos+=vel;
           vel+=acc;
           acc.x=0;
           acc.y=0;
           rays1.clear();
           for(double i=0;i<360;i+=numberofrays){
                rays1.push_back(Ray(pos,toradian(i)));
            }
        }
        Particle(){

        }

        bool show(std::vector<Boundry> w1,bool showbounds=false){
            bool touch=false;
            double inputs[rays1.size()];
            pp.setFillColor(sf::Color(sf::Color(255,255,255,150)));
            pp.setPosition(pos.x-carwidth/2,pos.y-carheight/2);
            int calcr=0;
            for(auto ray1:rays1){
                
                double record =INFINITY;
                sf::Vector2f dest1;
                dest1.x=-10000;
                dest1.y=0;
                if(showbounds){
                ray1.line1.draw(false);
                }
                for(int i=0;i<w1.size();i++){
                    sf::Vector2f dest= ray1.cast(w1[i]);
                    if(dest.x!=-10000){
                        double ddd =sqrt(pow(dest.x - pos.x, 2) + pow(dest.y - pos.y, 2) * 1.0);
                        if(ddd<record &&ddd<sight){
                            record=ddd;
                            dest1=dest;
                        }
                        if(ddd<=(carheight/2)*0.85){
                            //std::cerr<<"hit\n";
                        touch=true;
                    }}}
                if(dest1.x!=-10000){
                    bool docoloring=false;
                    inputs[calcr]=(sqrt(pow(dest1.x - pos.x, 2) + pow(dest1.y - pos.y, 2) * 1.0))/sight;
                    if(showbounds){
                    Boundry line5(pos.x,pos.y,dest1.x,dest1.y,0.5);
                    line5.draw(docoloring);
                    }
            }else{
                inputs[calcr]=1;
            }
            calcr++;
            }
            if(touch){
                pp.setFillColor(sf::Color(sf::Color::Red));
                window.draw(pp);
                return true;
            }
            double arr[1];
            double output =brain.feedforward(inputs,arr)[0];
            double angle11=output*2*2*acos(0.0);
            Vector2f des;
            des.x=cos(angle11);
            des.y=sin(angle11);
            des-=vel;
            /*double anglex1=vel.x/4;
            double angley1=vel.y/4;
            double anglex2=pos.x;
            double angley2=pos.y;
            double angx=acos(anglex1);
            double angy=acos(angley1);*/
            //cerr<<angx<<" "<<angy<<endl;
            
            pp.setRotation(bearing(pos.x,pos.y,pos.x+vel.x,pos.y+vel.y));
            applyforce(des);
            window.draw(pp);
            return false;
        };
        bool check(){
                for(auto ray1:rays1){
                double record =INFINITY;
                sf::Vector2f dest1;
                dest1.x=-10000;
                dest1.y=0;
                for(int i=0;i<checkpoint.size();i++){
                    sf::Vector2f dest= ray1.cast(checkpoint[i]);
                    if(dest.x!=-10000){
                        double ddd =sqrt(pow(dest.x - pos.x, 2) + pow(dest.y - pos.y, 2) * 1.0);
                        if(ddd<record &&ddd<sight){
                            record=ddd;
                            dest1=dest;
                        }else{
                            return false;
                        }
                        // cerr<<ddd<<endl;
                        if(ddd<=(carheight/2)*0.2){
                            //cerr<<"adding at "<<checks.size()<<endl;
                            checks[i]=1;
                            // cerr<<"added\n";
                                 if(checks.size()==accumulate(checks.begin(),checks.end(),0)){
                                    return true;
                                 }else{
                                     return false;
                                 }            
                    }}}  
            }
            return false;  
        };

};