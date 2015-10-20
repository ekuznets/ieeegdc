#include <stdlib.h>
#include <irrlicht.h>
#include <irrKlang.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
// comments will be addede soon

int main()
{
	//define device which will create a window params: rendering device type, screen size,colors,fullscreen,shodows,synchronization of monitor,input handle
	irr:IrrlichtDevice *device = createDevice(irr::video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);
	IVideoDriver *driver = device->getVideoDriver();
	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 255,0,0));
		driver->endScene();
	}

	device->drop();

	return 0;
}