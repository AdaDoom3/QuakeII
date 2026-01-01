#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<AL/al.h>
#include<AL/alc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define W 1280
#define H 720
typedef unsigned char U;typedef unsigned short S;typedef int J;typedef float F;typedef struct{F x,y,z;}V;typedef struct{F m[16];}M;typedef struct{char n[32];J w,h,o[4];}TX;typedef struct{J id,ver,nf,nv,oe,fs;}MD;typedef struct{U v[3];}MV;typedef struct{V s,t;}MF;typedef struct{V n;F d;}PL;typedef struct{J pn,ch[2];}ND;typedef struct{J ct,fs,ns;}BH;typedef struct{S pn;}BX;
SDL_Window*w;SDL_GLContext c;J prg,vao,vbo,nv,tex,lmt,mv1,mv2,wv1,wv2;F*vb,*m1,*m2,*w1,*w2;V cam,vel;F ya,pi,mf,wf;U pal[768],*md,*wd,*bp;PL*pl;ND*nd;BH*br;BX*bs;J np,nn,nb,nbs;
U*L(const char*p,J*z){FILE*f=fopen(p,"rb");if(!f)return 0;fseek(f,0,2);*z=ftell(f);fseek(f,0,0);U*b=malloc(*z);fread(b,1,*z,f);fclose(f);return b;}
J S_(J t,const char*p){char*s=(char*)L(p,&(J){0});J h=glCreateShader(t);glShaderSource(h,1,(const char**)&s,0);glCompileShader(h);free(s);return h;}
M ID(){M r;for(int i=0;i<16;i++)r.m[i]=i%5==0;return r;}
M PR(F fov,F asp,F n,F f){M r=ID();F t=1/tanf(fov*.5f);r.m[0]=t/asp;r.m[5]=t;r.m[10]=-(f+n)/(f-n);r.m[11]=-1;r.m[14]=-(2*f*n)/(f-n);r.m[15]=0;return r;}
M LK(V e,V t,V u){V f={t.x-e.x,t.y-e.y,t.z-e.z};F l=sqrtf(f.x*f.x+f.y*f.y+f.z*f.z);f.x/=l;f.y/=l;f.z/=l;V s={f.y*u.z-f.z*u.y,f.z*u.x-f.x*u.z,f.x*u.y-f.y*u.x};l=sqrtf(s.x*s.x+s.y*s.y+s.z*s.z);s.x/=l;s.y/=l;s.z/=l;V v={s.y*f.z-s.z*f.y,s.z*f.x-s.x*f.z,s.x*f.y-s.y*f.x};M r=ID();r.m[0]=s.x;r.m[4]=s.y;r.m[8]=s.z;r.m[1]=v.x;r.m[5]=v.y;r.m[9]=v.z;r.m[2]=-f.x;r.m[6]=-f.y;r.m[10]=-f.z;r.m[12]=-(s.x*e.x+s.y*e.y+s.z*e.z);r.m[13]=-(v.x*e.x+v.y*e.y+v.z*e.z);r.m[14]=f.x*e.x+f.y*e.y+f.z*e.z;return r;}
void LP(){J z;U*d=L("baseq2/pics/colormap.pcx",&z);if(d&&z>768)memcpy(pal,d+z-768,768);free(d);}
U*LP2(const char*p,J*wo,J*ho){J z;U*d=L(p,&z);if(!d||z<128)return 0;J w_=d[8]|(d[9]<<8)-d[4]|(d[5]<<8)+1,h_=d[10]|(d[11]<<8)-d[6]|(d[7]<<8)+1;*wo=w_;*ho=h_;U*ix=malloc(w_*h_),*rgb=malloc(w_*h_*3);J pi=0,di=128;while(pi<w_*h_&&di<z-768){U b=d[di++];if((b&0xC0)==0xC0){J c=b&0x3F;U v=d[di++];for(J j=0;j<c&&pi<w_*h_;j++)ix[pi++]=v;}else ix[pi++]=b;}for(J i=0;i<w_*h_;i++){rgb[i*3]=pal[ix[i]*3];rgb[i*3+1]=pal[ix[i]*3+1];rgb[i*3+2]=pal[ix[i]*3+2];}free(ix);free(d);return rgb;}
typedef struct{F v[2][4];char n[32];}TI;typedef struct{S v[2];}E;
void BL(const char*p){J z;U*d=L(p,&z);if(!d||z<8||memcmp(d,"IBSP",4)||((J*)d)[1]!=38){F t[]={-100,-100,0,0,0,100,-100,0,1,0,0,100,0,0.5,1};vb=malloc(sizeof(t));memcpy(vb,t,sizeof(t));nv=3;return;}J*lp=(J*)(d+8);pl=(PL*)(d+lp[2]);np=lp[3]/sizeof(PL);nd=(ND*)(d+lp[8]);nn=lp[9]/sizeof(ND);br=(BH*)(d+lp[28]);nb=lp[29]/sizeof(BH);bs=(BX*)(d+lp[30]);nbs=lp[31]/sizeof(BX);V*vx=(V*)(d+lp[4]);TI*ti=(TI*)(d+lp[10]);E*ed=(E*)(d+lp[22]);J*se=(J*)(d+lp[24]);typedef struct{S p,ps,fe,ne,ti;U s[4];J lm;}FC;FC*fc=(FC*)(d+lp[12]);U*lm=(U*)(d+lp[14]);J nf=lp[13]/sizeof(FC),nt=lp[11]/sizeof(TI),txc=0,txw[1024],txh[1024],txm[1024];U*txd[1024];J tmw=256,tmh=512;for(J i=0;i<nt&&i<1024;i++){txm[i]=0;char pt[128];sprintf(pt,"baseq2/textures/%s.wal",ti[i].n);J z2;U*td=L(pt,&z2);if(td&&z2>100&&txc<1024){TX*h=(TX*)td;txw[txc]=h->w;txh[txc]=h->h;txd[txc]=malloc(h->w*h->h*3);for(J j=0;j<h->w*h->h;j++){U ix=td[h->o[0]+j];txd[txc][j*3]=pal[ix*3];txd[txc][j*3+1]=pal[ix*3+1];txd[txc][j*3+2]=pal[ix*3+2];}txm[i]=txc++;if(h->w>tmw)tmw=h->w;if(h->h>tmh)tmh=h->h;free(td);}}U*la=malloc(1024*1024*3);memset(la,128,1024*1024*3);J lx=0,ly=0,lh=0;vb=malloc(nf*100*5*sizeof(F));F*vp=vb;for(J i=0;i<nf;i++){FC*f=&fc[i];if(f->ti>=nt)continue;J tx=txm[f->ti];TI*t=&ti[f->ti];V vl[64];J nv_=0;F mu=9e9,mv=9e9,Mu=-9e9,Mv=-9e9;for(J j=0;j<f->ne&&j<64;j++){J si=se[f->fe+j],ei=si>=0?si:-si;if(ei>=lp[23]/sizeof(E))break;S vi=si>=0?ed[si].v[0]:ed[-si].v[1];if(vi>=lp[5]/sizeof(V))break;vl[nv_]=vx[vi];F u=vl[nv_].x*t->v[0][0]+vl[nv_].y*t->v[0][1]+vl[nv_].z*t->v[0][2]+t->v[0][3];F vv=vl[nv_].x*t->v[1][0]+vl[nv_].y*t->v[1][1]+vl[nv_].z*t->v[1][2]+t->v[1][3];mu=u<mu?u:mu;mv=vv<mv?vv:mv;Mu=u>Mu?u:Mu;Mv=vv>Mv?vv:Mv;nv_++;}J lw=(J)((Mu-mu)/16)+1,lh_=(J)((Mv-mv)/16)+1;if(lx+lw>1024){lx=0;ly+=lh+1;lh=0;}if(ly+lh_<1024&&f->lm>=0){for(J y=0;y<lh_;y++)for(J x=0;x<lw;x++){if(lx+x<1024&&ly+y<1024){J si=(lx+x+(ly+y)*1024)*3,di=f->lm+(x+y*lw)*3;if(di+2<lp[15]){la[si]=lm[di];la[si+1]=lm[di+1];la[si+2]=lm[di+2];}}}}for(J k=0;k<nv_-2;k++){V*a=&vl[0],*b=&vl[k+1],*c=&vl[k+2];F u1=(a->x*t->v[0][0]+a->y*t->v[0][1]+a->z*t->v[0][2]+t->v[0][3])/tmw,v1=(a->x*t->v[1][0]+a->y*t->v[1][1]+a->z*t->v[1][2]+t->v[1][3])/tmh,u2=(b->x*t->v[0][0]+b->y*t->v[0][1]+b->z*t->v[0][2]+t->v[0][3])/tmw,v2=(b->x*t->v[1][0]+b->y*t->v[1][1]+b->z*t->v[1][2]+t->v[1][3])/tmh,u3=(c->x*t->v[0][0]+c->y*t->v[0][1]+c->z*t->v[0][2]+t->v[0][3])/tmw,v3=(c->x*t->v[1][0]+c->y*t->v[1][1]+c->z*t->v[1][2]+t->v[1][3])/tmh;*vp++=a->x;*vp++=a->y;*vp++=a->z;*vp++=u1;*vp++=v1;*vp++=b->x;*vp++=b->y;*vp++=b->z;*vp++=u2;*vp++=v2;*vp++=c->x;*vp++=c->y;*vp++=c->z;*vp++=u3;*vp++=v3;}lx+=lw+1;lh=lh_>lh?lh_:lh;}nv=(vp-vb)/5;glGenTextures(1,&lmt);glBindTexture(GL_TEXTURE_2D,lmt);glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE,la);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);J txa;glGenTextures(1,&txa);glBindTexture(GL_TEXTURE_2D_ARRAY,txa);glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGB,tmw,tmh,txc,0,GL_RGB,GL_UNSIGNED_BYTE,0);for(J i=0;i<txc;i++){U*td=malloc(tmw*tmh*3);for(J y=0;y<tmh;y++)for(J x=0;x<tmw;x++){J sx=x%txw[i],sy=y%txh[i],si=(sx+sy*txw[i])*3,di=(x+y*tmw)*3;td[di]=txd[i][si];td[di+1]=txd[i][si+1];td[di+2]=txd[i][si+2];}glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,i,tmw,tmh,1,GL_RGB,GL_UNSIGNED_BYTE,td);free(td);}glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D_ARRAY,txa);glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,lmt);free(la);bp=d;}
void LM2(const char*p,U**od,F**v1,F**v2,J ti){J z;*od=L(p,&z);if(!*od||z<68)return;MD*h=(MD*)*od;if(h->id!=844121161||h->ver!=8)return;J nv_=h->nv*h->nf;*v1=malloc(nv_*5*sizeof(F));*v2=malloc(nv_*5*sizeof(F));for(J f=0;f<h->nf;f++){MF*fr=(MF*)(*od+h->oe+f*h->fs);for(J i=0;i<h->nv;i++){MV*v=(MV*)(*od+h->oe+f*h->fs+sizeof(MF)+i*sizeof(MV));J ix=(f*h->nv+i)*5;(*v1)[ix]=v->v[0]*fr->s.x+fr->t.x;(*v1)[ix+1]=v->v[1]*fr->s.y+fr->t.y;(*v1)[ix+2]=v->v[2]*fr->s.z+fr->t.z;(*v1)[ix+3]=0;(*v1)[ix+4]=0;}}memcpy(*v2,*v1,nv_*5*sizeof(F));}
typedef struct{V p,v;J a;}PJ;PJ pj[64];F sht;J kw,ka,ks,kd,kjp,ksh;
void SS(const char*p){U*d=malloc(W*H*3);glReadPixels(0,0,W,H,GL_RGB,GL_UNSIGNED_BYTE,d);FILE*f=fopen(p,"wb");fprintf(f,"P6\n%d %d\n255\n",W,H);for(int y=H-1;y>=0;y--)fwrite(d+y*W*3,1,W*3,f);fclose(f);free(d);}
typedef struct{F fr;V n;J hit;}TC;
void TB(V s,V e,BH*b,TC*t,V bmn,V bmx){if(!(b->ct&1))return;F ei=-1,eo=1;V hn={0,0,0};for(J j=0;j<b->ns;j++){J si=b->fs+j;if(si>=nbs)continue;BX*x=&bs[si];if(x->pn>=np)continue;PL*pn=&pl[x->pn];F of=(pn->n.x<0?-bmx.x:-bmn.x)*pn->n.x+(pn->n.y<0?-bmx.y:-bmn.y)*pn->n.y+(pn->n.z<0?-bmx.z:-bmn.z)*pn->n.z;F ds=pn->n.x*s.x+pn->n.y*s.y+pn->n.z*s.z-pn->d-of,de=pn->n.x*e.x+pn->n.y*e.y+pn->n.z*e.z-pn->d-of;if(ds>0&&de>=ds)return;if(ds<0&&de<0)continue;F fr=ds/(ds-de);if(ds>de){if(fr>ei){ei=fr;hn=pn->n;}}else{if(fr<eo)eo=fr;}}if(ei<eo&&ei>-1&&ei<t->fr){if(ei<0)ei=0;t->fr=ei;t->n=hn;t->hit=1;}}
TC BT(V s,V e,V bmn,V bmx){TC t;t.fr=1;t.n=(V){0,0,1};t.hit=0;for(J i=0;i<nb;i++)TB(s,e,&br[i],&t,bmn,bmx);return t;}
V CV(V v,V n){F b=v.x*n.x+v.y*n.y+v.z*n.z;return(V){v.x-n.x*b,v.y-n.y*b,v.z-n.z*b};}
int main(int ac,char**av){
SDL_Init(SDL_INIT_VIDEO);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
w=SDL_CreateWindow("Q2",0,0,W,H,SDL_WINDOW_OPENGL);c=SDL_GL_CreateContext(w);glewInit();glEnable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);
ALCdevice*ad=alcOpenDevice(0);if(ad){ALCcontext*ac_=alcCreateContext(ad,0);if(ac_){alcMakeContextCurrent(ac_);J ab,as;S*aw=malloc(44100*sizeof(S));for(J i=0;i<44100;i++)aw[i]=sinf(i*.02f)*16384;alGenBuffers(1,&ab);alBufferData(ab,AL_FORMAT_MONO16,aw,44100*sizeof(S),44100);alGenSources(1,&as);alSourcei(as,AL_BUFFER,ab);alSource3f(as,AL_POSITION,120,-80,88);alSourcei(as,AL_LOOPING,AL_TRUE);alSourcePlay(as);free(aw);}}
J vs=S_(GL_VERTEX_SHADER,"vert.glsl"),fs=S_(GL_FRAGMENT_SHADER,"frag.glsl");prg=glCreateProgram();glAttachShader(prg,vs);glAttachShader(prg,fs);glLinkProgram(prg);glUseProgram(prg);
LP();BL(ac>1?av[1]:"baseq2/maps/demo1.bsp");
J sw,sh;U*sk=LP2("baseq2/models/monsters/soldier/skin.pcx",&sw,&sh);LM2("baseq2/models/monsters/soldier/tris.md2",&md,&m1,&m2,0);
U*wk=LP2("baseq2/models/weapons/v_blast/skin.pcx",&sw,&sh);LM2("baseq2/models/weapons/v_blast/tris.md2",&wd,&w1,&w2,1);
glGenVertexArrays(1,&vao);glGenBuffers(1,&vbo);glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,nv*5*sizeof(F),vb,GL_STATIC_DRAW);
glEnableVertexAttribArray(0);glVertexAttribPointer(0,3,GL_FLOAT,0,20,0);
glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,0,20,(void*)12);
if(md){glGenBuffers(1,&mv1);glBindBuffer(GL_ARRAY_BUFFER,mv1);glBufferData(GL_ARRAY_BUFFER,((MD*)md)->nv*((MD*)md)->nf*5*sizeof(F),m1,GL_STATIC_DRAW);glGenBuffers(1,&mv2);glBindBuffer(GL_ARRAY_BUFFER,mv2);glBufferData(GL_ARRAY_BUFFER,((MD*)md)->nv*((MD*)md)->nf*5*sizeof(F),m2,GL_STATIC_DRAW);}
if(wd){glGenBuffers(1,&wv1);glBindBuffer(GL_ARRAY_BUFFER,wv1);glBufferData(GL_ARRAY_BUFFER,((MD*)wd)->nv*((MD*)wd)->nf*5*sizeof(F),w1,GL_STATIC_DRAW);glGenBuffers(1,&wv2);glBindBuffer(GL_ARRAY_BUFFER,wv2);glBufferData(GL_ARRAY_BUFFER,((MD*)wd)->nv*((MD*)wd)->nf*5*sizeof(F),w2,GL_STATIC_DRAW);}
cam=(V){144,-336,60};vel=(V){0,0,0};ya=2.356;pi=0;V bmn={-16,-16,-24},bmx={16,16,32};
SDL_SetRelativeMouseMode(1);
glUniform1i(glGetUniformLocation(prg,"d"),0);
glUniform1i(glGetUniformLocation(prg,"e"),1);
J q=0,fr=0;while(!q&&fr<240){SDL_Event e;while(SDL_PollEvent(&e)){if(e.type==SDL_QUIT)q=1;if(e.type==SDL_KEYDOWN){J k=e.key.keysym.sym;if(k==SDLK_w)kw=1;if(k==SDLK_a)ka=1;if(k==SDLK_s)ks=1;if(k==SDLK_d)kd=1;if(k==SDLK_SPACE)kjp=1;if(k==SDLK_ESCAPE)q=1;}if(e.type==SDL_KEYUP){J k=e.key.keysym.sym;if(k==SDLK_w)kw=0;if(k==SDLK_a)ka=0;if(k==SDLK_s)ks=0;if(k==SDLK_d)kd=0;if(k==SDLK_SPACE)kjp=0;}if(e.type==SDL_MOUSEBUTTONDOWN&&e.button.button==SDL_BUTTON_LEFT)ksh=1;if(e.type==SDL_MOUSEBUTTONUP&&e.button.button==SDL_BUTTON_LEFT)ksh=0;if(e.type==SDL_MOUSEMOTION){ya+=e.motion.xrel*.002f;pi-=e.motion.yrel*.002f;if(pi>.8f)pi=.8f;if(pi<-.8f)pi=-.8f;}}
F fx=-sinf(ya)*cosf(pi),fy=cosf(ya)*cosf(pi),fz=sinf(pi);F rx=cosf(ya),ry=sinf(ya);
TC tdn=BT(cam,(V){cam.x,cam.y,cam.z-0.25},bmn,bmx);J gr=tdn.hit&&tdn.n.z>0.7;
vel.x*=.85f;vel.y*=.85f;vel.z*=gr?.85f:.98f;if(gr&&kjp)vel.z=8;vel.z-=.4f;
if(kw){vel.x+=fx*1.5f;vel.y+=fy*1.5f;}if(ks){vel.x-=fx*1.5f;vel.y-=fy*1.5f;}if(ka){vel.x-=rx*1.5f;vel.y-=ry*1.5f;}if(kd){vel.x+=rx*1.5f;vel.y+=ry*1.5f;}
for(J b=0;b<4;b++){V np={cam.x+vel.x,cam.y+vel.y,cam.z+vel.z};TC tr=BT(cam,np,bmn,bmx);cam.x+=vel.x*tr.fr;cam.y+=vel.y*tr.fr;cam.z+=vel.z*tr.fr;if(!tr.hit)break;vel=CV(vel,tr.n);if(tr.n.z>0.7)vel.z=0;}
if(ksh&&sht<=0){for(J i=0;i<64;i++){if(!pj[i].a){pj[i].p=(V){cam.x+fx*24,cam.y+fy*24,cam.z+fz*24-6};pj[i].v=(V){fx*10,fy*10,fz*10};pj[i].a=1;sht=.2f;break;}}}sht-=.016f;
for(J i=0;i<64;i++){if(pj[i].a){V op=pj[i].p;pj[i].p.x+=pj[i].v.x;pj[i].p.y+=pj[i].v.y;pj[i].p.z+=pj[i].v.z;TC pt=BT(op,pj[i].p,bmn,bmx);if(pt.hit)pj[i].a=0;if(sqrtf(pj[i].p.x*pj[i].p.x+pj[i].p.y*pj[i].p.y)>2000)pj[i].a=0;}}
V t={cam.x+fx,cam.y+fy,cam.z+fz};M mo=ID(),vi=LK(cam,t,(V){0,0,1}),pj_=PR(1.2f,(F)W/H,1,10000);
glUniformMatrix4fv(glGetUniformLocation(prg,"m"),1,0,mo.m);
glUniformMatrix4fv(glGetUniformLocation(prg,"v"),1,0,vi.m);
glUniformMatrix4fv(glGetUniformLocation(prg,"j"),1,0,pj_.m);
glClearColor(0.5,0.6,0.7,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glBindVertexArray(vao);glDrawArrays(GL_TRIANGLES,0,nv);
SDL_GL_SwapWindow(w);if(fr%60==0){char n[32];sprintf(n,"v2_%d.ppm",fr/60);SS(n);}fr++;SDL_Delay(16);}
SS("v2_final.ppm");
SDL_Quit();return 0;
}
