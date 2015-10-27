#include <iostream>
#include "irrlicht.h"
#include "irrklang.h"
// authors: Ramin and Evgeny 
using namespace std;
using namespace irr;
using namespace video;
using namespace gui;
using namespace scene;
using namespace core;
using namespace io;
using namespace irrklang;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

// global variables starts in here

// DOWNLOAD ALL THE ASSETS FILE FOR THIS TUTORIAL FROM https://github.com/ekuznets/ieeegdc
// PUT ALL THE FILES WITH YOUR SOURCE CODE FILE IN THE PROJECT .cpp

// ends here
int main()
{
	// definition of the required parameters starts in here
	std::cout << "hello world!\n";
	
	//create an Irrlitch engine called irr::IrrlichtDevice* it is a pointer to a data type
	irr::IrrlichtDevice* device = createDevice(irr::video::EDT_OPENGL,
		irr::core::dimension2d<u32>(640, 480),
		32,
		0,	 
		1,
		0,
		0
		);

	/* 
	Using this command will result in setting custom data name of the game, play with it if you like
	Note: L"ANY STRING" - L is special type of string just put it in there for complier to be happy
	*/
	device->setWindowCaption(L"Hello world. IEEEGDC stuff.");
	
	// Get a pointer the video driver called irr::video::IVideoDriver*
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	//create an Irrklang engine called irrklang::ISoundEngine*
	irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT);
	/*
	Provide a parameter which audio file you would like to open. 
	Note: is should be located within you source code file in order to be found by compiler
	Otherwise you will be required to specify a relative path, I will teach you how to do that
	*/
	soundEngine->play2D("IrrlichtTheme.ogg", 1);
	/*
	Provide a parameter which image file you would like to open.
	//create an texture parameter called irr::video::ITexture*
	*/
	irr::video::ITexture* ball = driver->getTexture("irrlichtlogo.BMP");

	// define position on a screen, remember we have 640x480 in total
	float x=320.0f; float y=240.0f;
	float k = 0.001;

	/*
	that following code required to create FPS of the program frames (frames per seconds) : optional step
	*/
	irr::f32 startTime = device->getTimer()->getTime();
	irr::f32 endTime = -1;
	irr::f32 deltaTime = -1;

	// Game Loop! where all the action will happen
	while (device->run())
	{
		/*
		These part of code is responsible for the synchronization of moving objects
		taking the current time from a driver and subtract the previous time will give a time that have been passed from last frame rendered
		it can be used to update positions of the all objects
		*/
		endTime = device->getTimer()->getTime();
		deltaTime = f32(endTime - startTime);
		startTime = endTime;
	    k = k*deltaTime;

		// output fps to console, will be very fast for now!
		std::cout << 1000.0f / deltaTime <<endl;

		// draw the background of the screen, you can play with parameters
		driver->beginScene(1,1,irr::video::SColor(255,0,0,255));

        //Draw a texture.

		// update position  = x + k*dt
		x = x + k;
		y = y + k;
		// draw the image on screen
		driver->draw2DImage(ball,irr::core::vector2d<s32>(x,y));

		// end frame when all the updates are finished
		driver->endScene();
	}

	// Release all resources.
	device->closeDevice();
	device->drop();
	//device->grab();
	return 0;
}