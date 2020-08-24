//SPDX-License-Identifier: BSD-3-Clause
//SPDX-FileCopyrightText: 2020 Lorenzo Cauli (lorecast162)

#ifdef NXDK
//XBOX Defines
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#endif
//SDL Defines
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gamecontroller.h>

//project includes
#include "linkedlist.h"

static void printSDLErrorAndReboot(void);

static void printIMGErrorAndReboot(void);

#ifdef NXDK
const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;
#else
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#endif

void game(void){
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

	SDL_Window* window = NULL;
	SDL_Event event;
	SDL_Surface* screen = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL.\n");
		printSDLErrorAndReboot();
	}

	window = SDL_CreateWindow("nxdkSol", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
#ifdef NXDK
		debugPrint("Window could not be created.\n");
#else
		printf("Window could not be created.");
#endif
		SDL_VideoQuit();
		printSDLErrorAndReboot();
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL_image.\n");
		printSDLErrorAndReboot();
	}

	screen = SDL_GetWindowSurface(window);
	if (!screen) {
		SDL_VideoQuit();
		printSDLErrorAndReboot();
	}

	SDL_Texture* cards[4][15] = {
		{
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\ace.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\2.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\3.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\4.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\5.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\6.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\7.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\8.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\9.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\10.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\jack.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\queen.png"),
			IMG_LoadTexture(renderer, "D:\\res\\clubs\\king.png")
		},
		{
			IMG_LoadTexture(renderer, "D:\\res\\spades\\ace.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\2.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\3.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\4.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\5.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\6.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\7.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\8.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\9.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\10.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\jack.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\queen.png"),
			IMG_LoadTexture(renderer, "D:\\res\\spades\\king.png")
		},
		{
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\ace.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\2.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\3.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\4.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\5.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\6.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\7.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\8.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\9.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\10.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\jack.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\queen.png"),
			IMG_LoadTexture(renderer, "D:\\res\\diamonds\\king.png")
		},
		{
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\ace.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\2.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\3.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\4.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\5.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\6.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\7.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\8.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\9.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\10.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\jack.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\queen.png"),
			IMG_LoadTexture(renderer, "D:\\res\\hearts\\king.png")
		}
	};
	SDL_Texture* stackTexture = IMG_LoadTexture(renderer, "D:\\res\\back.png");

	SDL_Texture* cursorTexture = IMG_LoadTexture(renderer, "D:\\res\\cursor.png");

	SDL_Rect stackRect = {16, 16, 64, 96};
	SDL_Rect stackOutputRect = {96, 16, 64, 96};

	SDL_Rect winPile1 = {566, 16, 64, 96};
	SDL_Rect winPile2 = {492, 16, 64, 96};
	SDL_Rect winPile3 = {418, 16, 64, 96};
	SDL_Rect winPile4 = {344, 16, 64, 96};

	SDL_Rect field1 = {88, 128, 64, 96};
	SDL_Rect field2 = {168, 128, 64, 96};
	SDL_Rect field3 = {248, 128, 64, 96};
	SDL_Rect field4 = {328, 128, 64, 96};
	SDL_Rect field5 = {408, 128, 64, 96};
	SDL_Rect field6 = {488, 128, 64, 96};

	node* stack = addnode(NULL, 1, 12);
	
	SDL_Rect choiceRects[2][6] = 
	{
		{
			stackRect,
			stackOutputRect,
			winPile4,
			winPile3,
			winPile2,
			winPile1
		},
		{
			field1,
			field2,
			field3,
			field4,
			field5,
			field6
		}
	};

	int choice[] = {0,0};

	SDL_Texture* bgTexture = IMG_LoadTexture(renderer, 
#ifdef NXDK
			"D:\\res\\bg.png"
#else
			"./res/bg.png"
#endif
			);
	SDL_Rect scrRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

#ifdef NXDK
	debugPrint("%i joysticks were found.\n\n", SDL_NumJoysticks() );
#else
	printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
#endif

	SDL_Texture* stackOutTexture = NULL; 

	SDL_GameController* joystick;
	SDL_GameControllerEventState(SDL_ENABLE);
	joystick = SDL_GameControllerOpen(0);

	//int choice = 0;

	char done = 0;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					switch (event.cbutton.button) {
						case SDL_CONTROLLER_BUTTON_A:
							switch (choice[1]) {
								case 0:
									if (choice[0] == 0) stackOutTexture = cards[2][5];
							}
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							if (choice[0] == 0) choice[0] = 1;
							else choice[0]--;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							if (choice[0] == 1) choice[0] = 0;
							else choice[0]++;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							if (choice[1] == 0) choice[1] = 5;
							else choice[1]--;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							if (choice[1] == 5) choice[1] = 0;
							else choice[1]++;
							break;
					}
					break;
				default:
					break;
			}
		}
		SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

		SDL_RenderCopy(renderer, stackTexture,  NULL, &stackRect);
		if (stackOutTexture) SDL_RenderCopy(renderer, stackOutTexture,  NULL, &stackOutputRect);
		SDL_RenderCopy(renderer, cards[0][0],  NULL, &winPile1);
		SDL_RenderCopy(renderer, cards[1][0],  NULL, &winPile2);
		SDL_RenderCopy(renderer, cards[2][0],  NULL, &winPile3);
		SDL_RenderCopy(renderer, cards[3][0],  NULL, &winPile4);
		SDL_RenderCopy(renderer, cursorTexture, NULL, &choiceRects[choice[0]][choice[1]]);
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);

	}

	SDL_VideoQuit();
	SDL_GameControllerClose(joystick);
}

int main() {
#ifdef NXDK
	XVideoSetMode(640,480,32,REFRESH_DEFAULT);
#endif
	game();
	return 0;
}

#ifdef NXDK
static void printSDLErrorAndReboot(void) {
	debugPrint("SDL_Error: %s\n", SDL_GetError());
	debugPrint("Rebooting in 5 seconds.\n");
	Sleep(5000);
	XReboot();
}

static void printIMGErrorAndReboot(void) {
	debugPrint("SDL_Image Error: %s\n", IMG_GetError());
	debugPrint("Rebooting in 5 seconds.\n");
	Sleep(5000);
	XReboot();
}

#else
static void printSDLErrorAndReboot(void) {
	printf("SDL_Error: %s\n", SDL_GetError());
	printf("Rebooting in 5 seconds.\n");
	IMG_Quit();
	SDL_Quit();
}

static void printIMGErrorAndReboot(void) {
	printf("SDL_Image Error: %s\n", IMG_GetError());
	printf("Rebooting in 5 seconds.\n");
	IMG_Quit();
	SDL_Quit();
}


#endif
