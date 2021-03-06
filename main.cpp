#include "objloader.h"
#include "objloader.cpp"
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

float angle=0.0;

const int WIDTH = 1024, HEIGHT = 800;
float ex = 0, ey = 0, ez = 100, cx = 0, cy = 0, cz = -1;

GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
float pos[]={-1.0,1.0,-2.0,1.0};

int cube;
objloader obj;	//create an instance of the objloader
std::vector<unsigned int> frames; //used to store the animations
int currentFrame=0;

void loadAnimation(std::vector<unsigned int>& frames,std::string path, std::string filename, unsigned int num,objloader& obj)
{
    char tmp[200];
    for(int i=1;i<=num;i++)
    {
        sprintf(tmp, ".%04d.obj",i);
        std::string tmp2(path+filename+tmp);
        std::cout << tmp2 << std::endl;
        unsigned int id=obj.load(tmp2.c_str(), path);
        frames.push_back(id);
    }
}

void drawSphere(double r, int lats, int longs) {
       int i, j;
       for(i = 0; i <= lats; i++) {
          double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
          double z0  = sin(lat0);
          double zr0 =  cos(lat0);
    
          double lat1 = M_PI * (-0.5 + (double) i / lats);
          double z1 = sin(lat1);
          double zr1 = cos(lat1);
    
          glBegin(GL_QUAD_STRIP);
          
          glColorMaterial(GL_FRONT, GL_DIFFUSE);
		  glEnable(GL_COLOR_MATERIAL);
          
           for(j = 0; j <= longs; j++) {
		       double lng = 2 * M_PI * (double) (j - 1) / longs;
		       double x = cos(lng);
		       double y = sin(lng);
			   glColor3f(0.8f, 0.8f, 0.0f);
		       glNormal3f(x * zr0, y * zr0, z0);
		       glVertex3f(x * zr0, y * zr0, z0);
		       glNormal3f(x * zr1, y * zr1, z1);
		       glVertex3f(x * zr1, y * zr1, z1);
		       glFlush();
           }
           glEnd();
       }
       
 }

// http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture
GLuint loadTextures(const char *filename) 
{
   GLuint texture;
   int height = 256;
   int width = 256;
   unsigned char *data;
   FILE *file;

   file = fopen(filename, "rb");

   if(file == NULL)
   {
      printf("Error opening a file.\n");
      return 0;
   }

   data = (unsigned char *)malloc( height * width * 3 );
   fread(data, height * width * 3, 1, file);
   fclose(file);

   for(int i = 0; i < width * height; i++)
   {
      int index = i*3;
      unsigned char B,R;
      B = data[index];
      R = data[index+2];

      data[index] = R;
      data[index+2] = B;

   }

   glGenTextures(1, &texture);
   glBindTexture( GL_TEXTURE_2D, texture);

   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
   gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
   free( data );

   return texture;
}

void drawCube(const char* textureName){

	glEnable(GL_TEXTURE_2D);
	GLuint texture;
	texture = loadTextures(textureName); //texture being applied to the cube
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
		  glColor3f(1.0f, 1.0f, 1.0f);
		  // Front Face
		  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		  // Back Face
		  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		  // Top Face
		  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		  // Bottom Face
		  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		  // Right face
		  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		  // Left Face
		  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
   	glEnd();
}

void init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,1.0,500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float col[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
	loadAnimation(frames, "character_walkingupstairs/", "character1", 30, obj);
	loadAnimation(frames, "character_pickup/", "character1", 30, obj);
}

void display()
{	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable (GL_COLOR_MATERIAL);
	
	glLoadIdentity();
	gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);
	

	//glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT1,GL_POSITION,pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glEnable(GL_LIGHT1);
	
	glScalef(2.0f, 2.0f, 2.0f); // makes everything bigger
	glRotatef(angle, 0.0f, 1.0f, 0.0f); //spins 
	
	glTranslatef(0.0f,-1.0f,0.0f); //translate down
	drawCube("Textures/zangief.bmp"); //draws cube with zangief texture applied
 	glTranslatef(0.0f,1.0f,0.0f); //reset translate

	//glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glEnable (GL_COLOR_MATERIAL);
	
    glTranslatef(0.0,1.5f,2.0f); // move up and foward
    drawCube("Textures/baby.bmp"); //draws cube with baby texture applied
    glTranslatef(0.0f,-1.5f,-2.0f); //resets translate
   	
   	glTranslatef(1.5f, 14.5f, 5.5f); //translate for the ball
    drawSphere(1.0, 10, 10); //draws ball
    glFlush();
    glTranslatef(-1.5f, -14.5f,-5.5f); //restets translate

    glScalef(0.07f, 0.07f, 0.07f); // shrinks the boy; hes too big
	if(currentFrame<30){
		glCallList(frames[currentFrame]); //first 30 frames, walking animation
		
	}
	else if(currentFrame<60){
		glTranslatef(0.0, 40.0, 37.5); // move the boy up and foward, on top of the 2nd box
		glCallList(frames[currentFrame]); //load pick up animation
	}
	currentFrame++; // increment frames
	if(currentFrame > 60) currentFrame = 0; // resets the animation when it reaches frame 60
	
	angle+=1;
	if(angle>360)
		angle-=360;
}


int main(int argc,char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen=SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_SWSURFACE|SDL_OPENGL);
	bool running=true;
	Uint32 start;
	SDL_Event event;
	init();
	while(running)
	{
		start=SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running=false;
					break;
			}
		}
		display(); //loads the scene
		
		SDL_GL_SwapBuffers();
		
		if(1000/30>(SDL_GetTicks()-start))
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
	}
	SDL_Quit();
	return 0;	
}
