#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <tgmath.h>
#include <math.h>
double height=800;
double width=800;
double mapp5(double n,double start1,double stop1,double start2,double stop2){
     return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
};
using namespace sf;
RenderWindow window(VideoMode(height,width), "SFML works!");
Vector2f normalized(const Vector2f& source){
    float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return Vector2f(source.x / length, source.y / length);
    else
        return source;
}
class Boundry{
    public:
        Vector2f p1;
        Vector2f p2;
        RectangleShape rect1;
        Vector2f a;
        Vector2f b;
        Vector2f c;
        Vector2f d;
        float thickness;
        Boundry(double x1,double y1,double x2,double y2,float thick=2){
           p1.x=x1;
           p1.y=y1;
           p2.x=x2;
           p2.y=y2;
            thickness=thick;
           Vector2f line = p2 - p1;
           Vector2f normal = normalized(Vector2f( -line.y, line.x));
             a = p1 - thickness * normal;
             b = p1 + thickness * normal;
             c = p2 - thickness * normal;
             d = p2 + thickness * normal;
        }
        Boundry(){

        }
        void draw(bool colo1=false){
        sf::Vertex line[] ={sf::Vertex(a), sf::Vertex(b),sf::Vertex(c),sf::Vertex(d)};
        if(colo1){
            for(int i=0;i<4;i++){
                line[i].color =  sf::Color(255, 255, 255, 45);
                //lin.color =  sf::Color(0, 0, 0, 255)

            }
        }
        window.draw(line, 4, sf::TriangleStrip);  
        }


};