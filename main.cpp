#include "objloader.h"
#include "objloader.cpp"
#include <string>
#include <vector>
#include <cstdio>

float angle=0.0;
float ex = 0, ey = 0, ez = 150, cx = 0, cy = 0, cz = -1; 

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
void init()
{
	glClearColor(0.5,0.5,0.5,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,1.0,500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	//cube=obj.load("test.obj");	//load it
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float col[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
	loadAnimation(frames, "character_walkingupstairs/", "character1", 30, obj);
	loadAnimation(frames, "character_pickup/", "character1", 30, obj);
	//loadAnimation(frames, "character_walkingupstairs/character1", 30, obj);
}

void display()
{	
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);
	float pos[]={-1.0,1.0,-2.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	glScalef(0.225f, 0.225f, 0.225);
	//glTranslatef(0.0,-30.0,-100.0);
	//glCallList(cube);	//and display it
	glCallList(frames[currentFrame]);
	
	currentFrame++;
	if(currentFrame>59){
		currentFrame=0;
	}

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
		angle+=0.5;
		if(angle>360)
			angle-=360;
		if(1000/30>(SDL_GetTicks()-start))
			SDL_Delay(1000/30-(SDL_GetTicks()-start));
	}
	SDL_Quit();
	return 0;	
}
