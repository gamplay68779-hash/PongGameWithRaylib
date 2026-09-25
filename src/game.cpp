#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <cmath>

using namespace std;

class Ball
{
public:
    Vector2 position;
    float ballSize = 7;
    Vector2 direction = { 1, 0 };
    float speed = 250.f;

    Ball(Vector2 pos)
    {
        position = pos;
    }

    void Draw()
    {
        DrawCircle(position.x, position.y, ballSize, WHITE);
    }

    void reset(float dirX)
    {
        position = { 900 / 2.0f , 480 / 2.0f };
        speed = 250.f;
        direction = { dirX, 0.0f };
    }
};

class PongWall
{
public:
    int Height = 70;
    int width = 15;
    Vector2 position;
    int score = 0;

    PongWall(Vector2 pos)
    {
        position = pos;
    }

    void Draw()
    {
        DrawRectangle(position.x, position.y, width, Height, WHITE);
    }
};

void HandlePaddleCollision(Ball& ball, PongWall& paddle, bool isLeftPaddle)
{
    Rectangle paddleRec = { paddle.position.x, paddle.position.y, (float)paddle.width, (float)paddle.Height };

    if (CheckCollisionCircleRec(ball.position, ball.ballSize, paddleRec))
    {
        float paddleCenter = paddle.position.y + (paddle.Height / 2.0f);
        float normalizedHit = (ball.position.y - paddleCenter) / (paddle.Height / 2.0f);

        float maxAngle = 60.0f * (PI / 180.0f);
        float bounceAngle = normalizedHit * maxAngle;

        ball.speed += 15.0f;

        float dirX = isLeftPaddle ? 1.0f : -1.0f;

        ball.direction.x = dirX * cos(bounceAngle);
        ball.direction.y = sin(bounceAngle);

        if (isLeftPaddle)
        {
            ball.position.x = paddle.position.x + paddle.width + ball.ballSize;
        }
        else
        {
            ball.position.x = paddle.position.x - ball.ballSize;
        }
    }
}

class Game
{
public:
    Vector2 wallOnePos = { 30, 200 };
    Vector2 wallTwoPos = { 855, 200 };

    PongWall wallOne = PongWall(wallOnePos);
    PongWall wallTwo = PongWall(wallTwoPos);

    Ball ball = Ball({ 900 / 2.0f , 480 / 2.0f });

    float speed = 250.f;

    Game() {}

    void Update()
    {
        wallOne.position = wallOnePos;
        wallTwo.position = wallTwoPos;
        wallOne.Draw();
        wallTwo.Draw();
        ball.Draw();
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

        // Paddle Movement (Left)
        if (IsKeyDown(KEY_W))
        {
            game.wallOnePos.y -= game.speed * dataFrame;
            if (game.wallOnePos.y <= 0) game.wallOnePos.y = 0;
        }
        if (IsKeyDown(KEY_S))
        {
            if (game.wallOnePos.y + game.wallOne.Height < screenHeight)
            {
                game.wallOnePos.y += game.speed * dataFrame;
            }
        }

        // Paddle Movement (Right)
        if (IsKeyDown(KEY_UP))
        {
            game.wallTwoPos.y -= game.speed * dataFrame;
            if (game.wallTwoPos.y <= 0) game.wallTwoPos.y = 0;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            if (game.wallTwoPos.y + game.wallTwo.Height < screenHeight)
            {
                game.wallTwoPos.y += game.speed * dataFrame;
            }
        }

        // Ball Movement
        game.ball.position.x += game.ball.direction.x * game.ball.speed * dataFrame;
        game.ball.position.y += game.ball.direction.y * game.ball.speed * dataFrame;

        // Top & Bottom Collisions
        if (game.ball.position.y - game.ball.ballSize <= 0)
        {
            game.ball.position.y = game.ball.ballSize;
            game.ball.direction.y *= -1;
        }
        else if (game.ball.position.y + game.ball.ballSize >= screenHeight)
        {
            game.ball.position.y = screenHeight - game.ball.ballSize;
            game.ball.direction.y *= -1;
        }

        // Paddle Collisions (Checked based on ball X velocity direction)
        if (game.ball.direction.x < 0)
        {
            HandlePaddleCollision(game.ball, game.wallOne, true);
        }
        else if (game.ball.direction.x > 0)
        {
            HandlePaddleCollision(game.ball, game.wallTwo, false);
        }

        if (game.ball.position.x + game.ball.ballSize < 0)
        {
            game.wallOne.score += 1;
            game.ball.reset(1.0f);
        }
        else if (game.ball.position.x - game.ball.ballSize > screenWidth)
        {
            game.wallTwo.score += 1;
            game.ball.reset(-1.0f);
        }

        // Draw Frame
        BeginDrawing();
        ClearBackground(BLACK);
        game.Update();
        DrawText(TextFormat("%d",game.wallOne.score), screenHeight / 3, 20, 40, WHITE);
        DrawText(TextFormat("%d" ,game.wallTwo.score), (screenHeight / 2)*3, 20, 40, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}