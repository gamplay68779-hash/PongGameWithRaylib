#include "raylib.h"

using namespace std;

int main()
{
	const int screenHight = 480;
	const int screenWidth = 750;
	
	InitWindow(screenWidth, screenHight, "Pong Game");

	while (!WindowShouldClose())
	{
		BeginDrawing();


		EndDrawing();
	}
	CloseWindow();
	return 0;
}