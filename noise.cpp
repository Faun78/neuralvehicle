#include <math.h>
#include <vector>
#include <random>
#include <ctime>


using namespace std;
const int PERLIN_YWRAPB = 4;
const float PERLIN_YWRAP = 1 << PERLIN_YWRAPB;
const int PERLIN_ZWRAPB = 8;
const float PERLIN_ZWRAP = 1 << PERLIN_ZWRAPB;
const float PERLIN_SIZE = 4095;

int perlin_octaves = 4; 
float perlin_amp_falloff = 0.5; 
    
double PI1 =3.14159265359;
mt19937 mt5(time(0));

float scaled_cosine (float i ){ 
 return 0.5 * (1.0 - cos(i * PI1));
}
#define uint32 uint32_t;

class lcg{
   public:
      long long c11,a11,m11;
      long long seed11, z11;
      lcg(){
      m11 = 4294967296;
      a11 = 1664525;
      c11 = 1013904223;
      seed11=mt5()*m11;
      z11=seed11;
      };  
      void setSeed11(int val11=0) {
          if(val11!=0){
            z11=val11;
            seed11=val11;
          }else{
            long long randval=mt5();
            z11=abs(randval * m11);
            seed11=abs(randval * m11);
          }
      }
      int getSeed11() {
          return seed11;
        }
      double rand123(){
          z11 = (a11 * z11+mt5() + c11) % m11;
          return abs(double(double(z11)/double(m11)));
        }
};
vector <double> perlin; 
double noise2D(lcg l11,float x,float y = 0,float z = 0) {
  if (perlin.size() == 0) {
    perlin.resize(PERLIN_SIZE + 1) ;
    for (int i = 0; i < PERLIN_SIZE + 1; i++) {
      perlin[i] = l11.rand123();
    }
  }

  if (x < 0) {
    x = -x;
  }
  if (y < 0) {
    y = -y;
  }
  if (z < 0) {
    z = -z;
  }

  int xi = int(x),
    yi = int(y),
    zi = int(z);
  float xf = x - xi;
  float yf = y - yi;
  float zf = z - zi;
  float rxf, ryf;

  double r = 0;
  float ampl = 0.5;

  float n1, n2, n3;

  for (int o = 0; o < perlin_octaves; o++) {
    int of = xi + (yi << PERLIN_YWRAPB) + (zi << PERLIN_ZWRAPB);
    rxf = scaled_cosine(xf);
    ryf = scaled_cosine(yf);
    int of1,of2,of3,of4;
    if(of>=PERLIN_SIZE){
      of1=PERLIN_SIZE;
    }else{
      of1=of;
    }
    if(of+PERLIN_YWRAP>=PERLIN_SIZE){
      of2=PERLIN_SIZE;
    }else{
      of2=of;
    }
    if(of+1>=PERLIN_SIZE){
      of3=PERLIN_SIZE;
    }else{
      of3=of;
    }
    if(of+1+PERLIN_YWRAP>=PERLIN_SIZE){
      of4=PERLIN_SIZE;
    }else{
      of4=of;
    }        
    n1 = perlin[of1 ];
    n1 += rxf * (perlin[(of3 + 1) ] - n1);
    n2 = perlin[(of2 + PERLIN_YWRAP)];
    n2 += rxf * (perlin[(of4 + PERLIN_YWRAP + 1) ] - n2);
    n1 += ryf * (n2 - n1);
    of += PERLIN_ZWRAP;
    if(of>=PERLIN_SIZE){
      of1=PERLIN_SIZE;
    }else{
      of1=of;
    }
    if(of+PERLIN_YWRAP>=PERLIN_SIZE){
      of2=PERLIN_SIZE;
    }else{
      of2=of;
    }
    if(of+1>=PERLIN_SIZE){
      of3=PERLIN_SIZE;
    }else{
      of3=of;
    }
    if(of+1+PERLIN_YWRAP>=PERLIN_SIZE){
      of4=PERLIN_SIZE;
    }else{
      of4=of;
    }  
    
    n2 = perlin[of1];
    n2 += rxf * (perlin[(of3 + 1) ] - n2);
    n3 = perlin[(of2 + PERLIN_YWRAP) ];
    n3 += rxf * (perlin[(of4 + PERLIN_YWRAP + 1)] - n3);
    n2 += ryf * (n3 - n2);

    n1 += scaled_cosine(zf) * (n2 - n1);
    r += n1 * ampl;
    ampl *= perlin_amp_falloff;
    xi <<= 1;
    xf *= 2;
    yi <<= 1;
    yf *= 2;
    zi <<= 1;
    zf *= 2;

    if (xf >= 1.0) {
      xi++;
      xf--;
    }
    if (yf >= 1.0) {
      yi++;
      yf--;
    }
    if (zf >= 1.0) {
      zi++;
      zf--;
    }
  }
  perlin.clear();
  return r;
};
 
void noiseDetail (float lod,float falloff) {
  if (lod > 0) {
    perlin_octaves = lod;
  }
  if (falloff > 0) {
    perlin_amp_falloff = falloff;
  }
};
void noiseSeed(lcg l11,float seed111=0) {
  l11.setSeed11(seed111);
  perlin.clear();
  perlin.resize(PERLIN_SIZE + 1);
  for (int i = 0; i < PERLIN_SIZE + 1; i++) {
    perlin[i] = l11.rand123();
  }
};