#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define W 1280
#define H 720
#define P 3.14159265f
typedef unsigned char u8;typedef unsigned short u16;typedef unsigned int u32;typedef int i32;
typedef struct{float x,y,z;}v3;typedef struct{i32 o,l;}lmp;typedef struct{i32 id,v;lmp l[19];}bhd;
typedef struct{v3 p;v3 n;float u,v;}vx;
GLFWwindow*w;GLuint sp,vao,vbo,tex;float px,py,pz,rx,ry,t;int k[512],nv;double mx,my,lx,ly;u8*bd;vx*verts;
char*ld(char*f){FILE*F=fopen(f,"rb");if(!F)return 0;fseek(F,0,2);long s=ftell(F);fseek(F,0,0);char*b=malloc(s+1);fread(b,1,s,F);b[s]=0;fclose(F);return b;}
u8*lb(char*f,long*z){FILE*F=fopen(f,"rb");if(!F)return 0;fseek(F,0,2);*z=ftell(F);fseek(F,0,0);u8*b=malloc(*z);fread(b,1,*z,F);fclose(F);return b;}
GLuint cs(char*s,int t){GLuint h=glCreateShader(t);glShaderSource(h,1,(const char**)&s,0);glCompileShader(h);return h;}
void mm(float*r,float*a,float*b){int i,j,k;for(i=0;i<4;i++)for(j=0;j<4;j++){r[i*4+j]=0;for(k=0;k<4;k++)r[i*4+j]+=a[i*4+k]*b[k*4+j];}}
void mp(float*m,float f,float a,float n,float F){memset(m,0,64);float g=1/tanf(f/2);m[0]=g/a;m[5]=g;m[10]=-(F+n)/(F-n);m[11]=-1;m[14]=-2*F*n/(F-n);}
void ml(float*m,float ex,float ey,float ez,float cx,float cy,float cz){
float fx=cx-ex,fy=cy-ey,fz=cz-ez,l=sqrtf(fx*fx+fy*fy+fz*fz);fx/=l;fy/=l;fz/=l;
float sx=fz,sy=0,sz=-fx;l=sqrtf(sx*sx+sz*sz);sx/=l;sz/=l;
float ux=sy*fz-sz*fy,uy=sz*fx-sx*fz,uz=sx*fy-sy*fx;
memset(m,0,64);m[0]=sx;m[4]=sy;m[8]=sz;m[1]=ux;m[5]=uy;m[9]=uz;m[2]=-fx;m[6]=-fy;m[10]=-fz;
m[12]=-(sx*ex+sy*ey+sz*ez);m[13]=-(ux*ex+uy*ey+uz*ez);m[14]=fx*ex+fy*ey+fz*ez;m[15]=1;
}
void kc(GLFWwindow*ww,int K,int s,int a,int m){if(a==1)k[K]=1;else if(a==0)k[K]=0;}
void mc(GLFWwindow*ww,double x,double y){mx=x;my=y;}
void bsp(char*fn){long z;bd=lb(fn,&z);if(!bd){printf("No BSP\n");return;}
bhd*h=(bhd*)bd;if(h->id!=0x50534249){printf("Bad BSP\n");return;}
v3*bv=(v3*)(bd+h->l[2].o);i32*ei=(i32*)(bd+h->l[11].o);
typedef struct{u16 p,n,t;}fc;fc*f=(fc*)(bd+h->l[6].o);int nf=h->l[6].l/sizeof(fc);
nv=0;for(int i=0;i<nf;i++)nv+=(f[i].n-2)*3;
verts=malloc(nv*sizeof(vx));int vi=0;
for(int i=0;i<nf;i++){fc*ff=&f[i];for(int j=2;j<ff->n;j++){
for(int q=0;q<3;q++){int idx=q==0?0:(q==1?j-1:j);v3*p=&bv[ei[ff->p+idx]];
verts[vi].p=*p;verts[vi].n=(v3){0,1,0};verts[vi].u=p->x/64;verts[vi].v=p->z/64;vi++;}}}
printf("BSP: %d tri\n",nv/3);
}
int main(int c,char**v){
if(!glfwInit())return 1;
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
w=glfwCreateWindow(W,H,"Q2",0,0);if(!w)return 1;
glfwMakeContextCurrent(w);glfwSetKeyCallback(w,kc);glfwSetCursorPosCallback(w,mc);glfwSetInputMode(w,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
if(glewInit())return 1;
char*vs=ld("vert.glsl"),*fs=ld("frag.glsl");
sp=glCreateProgram();glAttachShader(sp,cs(vs,35633));glAttachShader(sp,cs(fs,35632));glLinkProgram(sp);
glEnable(GL_DEPTH_TEST);glClearColor(0.3,0.5,0.8,1);
px=0;py=64;pz=100;rx=0;ry=0;lx=W/2;ly=H/2;
if(c>1)bsp(v[1]);
if(!verts){
nv=36;verts=malloc(nv*sizeof(vx));
v3 b[]={{-20,0,-20},{20,0,-20},{20,30,-20},{-20,30,-20},{-20,0,20},{20,0,20},{20,30,20},{-20,30,20}};
int F[][3]={{0,1,2},{0,2,3},{4,6,5},{4,7,6},{0,5,1},{0,4,5},{1,5,6},{1,6,2},{2,6,7},{2,7,3},{3,7,4},{3,4,0}};
for(int i=0;i<12;i++)for(int j=0;j<3;j++){
v3*p=&b[F[i][j]];verts[i*3+j]=(vx){*p,{0,1,0},p->x/20,p->z/20};
}}
glGenTextures(1,&tex);glBindTexture(3553,tex);
u8 tx[64*64*3];for(int i=0;i<64*64;i++){int c=((i/64)&8)^((i%64)&8)?200:100;tx[i*3]=tx[i*3+1]=tx[i*3+2]=c;}
glTexImage2D(3553,0,6407,64,64,0,6407,5121,tx);
glTexParameteri(3553,10241,9729);glTexParameteri(3553,10240,9729);
glGenVertexArrays(1,&vao);glGenBuffers(1,&vbo);
glBindVertexArray(vao);glBindBuffer(34962,vbo);
glBufferData(34962,nv*sizeof(vx),verts,35044);
glVertexAttribPointer(0,3,5126,0,32,0);glEnableVertexAttribArray(0);
glVertexAttribPointer(1,3,5126,0,32,(void*)12);glEnableVertexAttribArray(1);
glVertexAttribPointer(2,2,5126,0,32,(void*)24);glEnableVertexAttribArray(2);
while(!glfwWindowShouldClose(w)){
t+=0.016;if(k[256])break;
float s=1,dx=mx-lx,dy=my-ly;lx=mx;ly=my;
rx+=dy*0.002;ry+=dx*0.002;
if(rx>1.5)rx=1.5;if(rx<-1.5)rx=-1.5;
float fx=sinf(ry)*cosf(rx),fy=sinf(rx),fz=-cosf(ry)*cosf(rx);
if(k[87]){px+=fx*s;py+=fy*s;pz+=fz*s;}
if(k[83]){px-=fx*s;py-=fy*s;pz-=fz*s;}
if(k[65]){px+=cosf(ry-1.57)*s;pz-=sinf(ry-1.57)*s;}
if(k[68]){px-=cosf(ry-1.57)*s;pz+=sinf(ry-1.57)*s;}
if(k[32])py+=s;if(k[340])py-=s;
glClear(16640);glUseProgram(sp);
float pr[16],vi[16],mv[16];
mp(pr,1.04,(float)W/H,0.1,5000);
ml(vi,px,py,pz,px+fx,py+fy,pz+fz);
mm(mv,pr,vi);
glUniformMatrix4fv(glGetUniformLocation(sp,"u"),1,0,mv);
glUniform1f(glGetUniformLocation(sp,"t"),t);
glBindTexture(3553,tex);
glDrawArrays(4,0,nv);
glfwSwapBuffers(w);glfwPollEvents();
}
glfwTerminate();return 0;
}
