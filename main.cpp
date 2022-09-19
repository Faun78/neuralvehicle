
#include "ray.cpp"
#include "noise.cpp"
#include <cstdlib>

#include <ctime>
using namespace std;
lcg l1;

struct car{
    Particle pp;
    double score;
    bool finished;
};
vector<Boundry> walls;
mt19937 mt8(time(0));
sf::Vector2f start(75-5,350-5);
sf::Vector2f end11(350-5,75-5);
int popu=110;
bool newgen=true;
double numberofpart=35;
vector<car> alive;
int generation=0;

void newGeneration(NeuralNetwork BB1){
    generation++;
    alive.clear();
    alive.resize(popu);
    if(newgen){
        for(int iiii=0;iiii<popu;iiii++){
            alive[iiii].pp=Particle(start.x+4,start.y+4,numberofpart);
            alive[iiii].score=0;
            alive[iiii].finished=false;
            newgen=false;
    }
    }else{
        for(int iiii=0;iiii<popu;iiii++){
            alive[iiii].pp=Particle(start.x+4,start.y+4,numberofpart);
        if(iiii!=0){
            alive[iiii].pp.brain=NeuralNetwork::mutate4(BB1,pow(0.1+iiii/popu,2));
        }else{
           alive[iiii].pp.brain=BB1; 
        }
        alive[iiii].score=0;
        alive[iiii].finished=false;
        }
    }
}

vector<Vector2f> outside;
vector<Vector2f> inside;
vector<Vector2f> check11;


static std::uniform_real_distribution<> dis5(0, height);
static std::uniform_real_distribution<> dis6(0, 0.999999999999);
static std::uniform_real_distribution<> dis8(-0.06, 0.06);

void regeneratewalls(){
    l1.setSeed11(0);
    double noisemax=10;
    //walls.clear();
    double seed =dis6(mt2);
    outside.clear();
    inside.clear();
    static const double TWOPI = 6.2831853071795865;
    for(double a=0;a<TWOPI;a+=toradian(25)){
        double xoff=mapp5(cos(a),-1,1,0,noisemax);
        double yoff=mapp5(cos(a),-1,1,0,noisemax);
        double r=mapp5(noise2D(l1,xoff,yoff),0,1,200,height/2+50);
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
}
int main(){
    l1=lcg();
    l1.setSeed11(0);
    cerr<<l1.m11<<endl;
    cerr<<l1.seed11<<endl;
    double xoff=0;
    double yoff=0;

    sf::CircleShape sta1(5);
    sf::CircleShape end1(5);

    NeuralNetwork b1; 
    if(!newgen){
        b1=NeuralNetwork::importbrain();
    }
    regeneratewalls();
    newGeneration(b1);
    sta1.setPosition(start);
    end1.setPosition(end11);
    int numofwall=8;
    //walls
        /*{
        walls.push_back(Boundry(50,400,50,200,0.5));
        walls.push_back(Boundry(50,200,150,50,0.5));
        walls.push_back(Boundry(150,50,400,50,0.5));
        walls.push_back(Boundry(100,400,100,200,0.5));
        walls.push_back(Boundry(100,200,175,100,0.5));
        walls.push_back(Boundry(175,100,400,100,0.5));
        walls.push_back(Boundry(400,0,400,400,0));
        walls.push_back(Boundry(0,400,400,400,0));
        walls.push_back(Boundry(0,0,0,400,0));
        walls.push_back(Boundry(0,0,400,0,0));
    checkpoint.push_back(Boundry(350,50,350,100,0.5));
        }*/
    //walls
    vector<car> dead;
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
                //pick one function
                dead[maxiu1].pp.brain.exportBrain();
                //newGeneration(dead[maxiu].pp.brain);
                cerr<<dead[maxiu1].score<<endl;
                cerr<<"closing\n";
                window.close();
            }
        }
        window.clear();

        //generate new track
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5)){
            regeneratewalls();
            sta1.setPosition(start);
            end1.setPosition(end11);

        }
        //generate new track
        //showing the cars and applying forces
        for(int i=0;i<alive.size();i++){
            alive[i].pp.applyforce(sf::Vector2f(dis8(mt2),dis8(mt2)));        
            alive[i].pp.update();
            alive[i].score+=1;
            alive[i].finished=alive[i].pp.check();
            bool aa=alive[i].pp.show(walls);
            if(aa||alive[i].finished){
                //alive[i].score+=accumulate(alive[i].pp.checks.begin(),alive[i].pp.checks.end(),0)*1000;
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
        for(int i=0;i<alive.size();i++){
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
    //pick one function
        cerr<<maxiub<<endl;
        if(maxiub!=0){
            newGeneration(dead[maxiub].pp.brain);
            cerr<<dead[maxiub].score<<" finished "<<maxiub<<endl;
        }else{
            cerr<<dead[maxiu].score<<" "<<maxchecks<<" "<<dead.size()<<" "<<alive.size()<<endl;
            newGeneration(dead[maxiu].pp.brain);
        }
        //window.close();
        dead.clear();
        cycle=0;
    }
    //pick one function
    }
    return 0;
}