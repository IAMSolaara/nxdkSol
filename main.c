//XBOX Defines
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>

//SDL Defines
#include <SDL.h>
#include <SDL_image.h>

void printSDLErrorAndReboot(void) {
	debugPrint("SDL_Error: %s\n", SDL_GetError());
	debugPrint("Rebooting in 5 seconds.\n");
	Sleep(5000);
	XReboot();
}

void printIMGErrorAndReboot(void) {
	debugPrint("SDL_Image Error: %s\n", IMG_GetError());
	debugPrint("Rebooting in 5 seconds.\n");
	Sleep(5000);
	XReboot();
}

const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;

void game(void){
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

	SDL_Window* window = NULL;
	SDL_Event event;
	SDL_Surface* screen = NULL;

	if (SDL_VideoInit(NULL) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL video.\n");
		printSDLErrorAndReboot();
	}

	window = SDL_CreateWindow("nxdkSol", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
		debugPrint("Window could not be created.\n");
		SDL_VideoQuit();
		printSDLErrorAndReboot();
	}

	if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL_image.\n");
		printSDLErrorAndQuit();
	}

	screen = SDL_GetWindowSurface(window);
	if (!screen) {
		SDL_VideoQuit();
		printSDLErrorAndQuit();
	}

	SDL_Surface* cardSurf = IMG_Load("D:\\card01.jpg");
	if (!cardSurf) {
		SDL_VideoQuit();
		printIMGErrorAndQuit();
	}

	char done = 0;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}

		SDL_UpdateWindowSurface(window);
		Sleep(1000);
	}

	SDL_VideoQuit();

}

int main() {
	XVideoSetMode(640,480,32,REFRESH_DEFAULT);
	game();
	return 0;
}
