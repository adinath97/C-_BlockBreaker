#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "vector2.h"

void Draw(SDL_Renderer* mRenderer);

const int BALL_HEIGHT = 20;
const int BALL_WIDTH = 20;

class Ball {
    public:
        Ball(Vector2 position): position(position) {
            ballRect.x = static_cast<int>(position.x);
		    ballRect.y = static_cast<int>(position.y);
		    ballRect.w = BALL_WIDTH;
		    ballRect.h = BALL_HEIGHT;
        }

        Vector2 position;
        SDL_Rect ballRect;

        void Draw(SDL_Renderer* mRenderer);
};

void Ball::Draw(SDL_Renderer* mRenderer) {
    ballRect.x = static_cast<int>(position.x);
    ballRect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(mRenderer, &ballRect);
}