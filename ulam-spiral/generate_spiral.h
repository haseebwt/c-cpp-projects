#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cmath>
#include <iostream>

#define SDL_HINT_RENDER_LINE_METHOD "SDL_RENDER_LINE_METHOD"

class GenerateSpiral
{
      public:
        int posX{-1};
        int posY{-1};
        int prevX{-1};
        int prevY{-1};

        int stepSize{-1};

        int state{0};
        int step{1};
        int numStep{1};
        int turnCounter{1};

        GenerateSpiral(int x, int y, int size)
        {
                posX = x;
                posY = y;
                stepSize = size;
        }

        bool isPrime(int num)
        {
                if (num == 1)
                        return false;

                for (int i = 2; i < (std::floor(std::sqrt(num)) + 1); i++)
                {
                        if (num % i == 0)
                        {
                                return false;
                        }
                }

                return true;
        }

        // Figure out which way the line should move to
        void moveLine()
        {
                if (step != 0)
                {
                        switch (state)
                        {
                        case 0:
                                posX += stepSize;
                                break;
                        case 1:
                                posY -= stepSize;
                                break;
                        case 2:
                                posX -= stepSize;
                                break;
                        case 3:
                                posY += stepSize;
                                break;
                        }
                }
        }

        void changeState()
        {
                if (step % numStep == 0)
                {
                        // This ensures that our state doesn't get any higher than 3
                        state = (state + 1) % 4;

                        turnCounter++;

                        // evenly increase the turn counter
                        if (turnCounter % 2 == 0)
                        {
                                numStep++;
                        }
                }
                step++;
        }

        void spiralMath()
        {
                moveLine();
                changeState();
        }

        int drawCircle(SDL_Renderer *renderer, int x, int y, int radius)
        {
                int offsetx, offsety, d;
                int status;
                offsetx = 0;
                offsety = radius;
                d = radius - 1;
                status = 0;

                while (offsety >= offsetx)
                {

                        status += SDL_RenderLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
                        status += SDL_RenderLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
                        status += SDL_RenderLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
                        status += SDL_RenderLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

                        if (status < 0)
                        {
                                status = -1;
                                break;
                        }

                        if (d >= 2 * offsetx)
                        {
                                d -= 2 * offsetx + 1;
                                offsetx += 1;
                        }
                        else if (d < 2 * (radius - offsety))
                        {
                                d += 2 * offsety - 1;
                                offsety -= 1;
                        }
                        else
                        {
                                d += 2 * (offsety - offsetx - 1);
                                offsety -= 1;
                                offsetx += 1;
                        }
                }

                return status;
        }

        void drawSpiral(SDL_Renderer *renderer)
        {
                SDL_FRect lineRect = (SDL_FRect){static_cast<float>(posX), static_cast<float>(posY)};

                if (step == 1)
                {

                        SDL_SetRenderDrawColor(renderer, 36, 18, 12, 0xFF);
                        SDL_RenderPoint(renderer, posX, posY);
                }
                else
                {
                        SDL_SetRenderDrawColor(renderer, 36, 18, 12, 0xFF);
                        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF); // white line
                        SDL_RenderLine(renderer, posX, posY, prevX, prevY);

                        if (isPrime(step - 1))
                        {

                                SDL_SetRenderDrawColor(renderer, 155, 57, 21, 0xFF);
                                drawCircle(renderer, prevX, prevY, 2);
                        }
                }
                prevX = posX;
                prevY = posY;
        }
};
