#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

#include <glad/glad.h>

#include "block.h"
#include "ball.h"
#include "paddle.h"

bool Initialize();

void ShutDown();

void RunLoop();

void ProcessInput();

void UpdateGame();

void GenerateOutput();

void CheckPaddleCollision(Vector2& mBallPos, Vector2& mPaddlePos, Vector2& mBallVel, float const& thickness, float paddleH, float const& moveSpeed);

class Game {
    public:
        Game():mWindow(nullptr), mIsRunning(true) {
            mTicksCount = 0;
        }

        ~Game() {
            delete [] mBlockRects;
            mBlockRects = 0;
        }

        bool Initialize();
        void RunLoop();
        void ShutDown();
    
    private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Uint32 mTicksCount;
        
        bool mIsRunning, startRound, ballCollisionValue;

        float moveSpeed = 500.0f, deltaTime;

        const float thickness = 20.0f;

        int mPaddleDir = 0;

        Vector2 mBallVel = Vector2(-300.0f,-300.0f);
        Vector2 mPaddlePos = Vector2(475.0f,768.0f - thickness * 2);
        Vector2 mBallPos = Vector2(mPaddlePos.x + BALL_WIDTH * 2,mPaddlePos.y - 1.25 * BALL_HEIGHT);

        //initialize blocks
        Block* mBlockRects = new Block[15];

        //track blocks as destroyed
        bool mBlockStatus[15] = { false };

        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
};

bool Game::Initialize() {
    //random number generator seed
    srand (time(NULL));

    //initialize SDL library
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0) {
        //initialization failed
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    //if initialization successful, create window
    mWindow = SDL_CreateWindow(
        "Block Breaker",
        100,
        100,
        1024,
        768,
        0 // Flags
    );

    if(!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //initialization flags. use accelerated renderer and enable vsync
    );

    //if window and initialization successful, return true
    return true;
}

void Game::ShutDown() {
    //destroy SDL_Window
    SDL_DestroyWindow(mWindow);

    //destroy SDL_Renderer
    SDL_DestroyRenderer(mRenderer);

    //close SDL
    SDL_Quit();
}

void Game::ProcessInput() {
    SDL_Event event;

    //go through all events and respond as desired/appropriate
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    // Retrieve the state of all of the keys then scan each as desired
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    } if(state[SDL_SCANCODE_SPACE]) {
        startRound = true;
    }

    //update paddle movement
    mPaddleDir = 0;

    if(state[SDL_SCANCODE_LEFT]) {
        mPaddleDir -= 1;
    } else if(state[SDL_SCANCODE_RIGHT]) {
        mPaddleDir += 1;
    }
}

void Game::UpdateGame() {
    //update frame at fixed intervals (fixedDeltaTime)
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    //get deltaTime
    deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    //clamp deltaTime
    if(deltaTime > .05f) {
        deltaTime = 0.05f;
    }

    if(!startRound) { return; }

    //move ball
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    //move paddle
    if(mPaddleDir != 0) {
        mPaddlePos.x += mPaddleDir * 600.0f * deltaTime;
        //clamp paddlePos
        if(mPaddlePos.x < 1.1 * thickness) {
            mPaddlePos.x = 1.1 * thickness;
        } else if(mPaddlePos.x > 1024 - 6.1 * thickness) {
            mPaddlePos.x = 1024 - 6.1 * thickness;
        }
    }

    //collision detection
    if(mBallPos.y < thickness && mBallVel.y < 0.0f) { //if moving upwards and hit wall
        mBallVel.y *= -1;
    } else if(mBallPos.x < thickness && mBallVel.x < 0.0f) {
        mBallVel.x *= -1;
    } else if(mBallPos.x > 1024 - 2 * thickness && mBallVel.x > 0.0f) {
        mBallVel.x *= -1;
    }

    //check for collision with paddle
    CheckPaddleCollision(mBallPos,mPaddlePos,mBallVel,thickness,thickness * 5,moveSpeed);

    //check for collisions with blocks
    for(int i = 0; i < 15; i++) {
        if(mBlockStatus[i] == false) {
            mBlockStatus[i] = mBlockRects[i].CheckForCollision(mBallPos,mBallVel);
        }
    }
}

void CheckPaddleCollision(Vector2& mBallPos, Vector2& mPaddlePos, Vector2& mBallVel, float const& thickness, float paddleH, float const& moveSpeed) {

    if(mBallVel.y < 0.0f) { return; }
    
    float ballLeft = mBallPos.x;
	float ballRight = mBallPos.x + thickness;
	float ballTop = mBallPos.y;
	float ballBottom = mBallPos.y + thickness;

	float paddleLeft = mPaddlePos.x;
	float paddleRight = mPaddlePos.x + paddleH;
	float paddleTop = mPaddlePos.y;
	float paddleBottom = mPaddlePos.y + thickness;

    float paddleMid = mPaddlePos.x + paddleH;
    float ballMid = mBallPos.x + 0.5f * thickness;

	if (ballLeft >= paddleRight) { return; }

	if (ballRight <= paddleLeft) { return; }

	if (ballTop >= paddleBottom) { return; }

	if (ballBottom <= paddleTop) { return; }

    float relativeIntersectY = paddleMid - ballMid; //between -32 and 32

    float normalizedRelativeIntersectionY = (relativeIntersectY/(paddleH/2.0f)); //between -1 and 1 -- normalized

    float MAXBOUNCEANGLE = (5.0f * M_PI) / 12.0f;

    float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;

    float paddleType = 1.0f;

    mBallVel.x = moveSpeed * cos(bounceAngle) * paddleType;
    mBallVel.y = moveSpeed * sin(bounceAngle) * -1.0f;
}

void Game::GenerateOutput() {
    float yPos = 0.0f, xPos = 0.0f;

    SDL_SetRenderDrawColor(mRenderer,0,0,0,0);

    SDL_RenderClear(mRenderer); //clear back buffer to current draw color

    SDL_SetRenderDrawColor(mRenderer,255,255,255,255);

    SDL_Rect topWall = {0,0,1024,thickness};
    SDL_Rect rightWall = {0,0,thickness,768};
    SDL_Rect leftWall = {1004,0,thickness,768};

    // Draw the blocks. 3 rows containing 10 blocks each
    for(int i = 0; i < 15; i++) {

       if(i < 5) {
            yPos = 100.0f;
            xPos = i * 225 + 30;
        } else if(i < 10 && i >= 5) {
            yPos = 200.0f;
            xPos = (i - 5) * 225 + 30;
        } else {
            yPos = 300.0f;
            xPos = (i - 10) * 225 + 30;
        }
           
        Block block(Vector2(xPos, yPos));
        mBlockRects[i] = block;
    }

    for(int i = 0; i < 15; i++) {
        if(mBlockStatus[i] == false) {
            mBlockRects[i].Draw(mRenderer);
        }
    }

    SDL_RenderFillRect(mRenderer,&topWall);
    SDL_RenderFillRect(mRenderer,&rightWall);
    SDL_RenderFillRect(mRenderer,&leftWall);

    //create paddle
    Paddle paddle(Vector2(mPaddlePos.x,mPaddlePos.y));

    //create ball
    Ball ball(Vector2(mBallPos.x,mBallPos.y));

    // Draw the ball
    ball.Draw(mRenderer);

    //draw paddle
    paddle.Draw(mRenderer);

    SDL_RenderPresent(mRenderer);
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}
