
#include "ray.cpp"
#include <random>
#include <ctime>

using namespace std;
struct car{
    Particle pp;
    double score;
    bool finished;
};

mt19937 mt8(time(0));
sf::Vector2f start(75-5,350-5);
sf::Vector2f end11(350-5,75-5);
int popu=300;
bool newgen=false;
double numberofpart=35;
vector<car> alive;
void newGeneration(NeuralNetwork BB1){
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
        alive[iiii].pp.brain=NeuralNetwork::mutate4(BB1,0.1+iiii/10);
        alive[iiii].score=0;
        alive[iiii].finished=false;
    }}
}
int main(){
    double xoff=0;
    double yoff=0;

    sf::CircleShape sta1(5);
    sf::CircleShape end1(5);

    NeuralNetwork b1; 
    if(!newgen){
        b1=NeuralNetwork::importbrain();
    }
    checkpoint.push_back(Boundry(350,50,350,100,0.5));
    newGeneration(b1);
    sta1.setPosition(start);
    end1.setPosition(end11);
    int numofwall=8;
    //walls
        

    vector<Boundry> walls;
        {
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
        }
    //walls
    vector<car> dead;
    long long cycle=0;

    // cerr<<"part done\n";
    window.setFramerateLimit(60);
    while (window.isOpen()){
        cycle++;
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                int maxiu1=0;
            // cerr<<"closing1"<<endl;
            for(int uu=0;uu<dead.size();uu++){
                if(dead[uu].score>dead[maxiu1].score){
                    maxiu1=uu;
                }
                }
    //pick one function
        
        
        dead[maxiu1].pp.brain.exportBrain();
        //newGeneration(dead[maxiu].pp.brain);
        cerr<<dead[maxiu1].score<<endl;
        
        //window.close();
        //dead.clear();
            cerr<<"closing\n";
                window.close();
                }
        }
        window.clear();
        //generate new track
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5)){
            walls.clear();
            for(int jjj=0;jjj<numofwall;jjj++){
        walls.push_back(Boundry(mt8()%int(width),mt8()%int(width),mt8()%int(height),mt8()%int(height)));
        }}
        //generate new track
        //showing the cars and applying forces
        for(int i=0;i<alive.size();i++){
        alive[i].pp.applyforce(sf::Vector2f(0,-0.4));        
        alive[i].pp.update();
        
            // cerr<<"checking\n";
        
        alive[i].finished=alive[i].pp.check();
        
            // cerr<<"checked\n";
        
        bool aa=alive[i].pp.show(walls);
        if(aa||alive[i].finished){
            dead.resize(dead.size()+1);
            swap(alive[i],alive[alive.size()-1]);
            swap(dead[dead.size()-1],alive[alive.size()-1]);
            alive.pop_back();
        }
        alive[i].score+=1;
        }
        //showing the cars and applying forces
            //drawing start and end
             window.draw(sta1);
             window.draw(end1);
            //drawing start and end
        //drawing walls and checkpoints
        for(auto wall:walls){
            wall.draw();
        }
        for(auto check:checkpoint){
            check.draw();
        }
        //drawing walls and checkpoints
    window.display();
    //pick one function
    if(dead.size()==popu &&cycle%100==0||cycle>600){
        cerr<<"cleaning\n";
        int maxiu=0;
        int maxiub=0;

        for(int uu=0;uu<dead.size();uu++){
            if(dead[uu].score>dead[maxiu].score){
                maxiu=uu;
            }
            if(dead[uu].finished){
                if(dead[uu].score<dead[maxiub].score||!dead[maxiub].finished)
                maxiub=uu;
            }
        }
    //pick one function
        cerr<<maxiub<<endl;
        if(maxiub!=0){
        newGeneration(dead[maxiub].pp.brain);
        //dead[maxiub].pp.brain.exportBrain();
        cerr<<dead[maxiub].score<<" finished "<<maxiub<<endl;
        }else{
        //dead[maxiu].pp.brain.exportBrain();
        newGeneration(dead[maxiu].pp.brain);
        cerr<<dead[maxiu].score<<endl;
        }
        //window.close();
        dead.clear();
        cycle=0;
    }
    //pick one function
    }
    return 0;
}