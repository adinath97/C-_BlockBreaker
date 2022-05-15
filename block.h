#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "vector2.h"
#pragma once

void Draw(SDL_Renderer* mRenderer);
bool CheckForCollision(Vector2& ballPosition, Vector2& mBallVel);

const int BLOCK_HEIGHT = 20;
const int BLOCK_WIDTH = 60;

class Block {
    public:
        Block() { }

        Block(Vector2 position): position(position) {
            blockRect.x = static_cast<int>(position.x);
		    blockRect.y = static_cast<int>(position.y);
		    blockRect.w = BLOCK_WIDTH;
		    blockRect.h = BLOCK_HEIGHT;
        }

        Vector2 position;
        SDL_Rect blockRect;
        bool hitTop, hitBottom, hitRight, hitLeft;

        void Draw(SDL_Renderer* mRenderer);
        bool CheckForCollision(Vector2& ballPosition, Vector2& mBallVel);
};

void Block::Draw(SDL_Renderer* mRenderer) {

    blockRect.x = static_cast<int>(position.x);

	SDL_RenderFillRect(mRenderer, &blockRect);
}

bool Block::CheckForCollision(Vector2& ballPosition, Vector2& mBallVel) {
    //determine if there is a collision, else return
    
    float ballLeft = ballPosition.x;
	float ballRight = ballPosition.x + 20.0f;
	float ballTop = ballPosition.y;
	float ballBottom = ballPosition.y + 20.0f;

	float blockLeft = position.x;
	float blockRight = position.x + BLOCK_WIDTH;
	float blockTop = position.y;
	float blockBottom = position.y + BLOCK_HEIGHT;

    float xblockMid = position.x + .5f * BLOCK_WIDTH;
    float yBlockMid = position.y + .5f * BLOCK_HEIGHT;

    float xballMid = ballPosition.x + .5f * BLOCK_HEIGHT;
    float yballMid = ballPosition.y + .5f * BLOCK_HEIGHT;

	if (ballLeft >= blockRight) { return false; }

	if (ballRight <= blockLeft) { return false; }

	if (ballTop >= blockBottom) { return false; }

	if (ballBottom <= blockTop) { return false; }

    float xOverlap, yOverlap;

    //if collision happens, determine if x overlap or y overlap dominates
    if(mBallVel.y < 0.0f) {
        yOverlap = fabs(blockBottom - ballTop); 
    } else if(mBallVel.y > 0.0f) {
        yOverlap = fabs(blockTop - ballBottom);
    }

    if(mBallVel.x > 0.0f) {
        xOverlap = fmin(fabs(blockRight - ballLeft),fabs(ballRight - ballLeft));
    } else if(mBallVel.x < 0.0f) {
        xOverlap = fmin(fabs(blockLeft - ballRight),fabs(ballRight - ballLeft));
    }

    if(xOverlap >= yOverlap) {
        if(blockBottom >= ballTop && mBallVel.y < 0.0f) { //if colliding on bottom part and ball moving upwazrds
            mBallVel.y = fabs(mBallVel.y);
            return true;
        } else if(blockTop <= ballBottom && mBallVel.y > 0.0f) { //if colliding on top part and ball moving downwards
            mBallVel.y = -fabs(mBallVel.y);
            return true;
        }
    } else {
        //determine which side collision is on and respond appropriately
        if(blockRight >= ballLeft && mBallVel.x < 0.0f) { //if colliding on right side and ball moving to left
            mBallVel.x = fabs(mBallVel.x);
            return true;
        } else if(ballRight >= blockLeft && mBallVel.x > 0.0f) {
            mBallVel.x = -fabs(mBallVel.x);
            return true;
        } 
    }

    return true;
}