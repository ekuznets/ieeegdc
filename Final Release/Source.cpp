#include <iostream>
#include <time.h>
#include <random>
#include "irrlicht.h"
#include "irrklang.h"

// declare all this namespace, tho I will still use them in code as full name like: irr::video:: { some command }
using namespace std;
using namespace irr;
using namespace video;
using namespace gui;
using namespace scene;
using namespace core;
using namespace io;
using namespace irrklang;

// to say linker which command to use
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

// Global variables 

int screenWidth = 800;
int screenHeight = 600;
int player1Score = 0;
int player2Score = 0;

// ball informtion
f32 ballSpeed = 0.3f;
irr::core::vector2df ballPos;
irr::core::vector2df ballDir;
irr::core::vector2df target;

// paddle informtion
f32 paddle1Speed = 0.6f;
f32 paddle2Speed = 0.6f;
irr::core::vector2df paddle1Pos;
irr::core::vector2df paddle2Pos;
bool paddle1Up = false;
bool paddle1Down = false;
bool paddle2Up = false;
bool paddle2Down = false;


// Global engine pointers
IrrlichtDevice* device = NULL;
irr::video::IVideoDriver* driver = NULL;
irr::video::ITexture* ball = NULL;
irr::video::ITexture* paddle1 = NULL;
irr::video::ITexture* paddle2 = NULL;
irr::gui::IGUIEnvironment* env = NULL;
irrklang::ISoundEngine* soundEngine = NULL;

//function declarions
void updateBallPosition(f32 dt);
void updatePaddlePosition(f32 dt);


void updateBallPosition(f32 dt) 
{
	//position = x + k*dt  //dt = delta time
	ballPos += ballDir*(ballSpeed*dt); 
	//cout << dt << endl;
	if ((ballPos.X < paddle1Pos.X + paddle1->getSize().Width) // Check if the ball has moved past the right edge of the paddle
		&& (ballPos.X > paddle1Pos.X)	// Check if the ball has not moved past the left edge of the paddle

		&&

		((ballPos.Y + ball->getSize().Height) > paddle1Pos.Y) // Check if the ball has moved past the top edge of the paddle
		&& (ballPos.Y < paddle1Pos.Y + paddle1->getSize().Height)) // Check if the ball has not moved past the bottom edge of the paddle
	{
		//reverse serection and slitly increase the ball speed
		ballDir.X *= -1;
		ballSpeed *= 1.001f;  //small constant for increase ball speed
		// set a music to play when ball hit the boundary screen
		soundEngine->play2D("impact.wav");
	}
	// Collision detection for paddle 2
	if ((ballPos.X + ball->getSize().Width > paddle2Pos.X) // Check if the ball has moved past the right edge of the paddle
		&& (ballPos.X > paddle2Pos.X - paddle2->getSize().Width)	// Check if the ball has not moved past the left edge of the paddle

		&&

		((ballPos.Y + ball->getSize().Height) > paddle2Pos.Y) // Check if the ball has moved past the top edge of the paddle
		&& (ballPos.Y < paddle2Pos.Y + paddle2->getSize().Height)) // Check if the ball has not moved past the bottom edge of the paddle
	{
		//reverse serection and slitly increase the ball speed
		ballDir.X *= -1;
		ballSpeed *= 1.001f; // small constant for increase ball speed
		// set a music to play when ball hit the boundary screen
		soundEngine->play2D("impact.wav");
	}

	// old code for ball collision  can be removed 
	//if (ballPos.X >= screenWidth - ball->getSize().Width || ballPos.X <= 0)
	//{
	//	ballDir.X *= -1;
	//}

	// reverse serection of the ball when it`s get collided with top/bottom walls
	if (ballPos.Y >= screenHeight - ball->getSize().Height || ballPos.Y <= 0)
	{
		ballDir.Y *= -1;
		// set a music to play when ball hit the boundary screen
		soundEngine->play2D("ball.wav");
	}

	// modified ball score if puddle cross the screen
	if ((ballPos.X + ball->getSize().Width > screenWidth && ballDir.X > 0)
			|| (ballPos.X < 0) && ballDir.X < 0)
	{
		// Update player scores
		if (ballPos.X >(screenWidth / 2))
		{
			// set a music to gong when ball is scored
			player1Score++;
			soundEngine->play2D("gong.mp3");
		}
		else
		{
			// set a music to gong when ball is scored
			player2Score++;
			soundEngine->play2D("gong.mp3");
		}

		// Reset the ball position, direction, and starting speed.
		ballPos.X = (screenWidth / 2) - (ball->getSize().Width / 2);
		ballPos.Y = (screenHeight / 2) - (ball->getSize().Height / 2);
		srand(time(NULL));
		int v1 = rand() % 3 + 3;
		int v2 = rand() % 3 + 3;

		double vd1 = v1 / 5.0;
		double vd2 = v2 / 5.0;

		int sign1 = rand() % 2;
		int sign2 = rand() % 2;

		if (sign1 == 1)
		{
			vd1 = vd1*(-1);
		}
		if (sign2 == 1)
		{
			vd2 = vd2*(-1);
		}

		ballDir.set(vd1, vd2);
		ballSpeed = 0.3f;
		ballDir.normalize();
	}

}

