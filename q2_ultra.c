#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define W 1280
#define H 720
typedef unsigned char U;typedef float F;typedef int I;typedef struct{F x,y,z;}V;typedef struct{F m[16];}M;
SDL_Window*w;SDL_GLContext c;I p,o,b,n,t,k[256];F*v,*d;V a,e;F y,i,f;U l[768];
U*L(char*s,I*z){FILE*f=fopen(s,"rb");if(!f)return 0;fseek(f,0,2);*z=ftell(f);rewind(f);U*b=malloc(*z);fread(b,1,*z,f);fclose(f);return b;}
I S(I t,char*s){char*r=(char*)L(s,&(I){0});I h=glCreateShader(t);glShaderSource(h,1,(const char**)&r,0);glCompileShader(h);free(r);return h;}
M ID(){M m;for(I i=0;i<16;i++)m.m[i]=i%5==0;return m;}
M PR(F fv,F a,F n,F f){M m=ID();F t=1/tanf(fv*.5f);m.m[0]=t/a;m.m[5]=t;m.m[10]=-(f+n)/(f-n);m.m[11]=-1;m.m[14]=-(2*f*n)/(f-n);m.m[15]=0;return m;}
M LK(V e,V t,V u){V f={t.x-e.x,t.y-e.y,t.z-e.z},s;F l=sqrtf(f.x*f.x+f.y*f.y+f.z*f.z);f.x/=l;f.y/=l;f.z/=l;s.x=f.y*u.z-f.z*u.y;s.y=f.z*u.x-f.x*u.z;s.z=f.x*u.y-f.y*u.x;l=sqrtf(s.x*s.x+s.y*s.y+s.z*s.z);s.x/=l;s.y/=l;s.z/=l;V r={s.y*f.z-s.z*f.y,s.z*f.x-s.x*f.z,s.x*f.y-s.y*f.x};M m=ID();m.m[0]=s.x;m.m[4]=s.y;m.m[8]=s.z;m.m[1]=r.x;m.m[5]=r.y;m.m[9]=r.z;m.m[2]=-f.x;m.m[6]=-f.y;m.m[10]=-f.z;m.m[12]=-(s.x*e.x+s.y*e.y+s.z*e.z);m.m[13]=-(r.x*e.x+r.y*e.y+r.z*e.z);m.m[14]=f.x*e.x+f.y*e.y+f.z*e.z;return m;}
void LP(){I z;U*d=L("baseq2/pics/colormap.pcx",&z);if(d&&z>768)memcpy(l,d+z-768,768);free(d);}
void BL(char*s){I z;U*d=L(s,&z);if(!d||memcmp(d,"IBSP",4)){v=malloc(90*sizeof(F));F q[]={-100,-100,0,0,0,0,1,100,-100,0,1,0,0,1,0,100,0,1,1,0,1};memcpy(v,q,sizeof(q));n=3;return;}I*lp=(I*)(d+8);V*vx=(V*)(d+lp[4]);typedef struct{F v[2][4];char n[32];}TI;TI*ti=(TI*)(d+lp[10]);typedef struct{unsigned short v[2];}E;E*ed=(E*)(d+lp[22]);I*se=(I*)(d+lp[24]);typedef struct{unsigned short p,ps,fe,ne,ti;U s[4];I lm;}FC;FC*fc=(FC*)(d+lp[12]);I nf=lp[13]/60,nt=lp[11]/72;typedef struct{char n[32];I w,h,o[4];}TX;I txc=0,txw[1024],txh[1024];U*txd[1024];for(I i=0;i<nt&&i<1024;i++){char pt[128];sprintf(pt,"baseq2/textures/%s.wal",ti[i].n);I z2;U*td=L(pt,&z2);if(td&&z2>100&&txc<1024){TX*h=(TX*)td;txw[txc]=h->w;txh[txc]=h->h;txd[txc]=malloc(h->w*h->h*3);for(I j=0;j<h->w*h->h;j++){U ix=td[h->o[0]+j];txd[txc][j*3]=l[ix*3];txd[txc][j*3+1]=l[ix*3+1];txd[txc][j*3+2]=l[ix*3+2];}txc++;free(td);}}v=malloc(nf*300*sizeof(F));F*vp=v;for(I i=0;i<nf;i++){FC*g=&fc[i];if(g->ti>=nt)continue;TI*r=&ti[g->ti];V vl[64];I nv=0;for(I j=0;j<g->ne&&j<64;j++){I si=se[g->fe+j],ei=si>=0?si:-si;if(ei>=lp[23]/4)break;unsigned short vi=si>=0?ed[si].v[0]:ed[-si].v[1];if(vi>=lp[5]/12)break;vl[nv++]=vx[vi];}for(I j=0;j<nv-2;j++){V*x=&vl[0],*s=&vl[j+1],*m=&vl[j+2];*vp++=x->x;*vp++=x->y;*vp++=x->z;*vp++=(x->x*r->v[0][0]+x->y*r->v[0][1]+x->z*r->v[0][2]+r->v[0][3])/256;*vp++=(x->x*r->v[1][0]+x->y*r->v[1][1]+x->z*r->v[1][2]+r->v[1][3])/256;*vp++=g->ti;*vp++=s->x;*vp++=s->y;*vp++=s->z;*vp++=(s->x*r->v[0][0]+s->y*r->v[0][1]+s->z*r->v[0][2]+r->v[0][3])/256;*vp++=(s->x*r->v[1][0]+s->y*r->v[1][1]+s->z*r->v[1][2]+r->v[1][3])/256;*vp++=g->ti;*vp++=m->x;*vp++=m->y;*vp++=m->z;*vp++=(m->x*r->v[0][0]+m->y*r->v[0][1]+m->z*r->v[0][2]+r->v[0][3])/256;*vp++=(m->x*r->v[1][0]+m->y*r->v[1][1]+m->z*r->v[1][2]+r->v[1][3])/256;*vp++=g->ti;}}n=(vp-v)/6;glGenTextures(1,&t);glBindTexture(GL_TEXTURE_2D_ARRAY,t);glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGB,256,256,txc,0,GL_RGB,GL_UNSIGNED_BYTE,0);for(I i=0;i<txc;i++){U*td=malloc(256*256*3);for(I y=0;y<256;y++)for(I x=0;x<256;x++){I sx=x%txw[i],sy=y%txh[i],si=(sx+sy*txw[i])*3,di=(x+y*256)*3;td[di]=txd[i][si];td[di+1]=txd[i][si+1];td[di+2]=txd[i][si+2];}glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,i,256,256,1,GL_RGB,GL_UNSIGNED_BYTE,td);free(td);}glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);free(d);}
void SS(char*s){U*d=malloc(W*H*3);glReadPixels(0,0,W,H,GL_RGB,GL_UNSIGNED_BYTE,d);FILE*f=fopen(s,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(I y=H-1;y>=0;y--)fwrite(d+y*W*3,1,W*3,f);fclose(f);free(d);}
int main(I ac,char**av){
SDL_Init(SDL_INIT_VIDEO);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
w=SDL_CreateWindow("Q2",0,0,W,H,SDL_WINDOW_OPENGL);c=SDL_GL_CreateContext(w);glewInit();glEnable(GL_DEPTH_TEST);
I vs=S(GL_VERTEX_SHADER,"vert_ultra.glsl"),fs=S(GL_FRAGMENT_SHADER,"frag_ultra.glsl");p=glCreateProgram();glAttachShader(p,vs);glAttachShader(p,fs);glLinkProgram(p);glUseProgram(p);
LP();BL(ac>1?av[1]:"baseq2/maps/demo1.bsp");
glGenVertexArrays(1,&o);glGenBuffers(1,&b);glBindVertexArray(o);glBindBuffer(GL_ARRAY_BUFFER,b);glBufferData(GL_ARRAY_BUFFER,n*6*sizeof(F),v,GL_STATIC_DRAW);
glEnableVertexAttribArray(0);glVertexAttribPointer(0,3,GL_FLOAT,0,24,0);
glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,0,24,(void*)12);
glEnableVertexAttribArray(2);glVertexAttribPointer(2,1,GL_FLOAT,0,24,(void*)20);
a=(V){0,0,60};e=(V){0,0,0};y=0;i=0;SDL_SetRelativeMouseMode(1);
glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D_ARRAY,t);glUniform1i(glGetUniformLocation(p,"tx"),0);
I q=0,r=0;while(!q&&r<240){SDL_Event v;while(SDL_PollEvent(&v)){if(v.type==SDL_QUIT)q=1;if(v.type==SDL_KEYDOWN){k[v.key.keysym.sym]=1;if(v.key.keysym.sym==SDLK_ESCAPE)q=1;}if(v.type==SDL_KEYUP)k[v.key.keysym.sym]=0;if(v.type==SDL_MOUSEMOTION){y+=v.motion.xrel*.002f;i-=v.motion.yrel*.002f;if(i>.8f)i=.8f;if(i<-.8f)i=-.8f;}}
F fx=-sinf(y)*cosf(i),fy=cosf(y)*cosf(i),fz=sinf(i),rx=cosf(y),ry=sinf(y);
e.x*=.9f;e.y*=.9f;e.z-=.4f;
if(k[SDLK_w]){e.x+=fx;e.y+=fy;}if(k[SDLK_s]){e.x-=fx;e.y-=fy;}if(k[SDLK_a]){e.x-=rx;e.y-=ry;}if(k[SDLK_d]){e.x+=rx;e.y+=ry;}
a.x+=e.x;a.y+=e.y;a.z+=e.z;if(a.z<40){a.z=40;e.z=0;}
V g={a.x+fx,a.y+fy,a.z+fz};M m=ID(),u=LK(a,g,(V){0,0,1}),j=PR(1.2f,(F)W/H,1,10000);
glUniformMatrix4fv(glGetUniformLocation(p,"m"),1,0,m.m);
glUniformMatrix4fv(glGetUniformLocation(p,"v"),1,0,u.m);
glUniformMatrix4fv(glGetUniformLocation(p,"j"),1,0,j.m);
glClearColor(.5,.6,.7,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glBindVertexArray(o);glDrawArrays(GL_TRIANGLES,0,n);
SDL_GL_SwapWindow(w);if(r%60==0){char s[32];sprintf(s,"ultra_%d.ppm",r/60);SS(s);}r++;SDL_Delay(16);}
SS("ultra_final.ppm");SDL_Quit();return 0;
}
