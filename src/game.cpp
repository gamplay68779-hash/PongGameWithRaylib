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
    bool canMove = true;
    float delayTimer = 0.0f;
    Color color = WHITE;
    Ball(Vector2 pos)
    {
        position = pos;
    }

    void Draw()
    {
        DrawCircle(position.x, position.y, ballSize, color);
    }

    void reset(float dirX ,Sound wonSound)
    {
        position = { 900 / 2.0f , 480 / 2.0f };
        speed = 250.f;
        direction = { dirX, 0.0f };
        canMove = false;
        delayTimer = 1.5f;
        PlaySound(wonSound);
        color = WHITE;
    }
    void UpdateTimer(float deltaTime)
    {
        if (!canMove)
        {
            delayTimer -= deltaTime;
            if (delayTimer <= 0.0f)
            {
                canMove = true;
            }
        }
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

    void Draw(Color color)
    {
        DrawRectangle(position.x, position.y, width, Height, color);
    }
};

void HandlePaddleCollision(Ball& ball, PongWall& paddle, bool isLeftPaddle , Sound paddleBouncSound)
{
    Rectangle paddleRec = { paddle.position.x, paddle.position.y, (float)paddle.width, (float)paddle.Height };

    if (CheckCollisionCircleRec(ball.position, ball.ballSize, paddleRec))
    {
        PlaySound(paddleBouncSound);
        ball.color = isLeftPaddle ? BLUE : PINK;
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

    //Sound
    Sound bouncWallSound;
    Sound bouncPaddleSound;
    Sound winSound;
    //Background Music
    Music backgroundMusic;
    //Background Image
    Texture2D backgroundImage;
    Rectangle sourceRec;
    Rectangle destRec = { 0.0f, 0.0f, (float)900, (float)510};
    Vector2 origin = { 0.0f, 0.0f };
    Game()
    {
        InitAudioDevice();
        Image bgimage = LoadImage("assets/Graphics/Background.png");
        backgroundImage = LoadTextureFromImage(bgimage);
        sourceRec = { 0.0f, 0.0f, (float)backgroundImage.width, (float)backgroundImage.height };
        UnloadImage(bgimage);
        bouncWallSound = LoadSound("assets/Sounds/BouceWallSound.mp3");
        bouncPaddleSound = LoadSound("assets/Sounds/PaddleBouncSound.mp3");
        winSound = LoadSound("assets/Sounds/WinSound.mp3");
        backgroundMusic = LoadMusicStream("assets/BackgroundMusic/BackgroundMusic.mp3");
        PlayMusicStream(backgroundMusic);
    }
    ~Game()
    {
        UnloadSound(bouncWallSound);
        UnloadSound(bouncPaddleSound);
        UnloadSound(winSound);
        UnloadMusicStream(backgroundMusic);
        UnloadTexture(backgroundImage);
        CloseAudioDevice();
    }

    void Update()
    {
        UpdateMusicStream(backgroundMusic);
        DrawTexturePro(backgroundImage, sourceRec, destRec, origin, 0.0f, WHITE);
        wallOne.position = wallOnePos;
        wallTwo.position = wallTwoPos;
        wallOne.Draw(BLUE);
        wallTwo.Draw(PINK);
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
        //Ball delay cooldown
        game.ball.UpdateTimer(dataFrame);

        // Ball Movement
        if (game.ball.canMove == true)
        {
            game.ball.position.x += game.ball.direction.x * game.ball.speed * dataFrame;
            game.ball.position.y += game.ball.direction.y * game.ball.speed * dataFrame;
        }

        // Top & Bottom Collisions
        if (game.ball.position.y - game.ball.ballSize <= 0)
        {
            game.ball.position.y = game.ball.ballSize;
            game.ball.direction.y *= -1;
            PlaySound(game.bouncWallSound);
        }
        else if (game.ball.position.y + game.ball.ballSize >= screenHeight)
        {
            game.ball.position.y = screenHeight - game.ball.ballSize;
            game.ball.direction.y *= -1;
            PlaySound(game.bouncWallSound);
        }

        // Paddle Collisions (Checked based on ball X velocity direction)
        if (game.ball.direction.x < 0)
        {
            HandlePaddleCollision(game.ball, game.wallOne, true , game.bouncPaddleSound);
        }
        else if (game.ball.direction.x > 0)
        {
            HandlePaddleCollision(game.ball, game.wallTwo, false, game.bouncPaddleSound);
        }

        if (game.ball.position.x + game.ball.ballSize < 0)
        {
            game.wallTwo.score += 1;
            game.ball.reset(-1.0f ,game.winSound);
        }
        else if (game.ball.position.x - game.ball.ballSize > screenWidth)
        {
            game.wallOne.score += 1;
            game.ball.reset(1.0f , game.winSound);
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