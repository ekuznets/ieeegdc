# ieeegdc
Official IEEE Game Development Repository at Simon Fraser University.

To play:
Go to Pong executable folder and run PongGame.exe to start the game.

Controls:

Player 1
 
W - move up
S - move down

Player 2
 
O - move up
L - move down

ESC - close

Left mouse click - to cotrol the ball
(Just for debugging purpose, dont cheat :) )


To compile:
The latest code of our game is located at Final Release v 1.1 with all assets file.

To learn more about project:
Follow tutorials 1-6 to get basic of game development using IrrLitch and C++


If you want to contribute:

For Windows users:

Follow setupdevenv.docx - It will guide you how to install your development environment on your Windows PC.

For MacOS users:

I had spent some time on working with mac and find a solution how to compile Irrlicht.
Works on OS X 10.9 with Xcode 6.3, I think will work on latest as well
First setup a Xcode on you machine and download Irrlicht 1.8.
Note it will NOT WORK for 1.8.3 !!! However, 1.8.1 should be fine. Personally, I am still working with that version and have no difficulties what so ever.
Link for 1.8.1 is in here: http://sourceforge.net/…/ir…/files/Irrlicht%20SDK/1.8/1.8.1/

Follow this great tutorial how to make irrlicht work: https://vimeo.com/58331307
Next step is to download latest tutorial from our GIT and required asset files(.png). To test that everything is working: go to HelloWorld project and replace all the code from that project with one that provided in tutorials. Copy all of the assets files to the folder where is your main.cpp file.
IMPORTANT STEP: comment out all the references for irrKlang in that project. That is not yet added. I will work on sound library by next week.
Compile all the code, and ignore all of the warnings that Xcode display.
