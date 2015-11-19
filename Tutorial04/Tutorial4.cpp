#include <iostream>
#include <time.h>
#include <random>
#include "irrlicht.h"
#include "irrklang.h"

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

// event receiver to track user action from keyboard and mouse inputs
class EventReceiver : public IEventReceiver
{
public:
	EventReceiver() // tracking the keyboard inputs
	{
		for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

	bool OnEvent(const irr::SEvent & event) // tracking the keyboard inputs
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			if (event.MouseInput.isLeftPressed())
			{
				std::cout << "you pressed leftmouse."<<endl;
			}
		}
		return false;
	}

private:
	// TODO:fill in with key array
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

// Global variables 
int screenHeight = 480;
int screenWidth = 640;

// ball informtion
f32 ballSpeed = 0.3f;
irr::core::vector2df ballPos;
irr::core::vector2df ballDir;


// Global engine pointers
IrrlichtDevice* device = NULL;
irr::video::IVideoDriver* driver = NULL;
irr::video::ITexture* ball = NULL;
irr::gui::IGUIEnvironment* env = NULL;
irrklang::ISoundEngine* soundEngine = NULL;

void updateInformation(f32 dt)
{
	//position = x + k*dt
	ballPos += ballDir*(ballSpeed*dt);
	if (ballPos.X >= screenWidth || ballPos.X <= 0)
	{
		ballDir.X *= -1;
	}

	if (ballPos.Y >= screenHeight || ballPos.Y <= 0)
	{
		ballDir.Y *= -1;
	}
}

int main()
{
	/*Here is a template for including all the nessesery paramiters into the code
	By doing so you will insure that everything is created and ready for work
	Reference names:
	device - your created device an instance of the screen and make sure everything is running
	driver - choosed driver opengl
	skin - working with visuals may be?!
	font - letters for varios notes in game
	env - object on your screen
	*/

	irr::IrrlichtDevice* device = createDevice(irr::video::EDT_OPENGL,
		irr::core::dimension2d<u32>(screenWidth, screenHeight),
		32,
		0,
		1,
		0,
		0
		);



	//        irr::IEventReceiver* er = ;
	//Initialize the event reciver and set it to the pongevent reciver function
	EventReceiver pongEventReceiver;
	device->setEventReceiver(&pongEventReceiver);

	irr::video::IVideoDriver* driver = device->getVideoDriver();

	// load a requared texture
	ball = driver->getTexture("fireball.bmp");

	/*do not worry about the code in this module
	it is required for ball to move in every derection everytime you start the program
	TO BE EXPLAINED*/

	env = device->getGUIEnvironment();
	//irr::gui::IGUISkin* skin = env->getSkin();
	//irr::gui::IGUIFont* font = env->getFont("bigfont.png"); //loading the font
	//if (font)
	//        skin->setFont(font);
	//skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);


	// Enable sound engine
	irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT);
	// set a music to play
	soundEngine->play2D("bell.wav", 1);

	// We are going to set the window to not be resizeable for now
	device->setResizable(false);

	// set our lovely window caption
	device->setWindowCaption(L"Hello world. IEEEGDC stuff.");



	f32 FPS = 0;
	f32 currentTime = 0;
	f32 endTime = 0;
	f32 deltaTime = 0;

	currentTime = device->getTimer()->getTime();

	/*do not worry about the code in this module
	it is required for ball to move in every derection everytime you start the program
	TO BE EXPLAINED
	*/
	//srand(time(NULL));
	//int v1 = rand() % 3 + 3;
	//int v2 = rand() % 3 + 3;

	//double vd1 = v1 / 5.0;
	//double vd2 = v2 / 5.0;

	//int sign1 = rand() % 2;
	//int sign2 = rand() % 2;

	//if (sign1 == 1)
	//{
	//        vd1 = vd1*(-1);
	//}
	//if (sign2 == 1)
	//{
	//        vd2 = vd2*(-1);
	//}

	ballDir.set(1, 1);
	ballSpeed = 0.3f;
	ballDir.normalize(); // that is a special function for some godknown reason just leave it here

	ballPos.set(screenWidth / 2.0f, screenHeight / 2.0f);
	//cout << "Ball X: " << ballDir.X << "Ball Y: " << ballDir.Y << endl;

	int FPSStringSize = 0;
	core::stringc FPSString = "";


	while (device->run() && driver) //GAME LOOP
	{
		driver->beginScene(1, 1, video::SColor(0, 150, 150, 150));
		env->drawAll();

		//  to get a delta time
		endTime = device->getTimer()->getTime();
		deltaTime = (endTime - currentTime);        //deltaTime = (1000.0f / ((endTime - currentTime)));
		FPS = 1000.0f / (f32)(deltaTime);
		currentTime = endTime;


		updateInformation(deltaTime); // updating ball position

		// Reset screen width/height to whatever height is created during resizing. It will be reuared later!
		//screenWidth = driver->getScreenSize().Width;
		//screenHeight = driver->getScreenSize().Height;

		driver->beginScene(1, 1, video::SColor(255, 150, 150, 150));
		env->drawAll();

		driver->draw2DImage(ball,
		core::position2d<s32>((s32)ballPos.X, (s32)ballPos.Y),
			 core::rect<s32>(0, 0, ball->getSize().Width, ball->getSize().Height),
			 0,
			 video::SColor(255, 255, 255, 255),
			 1);

		//cout << ((core::stringc)(driver->getFPS())).c_str() << "\b\b\b\b\b";

		driver->endScene();
	}
	device->closeDevice();
	device->drop();

	return 0;
}