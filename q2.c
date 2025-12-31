#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define W 1280
#define H 720
typedef unsigned char U;typedef unsigned short S;typedef unsigned int I;typedef int J;typedef float F;typedef struct{F x,y,z;}V;typedef struct{F m[16];}M;typedef struct{I o,l;}L;typedef struct{S v[2];}E;typedef struct{J e;U d;}FE;typedef struct{S p,ps;J fe;S ne,ti;U s[4];J lm;}FC;typedef struct{char n[32];I w,h,o[4];char a[32];J f,c,v;}TX;typedef struct{F v[2][4];I f,val;char n[32];J nx;}TI;
SDL_Window*w;SDL_GLContext c;I prg,vao,vbo,nv,tex,lmt;F*vb;V cam;F ya,pi;U pal[768];
U*LF(const char*p,I*z){FILE*f=fopen(p,"rb");if(!f)return 0;fseek(f,0,2);*z=ftell(f);fseek(f,0,0);U*b=malloc(*z);fread(b,1,*z,f);fclose(f);return b;}
I CS(I t,const char*p){char*s=(char*)LF(p,&(I){0});I h=glCreateShader(t);glShaderSource(h,1,(const char**)&s,0);glCompileShader(h);free(s);return h;}
M ID(){M r;for(int i=0;i<16;i++)r.m[i]=i%5==0;return r;}
M PR(F fov,F asp,F n,F f){M r=ID();F t=1/tanf(fov*.5f);r.m[0]=t/asp;r.m[5]=t;r.m[10]=-(f+n)/(f-n);r.m[11]=-1;r.m[14]=-(2*f*n)/(f-n);r.m[15]=0;return r;}
M LK(V e,V t,V u){V f={t.x-e.x,t.y-e.y,t.z-e.z};F l=sqrtf(f.x*f.x+f.y*f.y+f.z*f.z);f.x/=l;f.y/=l;f.z/=l;V s={f.y*u.z-f.z*u.y,f.z*u.x-f.x*u.z,f.x*u.y-f.y*u.x};l=sqrtf(s.x*s.x+s.y*s.y+s.z*s.z);s.x/=l;s.y/=l;s.z/=l;V v={s.y*f.z-s.z*f.y,s.z*f.x-s.x*f.z,s.x*f.y-s.y*f.x};M r=ID();r.m[0]=s.x;r.m[4]=s.y;r.m[8]=s.z;r.m[1]=v.x;r.m[5]=v.y;r.m[9]=v.z;r.m[2]=-f.x;r.m[6]=-f.y;r.m[10]=-f.z;r.m[12]=-(s.x*e.x+s.y*e.y+s.z*e.z);r.m[13]=-(v.x*e.x+v.y*e.y+v.z*e.z);r.m[14]=f.x*e.x+f.y*e.y+f.z*e.z;return r;}
void LP(){I sz;U*d=LF("baseq2/pics/colormap.pcx",&sz);if(d&&sz>768){memcpy(pal,d+sz-768,768);free(d);printf("Palette: loaded\n");}else{free(d);printf("Palette: failed\n");}}
I LW(const char*p){I sz;U*d=LF(p,&sz);if(!d||sz<100)return 0;TX*h=(TX*)d;I tw=h->w,th=h->h;U*rgb=malloc(tw*th*3);for(I i=0;i<tw*th;i++){U idx=d[h->o[0]+i];rgb[i*3]=pal[idx*3];rgb[i*3+1]=pal[idx*3+1];rgb[i*3+2]=pal[idx*3+2];}I tx;glGenTextures(1,&tx);glBindTexture(GL_TEXTURE_2D,tx);glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tw,th,0,GL_RGB,GL_UNSIGNED_BYTE,rgb);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);printf("WAL: %dx%d\n",tw,th);free(rgb);free(d);return tx;}
void BS(const char*p){I sz;U*d=LF(p,&sz);if(!d||sz<8||memcmp(d,"IBSP",4)||((I*)d)[1]!=38){F t[]={-100,-100,0,0,0,0.5,0.5,0,1,0,100,-100,0,1,0,0.5,0.5,0,1,0,0,100,0,0.5,1,0.5,0.5,0,1,0};vb=malloc(sizeof(t));memcpy(vb,t,sizeof(t));nv=3;return;}L*lp=(L*)(d+8);V*vx=(V*)(d+lp[2].o);TI*ti=(TI*)(d+lp[5].o);E*ed=(E*)(d+lp[11].o);J*se=(J*)(d+lp[12].o);FC*fc=(FC*)(d+lp[6].o);U*lm=(U*)(d+lp[7].o);I nf=lp[6].l/sizeof(FC);U*la=malloc(512*512*3);memset(la,128,512*512*3);I lx=0,ly=0,lh=0;vb=malloc(nf*100*10*sizeof(F));F*vp=vb;F mx=-9e9,my=-9e9,mz=-9e9,Mx=9e9,My=9e9,Mz=9e9;for(I i=0;i<nf;i++){FC*f=&fc[i];TI*tx=&ti[f->ti];V vl[64];I nv_=0;F mu=9e9,mv=9e9,Mu=-9e9,Mv=-9e9;for(I j=0;j<f->ne&&j<64;j++){J si=se[f->fe+j];S vi=si>=0?ed[si].v[0]:ed[-si].v[1];vl[nv_]=vx[vi];F u=vl[nv_].x*tx->v[0][0]+vl[nv_].y*tx->v[0][1]+vl[nv_].z*tx->v[0][2]+tx->v[0][3];F vv=vl[nv_].x*tx->v[1][0]+vl[nv_].y*tx->v[1][1]+vl[nv_].z*tx->v[1][2]+tx->v[1][3];mu=u<mu?u:mu;mv=vv<mv?vv:mv;Mu=u>Mu?u:Mu;Mv=vv>Mv?vv:Mv;nv_++;}I lw=(I)((Mu-mu)/16)+1,lh_=(I)((Mv-mv)/16)+1;if(lx+lw>512){lx=0;ly+=lh+1;lh=0;}if(f->lm>=0&&f->lm<lp[7].l){for(I y=0;y<lh_;y++)for(I x=0;x<lw;x++){I si=(lx+x+(ly+y)*512)*3,di=f->lm+(x+y*lw)*3;if(di+2<lp[7].l){la[si]=lm[di];la[si+1]=lm[di+1];la[si+2]=lm[di+2];}}}for(I t=0;t<nv_-2;t++){for(I k=0;k<3;k++){V*v=k==0?&vl[0]:k==1?&vl[t+1]:&vl[t+2];F u=(v->x*tx->v[0][0]+v->y*tx->v[0][1]+v->z*tx->v[0][2]+tx->v[0][3])/256.0f;F vv=(v->x*tx->v[1][0]+v->y*tx->v[1][1]+v->z*tx->v[1][2]+tx->v[1][3])/512.0f;F lu=(v->x*tx->v[0][0]+v->y*tx->v[0][1]+v->z*tx->v[0][2]+tx->v[0][3]-mu)/16.0f;F lv=(v->x*tx->v[1][0]+v->y*tx->v[1][1]+v->z*tx->v[1][2]+tx->v[1][3]-mv)/16.0f;mx=v->x>mx?v->x:mx;my=v->y>my?v->y:my;mz=v->z>mz?v->z:mz;Mx=v->x<Mx?v->x:Mx;My=v->y<My?v->y:My;Mz=v->z<Mz?v->z:Mz;*vp++=v->x;*vp++=v->y;*vp++=v->z;*vp++=u;*vp++=vv;*vp++=(lx+lu)/512.0f;*vp++=(ly+lv)/512.0f;*vp++=1;*vp++=1;*vp++=1;}}lx+=lw+1;lh=lh_>lh?lh_:lh;}nv=(vp-vb)/10;glGenTextures(1,&lmt);glBindTexture(GL_TEXTURE_2D,lmt);glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE,la);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);free(la);printf("BSP: %d faces -> %d tris, bounds:[%.0f,%.0f] [%.0f,%.0f] [%.0f,%.0f]\n",nf,nv/3,Mx,mx,My,my,Mz,mz);free(d);}
void SS(const char*p){U*d=malloc(W*H*3);glReadPixels(0,0,W,H,GL_RGB,GL_UNSIGNED_BYTE,d);FILE*f=fopen(p,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(int y=H-1;y>=0;y--)fwrite(d+y*W*3,1,W*3,f);fclose(f);free(d);printf("Saved: %s\n",p);}
int main(int ac,char**av){
SDL_Init(SDL_INIT_VIDEO);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
w=SDL_CreateWindow("Q2",0,0,W,H,SDL_WINDOW_OPENGL);c=SDL_GL_CreateContext(w);glewInit();glEnable(GL_DEPTH_TEST);
I vs=CS(GL_VERTEX_SHADER,"vert.glsl"),fs=CS(GL_FRAGMENT_SHADER,"frag.glsl");prg=glCreateProgram();glAttachShader(prg,vs);glAttachShader(prg,fs);glLinkProgram(prg);glUseProgram(prg);
LP();tex=LW("baseq2/textures/eq2/wall.wal");
BS(ac>1?av[1]:"baseq2/maps/sample.bsp");
glGenVertexArrays(1,&vao);glGenBuffers(1,&vbo);glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,nv*10*sizeof(F),vb,GL_STATIC_DRAW);
glEnableVertexAttribArray(0);glVertexAttribPointer(0,3,GL_FLOAT,0,40,0);
glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,0,40,(void*)12);
glEnableVertexAttribArray(2);glVertexAttribPointer(2,2,GL_FLOAT,0,40,(void*)20);
glEnableVertexAttribArray(3);glVertexAttribPointer(3,3,GL_FLOAT,0,40,(void*)28);
cam=(V){0,0,600};glDisable(GL_CULL_FACE);
glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,tex);glUniform1i(glGetUniformLocation(prg,"d"),0);
glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,lmt);glUniform1i(glGetUniformLocation(prg,"e"),1);
for(int i=0;i<60;i++){
V t={120,-80,88};
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
