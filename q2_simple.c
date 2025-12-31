#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define W 1280
#define H 720
typedef unsigned char U;typedef unsigned int I;typedef float F;typedef struct{F x,y,z;}V;typedef struct{F m[16];}M;
SDL_Window*win;SDL_GLContext ctx;I prg,vao,vbo;V cam;F ya,pi;F*vbuf;I nverts;
char*LD(const char*p){FILE*f=fopen(p,"rb");if(!f)return 0;fseek(f,0,2);long s=ftell(f);fseek(f,0,0);char*b=malloc(s+1);fread(b,1,s,f);b[s]=0;fclose(f);return b;}
I CS(I t,const char*p){char*s=LD(p);I h=glCreateShader(t);glShaderSource(h,1,(const char**)&s,0);glCompileShader(h);free(s);return h;}
void IN(){SDL_Init(SDL_INIT_VIDEO);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);win=SDL_CreateWindow("Q2",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_OPENGL);ctx=SDL_GL_CreateContext(win);glewInit();glEnable(GL_DEPTH_TEST);}
void SH(){I vs=CS(GL_VERTEX_SHADER,"vert.glsl"),fs=CS(GL_FRAGMENT_SHADER,"frag.glsl");prg=glCreateProgram();glAttachShader(prg,vs);glAttachShader(prg,fs);glLinkProgram(prg);glUseProgram(prg);}
M ID(){M r;for(int i=0;i<16;i++)r.m[i]=i%5==0;return r;}
M PR(F fov,F asp,F n,F f){M r=ID();F t=1.0f/tan(fov*0.5f);r.m[0]=t/asp;r.m[5]=t;r.m[10]=-(f+n)/(f-n);r.m[11]=-1;r.m[14]=-(2*f*n)/(f-n);r.m[15]=0;return r;}
M LK(V e,V c,V u){V f={c.x-e.x,c.y-e.y,c.z-e.z};F l=sqrtf(f.x*f.x+f.y*f.y+f.z*f.z);f.x/=l;f.y/=l;f.z/=l;V s={f.y*u.z-f.z*u.y,f.z*u.x-f.x*u.z,f.x*u.y-f.y*u.x};l=sqrtf(s.x*s.x+s.y*s.y+s.z*s.z);s.x/=l;s.y/=l;s.z/=l;V uu={s.y*f.z-s.z*f.y,s.z*f.x-s.x*f.z,s.x*f.y-s.y*f.x};M r=ID();r.m[0]=s.x;r.m[4]=s.y;r.m[8]=s.z;r.m[1]=uu.x;r.m[5]=uu.y;r.m[9]=uu.z;r.m[2]=-f.x;r.m[6]=-f.y;r.m[10]=-f.z;r.m[12]=-(s.x*e.x+s.y*e.y+s.z*e.z);r.m[13]=-(uu.x*e.x+uu.y*e.y+uu.z*e.z);r.m[14]=f.x*e.x+f.y*e.y+f.z*e.z;return r;}
void MK(){F d=200;vbuf=malloc(36*10*sizeof(F));F*v=vbuf;for(int f=0;f<6;f++){F x=(f==0)-(f==1),y=(f==2)-(f==3),z=(f==4)-(f==5);for(int i=0;i<6;i++){int t[]={0,1,2,0,2,3};int j=t[i];F u=((j&1)?1:-1)*d,w=((j&2)?1:-1)*d;V p={x?x*d:u,y?y*d:w,z?z*d:(x?w:u)};*v++=p.x;*v++=p.y;*v++=p.z;*v++=(j&1);*v++=(j>>1&1);*v++=0.5;*v++=0.5;*v++=x;*v++=y;*v++=z;}}nverts=36;}
void DR(){glGenVertexArrays(1,&vao);glGenBuffers(1,&vbo);glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,nverts*10*sizeof(F),vbuf,GL_STATIC_DRAW);glEnableVertexAttribArray(0);glVertexAttribPointer(0,3,GL_FLOAT,0,40,0);glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,0,40,(void*)12);glEnableVertexAttribArray(2);glVertexAttribPointer(2,2,GL_FLOAT,0,40,(void*)20);glEnableVertexAttribArray(3);glVertexAttribPointer(3,3,GL_FLOAT,0,40,(void*)28);}
void SS(const char*p){U*d=malloc(W*H*3);glReadPixels(0,0,W,H,GL_RGB,GL_UNSIGNED_BYTE,d);FILE*f=fopen(p,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(int y=H-1;y>=0;y--)fwrite(d+y*W*3,1,W*3,f);fclose(f);free(d);}
void RN(){V t={cam.x+cosf(pi)*cosf(ya),cam.y+cosf(pi)*sinf(ya),cam.z+sinf(pi)};M mo=ID(),vi=LK(cam,t,(V){0,0,1}),pj=PR(1.2f,(F)W/H,1,4000);glUniformMatrix4fv(glGetUniformLocation(prg,"m"),1,0,mo.m);glUniformMatrix4fv(glGetUniformLocation(prg,"v"),1,0,vi.m);glUniformMatrix4fv(glGetUniformLocation(prg,"j"),1,0,pj.m);glUniform3f(glGetUniformLocation(prg,"a"),cam.x,cam.y,cam.z);glUniform1f(glGetUniformLocation(prg,"g"),0.0005f);glClearColor(0.5f,0.6f,0.7f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);glBindVertexArray(vao);glDrawArrays(GL_TRIANGLES,0,nverts);SDL_GL_SwapWindow(win);}
int main(){cam=(V){0,0,300};IN();SH();MK();DR();for(int i=0;i<60;i++){RN();SDL_Delay(16);}SS("test.ppm");SDL_Quit();return 0;}