void updatePaddlePosition(f32 dt)
{
	//TODO: puddle colision with walls


	// when we recieve a call when key is pressed move puddle on current possition + deltatime*speed
	if (paddle1Up == true && paddle1Pos.Y >= 0)
	{ 
		paddle1Pos.Y -= dt*paddle1Speed; 
	}
	
	if (paddle1Down == true && paddle1Pos.Y + paddle1->getSize().Height < screenHeight )
	{ 
		paddle1Pos.Y += dt*paddle1Speed; 
	}
	 
	if (paddle2Up == true && paddle2Pos.Y >= 0)
	{ 
		paddle2Pos.Y -= dt*paddle2Speed; 
	}

	if (paddle2Down == true && paddle2Pos.Y + paddle2->getSize().Height < screenHeight)
	{ 
		paddle2Pos.Y += dt*paddle2Speed; 
	}

	//if (ballPos.X > )

}


// class EventReceiver which would handle our interapts events
class EventReceiver : public IEventReceiver
{
public:
	EventReceiver()
	{
		// keep track of array of keys command such as keyboard/mouse to keep track which are pressed
		for (u32 i = 0; i<irr::KEY_KEY_CODES_COUNT; i++)
			KeyIsDown[i] = false;
	}

	virtual bool OnEvent(const irr::SEvent & event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			if (event.MouseInput.isLeftPressed())
			{
				//ball will be controled by mouse movement just for debugging/ you can comment out code for the actual game
				target.X = event.MouseInput.X;
				target.Y = event.MouseInput.Y;
				ballDir.X = target.X - ballPos.X; // x-component vector from mouse position to ball position.
				ballDir.Y = target.Y - ballPos.Y; // y-component vector from mouse position to ball position.
				ballDir.normalize();
			}
		}
		// Control whether paddles are moving based on keyboard inputs.
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			// Record keys that are down into the key array.
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

			// Paddle1 control values.
			if (KeyIsDown[irr::KEY_KEY_W]) 
			{ paddle1Up = true; } // set true when key is pressed on keyboard
			else 
			{ paddle1Up = false; } // otherwise set to false
			if (KeyIsDown[irr::KEY_KEY_S]) 
			{ paddle1Down = true; }
			else 
			{ paddle1Down = false; }

			// Paddle2 control values.
			if (KeyIsDown[irr::KEY_KEY_O])
			{ paddle2Up = true; }
			else { paddle2Up = false; }
			if (KeyIsDown[irr::KEY_KEY_L]) 
			{ paddle2Down = true; }
			else { paddle2Down = false; }
		}

		return false;
	}
