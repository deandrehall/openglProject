#include "objloader.h"
#include "objloader.cpp"
#include <string>
#include <vector>
#include <cstdio>

float angle=0.0;
float ex = 0, ey = 50, ez = 150, cx = 0, cy = 0, cz = -1; 

int cube;
objloader obj;	//create an instance of the objloader
std::vector<unsigned int> frames;
int currentFrame=0;

void loadAnimation(std::vector<unsigned int>& frames,std::string path, std::string filename, unsigned int num,objloader& obj)
{
    char tmp[200];
    for(int i=1;i<=num;i++)
    {
        sprintf(tmp, ".%04d.obj",i);
        std::string tmp2(path+filename+tmp);
        //tmp2+=".obj";
        std::cout << tmp2 << std::endl;
        unsigned int id=obj.load(tmp2.c_str(), path);
        frames.push_back(id);
    }
}

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

void init()
{
	glClearColor(0.5,0.5,0.5,1.0);
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
	glLoadIdentity();
	gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);
	float pos[]={-1.0,1.0,-2.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	
	glScalef(5.0f, 5.0f, 5.0f); // shrinks the boy; hes too big
	glRotatef(angle, 0.0f, 1.0f, 0.0f); //spin the models
	
	glEnable(GL_TEXTURE_2D);
   GLuint texture;
   texture = loadTextures("Textures/marble.bmp");
   glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(0.0f,-20.0f,-0.0f);
   glBegin(GL_QUADS);
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
   
   //glLoadIdentity();

   glTranslatef(-1.5f,-1.5f,-1.0f);                      // Move Into The Screen 5 Units
   texture = loadTextures("Textures/pattern.bmp");
   glBindTexture(GL_TEXTURE_2D, texture);

   glBegin(GL_QUADS);
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

	//glLoadIdentity();
   glFlush();
   
   glScalef(0.0f, 0.0f, 0.0f);
   glScalef(0.125f, 0.125f, 0.125); // shrinks the boy; hes too big
	
	if(currentFrame<30){
		glCallList(frames[currentFrame]);
		
	}
	else if(currentFrame<60){
		glTranslatef(0.0, 50.0, 38.0);
		glCallList(frames[currentFrame]);
	}
	
	currentFrame++;
	
	if(currentFrame > 60) currentFrame = 0;
	
	angle+=1;
	if(angle>360)
		angle-=360;
}


int main(int argc,char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen=SDL_SetVideoMode(1024,800,32,SDL_SWSURFACE|SDL_OPENGL);
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
		display();
		
		
		SDL_GL_SwapBuffers();
		
		if(1000/30>(SDL_GetTicks()-start))
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
	}
	SDL_Quit();
	return 0;	
}
