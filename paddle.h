#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "vector2.h"

void Draw(SDL_Renderer* mRenderer);

const int PADDLE_HEIGHT = 20;
const int PADDLE_WIDTH = 100;

class Paddle {
    public:
        Paddle(Vector2 position): position(position) {
            paddleRect.x = static_cast<int>(position.x);
		    paddleRect.y = static_cast<int>(position.y);
		    paddleRect.w = PADDLE_WIDTH;
		    paddleRect.h = PADDLE_HEIGHT;
        }

        Vector2 position;
        SDL_Rect paddleRect;

        void Draw(SDL_Renderer* mRenderer);
};

void Paddle::Draw(SDL_Renderer* mRenderer) {
    paddleRect.x = static_cast<int>(position.x);

	SDL_RenderFillRect(mRenderer, &paddleRect);
}