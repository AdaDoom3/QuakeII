#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define W 1280
#define H 720
typedef unsigned char U;typedef unsigned short S;typedef unsigned int I;typedef int J;typedef float F;typedef struct{F x,y,z;}V;typedef struct{F m[16];}M;typedef struct{I o,l;}L;typedef struct{S v[2];}E;typedef struct{J e;U d;}FE;typedef struct{S p,ps;J fe;S ne,ti;U s[4];J lm;}FC;
SDL_Window*w;SDL_GLContext c;I prg,vao,vbo,nv;F*vb;V cam;F ya,pi;
U*LF(const char*p,I*z){FILE*f=fopen(p,"rb");if(!f)return 0;fseek(f,0,2);*z=ftell(f);fseek(f,0,0);U*b=malloc(*z);fread(b,1,*z,f);fclose(f);return b;}
I CS(I t,const char*p){char*s=(char*)LF(p,&(I){0});I h=glCreateShader(t);glShaderSource(h,1,(const char**)&s,0);glCompileShader(h);free(s);return h;}
M ID(){M r;for(int i=0;i<16;i++)r.m[i]=i%5==0;return r;}
M PR(F fov,F asp,F n,F f){M r=ID();F t=1/tanf(fov*.5f);r.m[0]=t/asp;r.m[5]=t;r.m[10]=-(f+n)/(f-n);r.m[11]=-1;r.m[14]=-(2*f*n)/(f-n);r.m[15]=0;return r;}
M LK(V e,V t,V u){V f={t.x-e.x,t.y-e.y,t.z-e.z};F l=sqrtf(f.x*f.x+f.y*f.y+f.z*f.z);f.x/=l;f.y/=l;f.z/=l;V s={f.y*u.z-f.z*u.y,f.z*u.x-f.x*u.z,f.x*u.y-f.y*u.x};l=sqrtf(s.x*s.x+s.y*s.y+s.z*s.z);s.x/=l;s.y/=l;s.z/=l;V v={s.y*f.z-s.z*f.y,s.z*f.x-s.x*f.z,s.x*f.y-s.y*f.x};M r=ID();r.m[0]=s.x;r.m[4]=s.y;r.m[8]=s.z;r.m[1]=v.x;r.m[5]=v.y;r.m[9]=v.z;r.m[2]=-f.x;r.m[6]=-f.y;r.m[10]=-f.z;r.m[12]=-(s.x*e.x+s.y*e.y+s.z*e.z);r.m[13]=-(v.x*e.x+v.y*e.y+v.z*e.z);r.m[14]=f.x*e.x+f.y*e.y+f.z*e.z;return r;}
void BS(const char*p){I sz;U*d=LF(p,&sz);if(!d||sz<8||memcmp(d,"IBSP",4)||((I*)d)[1]!=38){F t[]={-100,-100,0,0,0,0.5,0.5,0,1,0,100,-100,0,1,0,0.5,0.5,0,1,0,0,100,0,0.5,1,0.5,0.5,0,1,0};vb=malloc(sizeof(t));memcpy(vb,t,sizeof(t));nv=3;return;}L*lp=(L*)(d+8);V*vx=(V*)(d+lp[2].o);I nve=lp[2].l/12;vb=malloc(nve*10*sizeof(F));F*vp=vb;for(I i=0;i<nve-2;i+=3){for(I k=0;k<3;k++){V*v=&vx[i+k];*vp++=v->x;*vp++=v->y;*vp++=v->z;*vp++=0;*vp++=0;*vp++=0.5;*vp++=0.5;*vp++=0;*vp++=1;*vp++=0;}}nv=(vp-vb)/10;free(d);printf("BSP: %d verts -> %d tris\n",nve,nv/3);}
void SS(const char*p){U*d=malloc(W*H*3);glReadPixels(0,0,W,H,GL_RGB,GL_UNSIGNED_BYTE,d);FILE*f=fopen(p,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(int y=H-1;y>=0;y--)fwrite(d+y*W*3,1,W*3,f);fclose(f);free(d);printf("Saved: %s\n",p);}
int main(int ac,char**av){
SDL_Init(SDL_INIT_VIDEO);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
w=SDL_CreateWindow("Q2",0,0,W,H,SDL_WINDOW_OPENGL);c=SDL_GL_CreateContext(w);glewInit();glEnable(GL_DEPTH_TEST);
I vs=CS(GL_VERTEX_SHADER,"vert.glsl"),fs=CS(GL_FRAGMENT_SHADER,"frag.glsl");prg=glCreateProgram();glAttachShader(prg,vs);glAttachShader(prg,fs);glLinkProgram(prg);glUseProgram(prg);
BS(ac>1?av[1]:"baseq2/maps/sample.bsp");
glGenVertexArrays(1,&vao);glGenBuffers(1,&vbo);glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,nv*10*sizeof(F),vb,GL_STATIC_DRAW);
glEnableVertexAttribArray(0);glVertexAttribPointer(0,3,GL_FLOAT,0,40,0);
glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,0,40,(void*)12);
glEnableVertexAttribArray(2);glVertexAttribPointer(2,2,GL_FLOAT,0,40,(void*)20);
glEnableVertexAttribArray(3);glVertexAttribPointer(3,3,GL_FLOAT,0,40,(void*)28);
cam=(V){0,0,100};glDisable(GL_CULL_FACE);
for(int i=0;i<60;i++){
V t={0,0,0};
M mo=ID(),vi=LK(cam,t,(V){0,0,1}),pj=PR(1.2f,(F)W/H,1,10000);
glUniformMatrix4fv(glGetUniformLocation(prg,"m"),1,0,mo.m);
glUniformMatrix4fv(glGetUniformLocation(prg,"v"),1,0,vi.m);
glUniformMatrix4fv(glGetUniformLocation(prg,"j"),1,0,pj.m);
glUniform3f(glGetUniformLocation(prg,"a"),cam.x,cam.y,cam.z);
glUniform1f(glGetUniformLocation(prg,"g"),0.0005f);
glClearColor(0.5,0.6,0.7,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glDrawArrays(GL_TRIANGLES,0,nv);
SDL_GL_SwapWindow(w);SDL_Delay(16);
}
SS("bsp_render.ppm");
SDL_Quit();return 0;
}