private:
	// TODO:fill in with key array
	bool KeyIsDown[KEY_KEY_CODES_COUNT]; // array of keys true/false = pressed/ not pressed
};

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

	// define a device with paramiters
	irr::IrrlichtDevice* device = createDevice(
		irr::video::EDT_OPENGL, // video driver openGL
		irr::core::dimension2d<u32>(screenWidth, screenHeight), // size of the screen
		32, // colors
		0,
		1, // vertical sync 
		0,
		0
		);

	// create an event reciver class to take user input from keyboard/mouse/joystick
	EventReceiver pongEventReceiver;
	device->setEventReceiver(&pongEventReceiver);

	// create a video driver for your game to render a screen
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	// load a requared textures
	ball = driver->getTexture("ball.png");
	paddle1 = driver->getTexture("paddle1.png");
	paddle2 = driver->getTexture("paddle2.png");

	/*do not worry about the code in this module
	it is required for ball to move in every derection everytime you start the program
	TO BE EXPLAINED*/

	env = device->getGUIEnvironment();
	irr::gui::IGUISkin* skin = env->getSkin();
	irr::gui::IGUIFont* font = env->getFont("bigfont.png"); //loading the font from a file to display your characters on a screen
	if (font)
		skin->setFont(font);
	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP); // apply up this font to be used instead of default


	// Enable sound engine
    soundEngine = irrklang::createIrrKlangDevice(ESOD_AUTO_DETECT);

	// We are going to set the window to not be resizeable for now
	device->setResizable(false);

	// set our lovely window caption
	device->setWindowCaption(L"Hello world. IEEEGDC stuff.");

	f32 FPS = 0; // FPS = frames per second
	f32 currentTime = 0; // time at the current frame
	f32 endTime = 0; // time mesured at next frame
	f32 deltaTime = 0; // difference of endTime - endTime  will give us time that get passed

	currentTime = device->getTimer()->getTime();

	/*do not worry about the code in this module
	it is required for ball to move in every derection everytime you start the program
	TO BE EXPLAINED
	*/
	srand(time(NULL));
	int v1 = rand() % 3 + 3;
	int v2 = rand() % 3 + 3;

	double vd1 = v1 / 5.0;
	double vd2 = v2 / 5.0;

	int sign1 = rand() % 2;
	int sign2 = rand() % 2;

	if (sign1 == 1)
	{
		vd1 = vd1*(-1);
	}
	if (sign2 == 1)
	{
		vd2 = vd2*(-1);
	}

	ballDir.set(vd1, vd2);
	ballSpeed = 0.3f;
	ballDir.normalize(); // that is a special function for some godknown reason just leave it here

	ballPos.set(screenWidth / 2.0f, screenHeight / 2.0f);

	int FPSStringSize = 0;
	core::stringc FPSString = "";

	// Set initial paddle positions to the opposite sides of the screen.
	paddle1Pos.X = 10;
	paddle1Pos.Y = screenHeight*.5 - paddle1->getSize().Height*.5;
	paddle2Pos.X = screenWidth - 10 - paddle2->getSize().Width;
	paddle2Pos.Y = screenHeight*.5 - paddle2->getSize().Height*.5;

	int i = 0;
	while (device->run() && driver) //GAME LOOP
	{
		i++;
		i = i % 10000;
		driver->beginScene(1, 1, video::SColor(255, 150, 150, 150)); // start scene by drawing a backgroud basic color
		env->drawAll(); // set it to be drawn before all other textures

		// compute a delta time
		endTime = device->getTimer()->getTime();
		deltaTime = (endTime - currentTime);	
		FPS = 1000.0f / (f32)(deltaTime);
		currentTime = endTime;

		updateBallPosition(deltaTime); // updating ball position
		updatePaddlePosition(deltaTime); // updating paddle position

		// Reset screen width/height to whatever height is created during resizing. It will be reuared later!
		//screenWidth = driver->getScreenSize().Width;
		//screenHeight = driver->getScreenSize().Height;

		// draw a line which will separate a boundary for two players fields
		driver->draw2DLine(irr::core::vector2d<s32>(screenWidth / 2, 0), irr::core::vector2d<s32>(screenWidth / 2, screenHeight), irr::video::SColor(255, 0, 255, 0));

		//draw a score points for player1 - left side middle
		font->draw((core::stringc)player1Score,
			core::rect<s32>(screenWidth / 4, screenHeight / 2, screenHeight, screenWidth),
			video::SColor(255, 255, 255, 255));

		//draw a score points for player2 - right side middle
		font->draw((core::stringc)player2Score,
			core::rect<s32>(screenWidth* 3 / 4, screenHeight / 2, screenHeight, screenWidth),
			video::SColor(255, 255, 255, 255));
		//draw a ball middle of the screen
		driver->draw2DImage(ball,
			core::position2d<s32>((s32)ballPos.X, (s32)ballPos.Y),
			core::rect<s32>(0, 0, ball->getSize().Width, ball->getSize().Height),
			0,
			video::SColor(255, i%255, 255, 255),
			1);
		//draw puddle for player1 - left side 
		driver->draw2DImage(paddle1,
			core::position2d<s32>(paddle1Pos.X, paddle1Pos.Y),
			core::rect<s32>(0, 0, paddle1->getSize().Width, paddle1->getSize().Height),
			0,
			video::SColor(255, 255, 255, 255),
			1);
		//draw a puddle for player2 - right side 
		driver->draw2DImage(paddle2,
			core::position2d<s32>(paddle2Pos.X, paddle2Pos.Y),
			core::rect<s32>(0, 0, paddle2->getSize().Width, paddle2->getSize().Height),
			0,
			video::SColor(255, 255, 255, 255),
			1);
		// call driver->endScene(); when all the rendering is done and ready for next loop
		driver->endScene();
	}

	//close your device
	device->closeDevice();
	//free memmory of your device
	// warning if that command is not called you will get a memory leak
	device->drop();

	return 0;
}