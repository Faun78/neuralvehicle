#include "ray.cpp"
#include "noise.cpp"
#include <cstdlib>
#include <ctime>
using namespace std;
lcg l1;
struct car{
    Car pp;
    double score;
    bool finished;
};
vector<Boundry> walls;
mt19937 mt8(time(0));
sf::Vector2f start(75-5,350-5);
sf::Vector2f end11(350-5,75-5);
int popu=100;
bool newgen=true;
bool debug=false;
double numberofpart=45;
vector<car> alive;
vector<car> dead;
int generation=0;
vector<Vector2f> outside;
vector<Vector2f> inside;
vector<Vector2f> check11;
static std::uniform_real_distribution<> dis5(0, height);
static std::uniform_real_distribution<> dis6(0, 0.999999999999);
static std::uniform_real_distribution<> dis8(-0.06, 0.06);
void newGeneration(NeuralNetworkLayers BB1){
    generation++;
    alive.clear();
    alive.resize(popu);
    if(newgen){
        for(int iiii=0;iiii<popu;iiii++){
            alive[iiii].pp=Car(start.x+4,start.y+4,numberofpart);
            alive[iiii].score=0;
            alive[iiii].finished=false;
            newgen=false;
        }
    }else{
        for(int iiii=0;iiii<popu;iiii++){
            alive[iiii].pp=Car(start.x+4,start.y+4,numberofpart);
        if(iiii!=0){
            alive[iiii].pp.brain=NeuralNetworkLayers::mutate4(BB1,pow(0.1+iiii/popu,2));
        }else{
           alive[iiii].pp.brain=NeuralNetworkLayers(BB1); 
        }
        alive[iiii].score=0;
        alive[iiii].finished=false;
        }
    }
}
void applybatch(int from,int to){
    for(int i=from;i<to;i++){
        if(to>=alive.size()){
            break;
        }
        alive[i].pp.applyforce(sf::Vector2f(dis8(mt2),dis8(mt2)));        
        alive[i].pp.update();
        alive[i].score+=1;
        alive[i].finished=alive[i].pp.check();
        bool aa=alive[i].pp.show(walls,debug);//if debug true then raycasting is visible
        if(aa||alive[i].finished){
            int numofcehckspass =accumulate(alive[i].pp.checks.begin(),alive[i].pp.checks.end(),0);
            alive[i].score=(3000-alive[i].score)*(numofcehckspass*2);
            dead.resize(dead.size()+1);
            swap(alive[i],alive[alive.size()-1]);
            swap(dead[dead.size()-1],alive[alive.size()-1]);
            alive.pop_back();
            i-=1;
        }
    }
}
void regeneratewalls(){
    l1.setSeed11(0);
    double noisemax=10;
    double seed =dis6(mt2);
    outside.clear();
    inside.clear();
    static const double TWOPI = 6.2831853071795865;
    for(double a=0;a<TWOPI;a+=toradian(6)){
        double xoff=mapp5(cos(a),-1,1,0,noisemax);
        double yoff=mapp5(sin(a),-1,1,0,noisemax);
        double r=mapp5(noise2D(l1,xoff,yoff),0,1,250,height/2-50);
        double x11=width/2+(r-50)*cos(a);
        double y11=height/2+(r-50)*sin(a);
        double x22=width/2+(r+50)*cos(a);
        double y22=height/2+(r+50)*sin(a);
        Vector2f a11;
        Vector2f a22;
        Vector2f c111;
        c111.x=width/2+r*cos(a);
        c111.y=height/2+r*sin(a);
        a22.x=x22;
        a22.y=y22;
        a11.x=x11;
        a11.y=y11;
        outside.push_back(a11);
        inside.push_back(a22);
        check11.push_back(c111);
    }
    walls.clear();
    checkpoint.clear();
    for(int i=0;i<outside.size();i++){
        if(i==outside.size()-1){
            walls.push_back(Boundry(outside[i].x,outside[i].y,outside[0].x,outside[0].y,0.5));
            walls.push_back(Boundry(inside[i].x,inside[i].y,inside[0].x,inside[0].y,0.5));
            walls.push_back(Boundry(outside[0].x-10,outside[0].y-20,inside[0].x+10,inside[0].y-20,0.5));
            checkpoint.push_back(Boundry(outside[i].x,outside[i].y,inside[i].x,inside[i].y,0.5));        
            break;
        }
        walls.push_back(Boundry(outside[i].x,outside[i].y,outside[i+1].x,outside[i+1].y,0.5));
        walls.push_back(Boundry(inside[i].x,inside[i].y,inside[i+1].x,inside[i+1].y,0.5));
        checkpoint.push_back(Boundry(outside[i].x,outside[i].y,inside[i].x,inside[i].y,0.5));        
    }
    start.x=check11[0].x-5;
    start.y=check11[0].y-5;
    end11.x=check11[check11.size()-1].x-5;
    end11.y=check11[check11.size()-1].y-5;
    check11.clear();
    cerr<<"Walls generated\n";
}
int main(){
    l1=lcg();
    l1.setSeed11(0);
    cerr<<l1.m11<<endl;
    cerr<<l1.seed11<<endl;
    double xoff=0;
    double yoff=0;
    vector<sf::Thread>carsdestroyed;
    sf::CircleShape sta1(5);
    sf::CircleShape end1(5);
    NeuralNetworkLayers b1; 
    if(!newgen){
        b1=NeuralNetworkLayers::importbrainLayer();
    }
    regeneratewalls();
    newGeneration(b1);
    sta1.setPosition(start);
    end1.setPosition(end11);
    int numofwall=8;
    long long cycle=0;
    window.setFramerateLimit(60);
    while (window.isOpen()){
        cycle++;
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                int maxiu1=0;
                for(int uu=0;uu<dead.size();uu++){
                    if(dead[uu].score>dead[maxiu1].score){
                        maxiu1=uu;
                    }
                }
                dead[maxiu1].pp.brain.exportBrainLayer();
                cerr<<dead[maxiu1].score<<endl;
                window.close();
            }
        }
        window.clear();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5)){
            regeneratewalls();
            sta1.setPosition(start);
            end1.setPosition(end11);
        }
        for(int i=0;i<alive.size();i++){
            alive[i].pp.applyforce(sf::Vector2f(dis8(mt2),dis8(mt2))); 
            alive[i].pp.update();
            alive[i].score+=1;
            alive[i].finished=alive[i].pp.check();

            bool aa=alive[i].pp.show(walls,debug);
            if(aa||alive[i].finished){
                int numofcehckspass =accumulate(alive[i].pp.checks.begin(),alive[i].pp.checks.end(),0);
                alive[i].score=(3000-alive[i].score)*(numofcehckspass*2);
                dead.resize(dead.size()+1);
                swap(alive[i],alive[alive.size()-1]);
                swap(dead[dead.size()-1],alive[alive.size()-1]);
                alive.pop_back();
                i-=1;
            }
        }
        window.draw(sta1);
        window.draw(end1);
        for(auto wall:walls){
            wall.draw();
        }
        for(auto check:checkpoint){
            check.draw();
        }
        window.display();
        if(dead.size()==popu &&cycle%100==0||cycle>min(max(200*generation,1500),3000)||sf::Keyboard::isKeyPressed(sf::Keyboard::F7)&&cycle%100==0){
            cerr<<"cleaning\n";
            for(int i=0;i<alive.size();i++){//kill all
                int numofcehckspass =accumulate(alive[i].pp.checks.begin(),alive[i].pp.checks.end(),0);
                alive[i].score=(3000-alive[i].score)*(numofcehckspass*2);
                dead.resize(dead.size()+1);
                swap(alive[i],alive[alive.size()-1]);
                swap(dead[dead.size()-1],alive[alive.size()-1]);
                alive.pop_back();
                i-=1;
            }
            alive.clear();
            int maxiu=0;
            int maxiub=0;
            int maxchecks=0;
            for(int uu=0;uu<dead.size();uu++){
                if(dead[uu].score>dead[maxiu].score){
                    maxiu=uu;
                }
            }
            cerr<<maxiub<<endl;
            if(maxiub!=0){
                newGeneration(dead[maxiub].pp.brain);
                cerr<<dead[maxiub].score<<" finished "<<maxiub<<endl;
            }else{
                newGeneration(dead[maxiu].pp.brain);
            }
            int numofcehckspass2 =accumulate(dead[maxiu].pp.checks.begin(),dead[maxiu].pp.checks.end(),0);
            dead.clear();
            cerr<<"Dead "<<maxcheckpoint<<" "<<numofcehckspass2<<endl;
            maxcheckpoint=0;
            cycle=0;
        }
    }
    return 0;
}