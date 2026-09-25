#include <raylib.h>
#include <iostream>
using namespace std;

class PongWall
{
public:
	Vector2 position;
	PongWall(Vector2 pos)
	{
		position = pos;
	}
	
	void Draw()
	{
		DrawRectangle(position.x, position.y, 15, 70, WHITE);
	}
};

class Game
{
public:
	Vector2 wallOnePos = { 30, 200 };
	Vector2 wallTwoPos = { 855, 200 };
	PongWall wallOne = PongWall(wallOnePos);
	PongWall wallTwo = PongWall(wallTwoPos);
	float speed = 200.f;

	Game()
	{

	}

	void Update()
	{
		wallOne.position = wallOnePos;
		wallTwo.position = wallTwoPos;
		wallOne.Draw();
		wallTwo.Draw();
	}
};

int main()
{
	const int screenHeight = 480;
	const int screenWidth = 900;
	
	InitWindow(screenWidth, screenHeight, "Pong Game");
	SetTargetFPS(60);
	Game game = Game();

	while (!WindowShouldClose())
	{
		float dataFrame = GetFrameTime();

		//Paddle movement
		if (IsKeyDown(KEY_W))
		{
			if (game.wallOnePos.y > 3)
			{
				game.wallOnePos.y -= 1 * game.speed * dataFrame;
			}
		}
		if (IsKeyDown(KEY_S))
		{	
			if (game.wallOnePos.y < screenHeight -70)
			{
				game.wallOnePos.y += 1 * game.speed * dataFrame;
			}
		}

		if (IsKeyDown(KEY_UP))
		{
			if (game.wallTwoPos.y > 3)
			{
				game.wallTwoPos.y -= 1 * game.speed * dataFrame;
			}
		}
		if (IsKeyDown(KEY_DOWN))
		{
			if (game.wallTwoPos.y < screenHeight - 70)
			{
				game.wallTwoPos.y += 1 * game.speed * dataFrame;
			}
		}


		BeginDrawing();
		ClearBackground(BLACK);
		game.Update();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}