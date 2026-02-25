// g++ snake.cpp -o main `pkg-config --libs --cflags sdl3 sdl3-ttf sdl3-mixer` && ./main

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>
#include <vector>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

int state{0};

struct Snake
{
        int x, y;
        int score{-1};

        int SNAKE_SPEED{-1};
        float CELL_SIZE{10.0f};
        std::vector<int> snakePos;
        std::vector<std::vector<int>> snakeBody;

        std::vector<int> foodPos;
        float FOOD_SIZE{10.0f};
        int foodSpawn{-1};

        std::string direction;
        std::string changeTo;

        SDL_FRect snakeCell;
        SDL_FRect foodRect;

        // snakeColor = (99, 60, 45);
        // appleColor = (155, 57, 21);

        /* AUDIO */
        // The mixer to output to
        MIX_Mixer *gMixer{nullptr};
        // The sound effects that will be used
        MIX_Audio *gDeathAudio{nullptr};
        MIX_Audio *gEatAudio{nullptr};

        // The track the sound effects will be played on
        MIX_Track *gEffectTrack{nullptr};

        Snake()
        {
                foodRect = (SDL_FRect){0, 0, FOOD_SIZE, FOOD_SIZE};
                foodSpawn = 1;
                snakeCell = (SDL_FRect){0, 0, CELL_SIZE, CELL_SIZE};
                SNAKE_SPEED = 10;

                direction = "RIGHT";
                changeTo = direction;

                /* INIT AUDIO */
                if (!MIX_Init())
                {
                        SDL_Log("Mixer INIT Error: %s", SDL_GetError());
                }
                gMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
                if (gMixer == nullptr)
                {
                        SDL_Log("Mixer Error: %s", SDL_GetError());
                }

                gDeathAudio = MIX_LoadAudio(gMixer, "./death.wav", true);
                gEatAudio = MIX_LoadAudio(gMixer, "./eat.wav", true);

                gEffectTrack = MIX_CreateTrack(gMixer);
        }
        ~Snake()
        {
                // Sound effect track
                MIX_DestroyTrack(gEffectTrack);
                gEffectTrack = nullptr;

                // Free sound effects
                MIX_DestroyAudio(gDeathAudio);
                gDeathAudio = nullptr;
                MIX_DestroyAudio(gEatAudio);
                gEatAudio = nullptr;

                // Free the mixer
                MIX_DestroyMixer(gMixer);
                gMixer = nullptr;
        }
        void initSnake()
        {
                x = WINDOW_WIDTH / 2;
                y = WINDOW_HEIGHT / 2;
                score = 0;
                snakePos = {x, y};
                snakeBody = {{x, y}, {x - 10, y}, {x - 20, y}, {x - 30, y}};
                foodPos = {200, 100};
        }

        void spawnFood()
        {
                srand(time(0));
                int x = (rand() % WINDOW_WIDTH / 10) * 10;
                int y = (rand() % WINDOW_HEIGHT / 10) * 10;
                foodPos = {x, y};
        }

        void changeDirection(SDL_Keycode key)
        {
                switch (key)
                {
                case SDLK_W:
                case SDLK_UP:
                        changeTo = "UP";
                        break;
                case SDLK_S:
                case SDLK_DOWN:
                        changeTo = "DOWN";
                        break;
                case SDLK_A:
                case SDLK_LEFT:
                        changeTo = "LEFT";
                        break;
                case SDLK_D:
                case SDLK_RIGHT:
                        changeTo = "RIGHT";
                        break;
                }
        }
        void checkOverlap()
        {
                if (changeTo == "UP" && direction != "DOWN")
                {
                        direction = "UP";
                }
                if (changeTo == "DOWN" && direction != "UP")
                {
                        direction = "DOWN";
                }
                if (changeTo == "LEFT" && direction != "RIGHT")
                {
                        direction = "LEFT";
                }
                if (changeTo == "RIGHT" && direction != "LEFT")
                {
                        direction = "RIGHT";
                }
        }

        void moveSnake()
        {
                if (direction == "UP")
                {
                        snakePos[1] -= SNAKE_SPEED;
                }
                if (direction == "DOWN")
                {
                        snakePos[1] += SNAKE_SPEED;
                }
                if (direction == "LEFT")
                {

                        snakePos[0] -= SNAKE_SPEED;
                }
                if (direction == "RIGHT")
                {
                        snakePos[0] += 10;
                }
        }

        void wrapAround()
        {
                if (snakePos[0] < 0)
                {
                        snakePos[0] = WINDOW_WIDTH;
                }
                else if (snakePos[0] > WINDOW_WIDTH - 10)
                {
                        snakePos[0] = 0;
                }
                if (snakePos[1] < 0)
                {
                        snakePos[1] = WINDOW_HEIGHT;
                }
                else if (snakePos[1] > WINDOW_HEIGHT - 10)
                {
                        snakePos[1] = 0;
                }
        }

        void gameLogic(SDL_Renderer *pRenderer)
        {
                checkOverlap();
                moveSnake();

                snakeBody.insert(snakeBody.begin(), snakePos);
                if (snakePos[0] == foodPos[0] && snakePos[1] == foodPos[1])
                {
                        score++;
                        foodSpawn = 0;

                        MIX_SetTrackAudio(gEffectTrack, gEatAudio);
                        MIX_PlayTrack(gEffectTrack, 0);
                }
                else
                {
                        snakeBody.pop_back();
                }

                if (!foodSpawn)
                {
                        spawnFood();
                        foodSpawn = 1;
                }
                for (int i = 0; i < snakeBody.size(); i++)
                {
                        auto snakeScale = snakeBody[i];
                        snakeCell.x = snakeScale[0];
                        snakeCell.y = snakeScale[1];

                        if (i == 0)
                        {
                                SDL_SetRenderDrawColor(pRenderer, 147, 91, 69, 255);
                        }
                        else
                        {
                                SDL_SetRenderDrawColor(pRenderer, 90, 60, 45, 255);
                        }

                        SDL_RenderFillRect(pRenderer, &snakeCell);
                }

                for (int i = 1; i < snakeBody.size(); i++)
                {
                        auto cell = snakeBody[i];
                        if (snakePos[0] == cell[0] && snakePos[1] == cell[1])
                        {
                                MIX_SetTrackAudio(gEffectTrack, gDeathAudio);
                                MIX_PlayTrack(gEffectTrack, 0);

                                state = 2;
                        }
                }

                foodRect.x = foodPos[0];
                foodRect.y = foodPos[1];

                SDL_SetRenderDrawColor(pRenderer, 155, 57, 21, 255);
                SDL_RenderFillRect(pRenderer, &foodRect);

                wrapAround();
        }
};

Snake snake;

struct gameWindow
{
        SDL_Window *gWindow{nullptr};
        SDL_Renderer *gRenderer{nullptr};
        TTF_Font *mononokiFont{nullptr};

        Uint32 windowColor;
        int running{1};

        gameWindow(const char *pTitle, int pWindowWidth, int pWindowHeight)
        {
                if (!TTF_Init())
                        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());

                if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
                        SDL_Log("Failed initialization: %s", SDL_GetError());

                gWindow = SDL_CreateWindow(pTitle, pWindowWidth, pWindowHeight, 0);
                gRenderer = SDL_CreateRenderer(gWindow, nullptr);

                mononokiFont = TTF_OpenFont("./mononoki-Regular.ttf", 26);
                if (!mononokiFont)
                {
                        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
                }
        }
        ~gameWindow()
        {
                SDL_DestroyWindow(gWindow);
                SDL_DestroyRenderer(gRenderer);
                TTF_CloseFont(mononokiFont);
                TTF_Quit();
                SDL_Quit();
        }

        void eventHandler()
        {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_X)
                                running = 0;
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;
                        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE)
                        {
                                if (state == 0 || state == 2)
                                {
                                        state = 1;
                                        snake.initSnake();
                                }
                        }
                        if (event.type == SDL_EVENT_KEY_DOWN)
                        {
                                snake.changeDirection(event.key.key);
                        }
                }
        }

        void getText(const char *phrase, bool center, float posX = 0, float posY = 0, int inc = 1)
        {
                SDL_Surface *text{nullptr};
                SDL_Texture *textTexture{nullptr};

                SDL_Color color = (SDL_Color){252, 251, 224, 255};

                /* Create the text */
                text = TTF_RenderText_Solid(mononokiFont, phrase, 0, color);
                if (text)
                {
                        textTexture = SDL_CreateTextureFromSurface(gRenderer, text);
                        SDL_DestroySurface(text);
                }
                if (!textTexture)
                {
                        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
                }

                SDL_FRect textDst;
                textDst.x = posX;
                textDst.y = posY;

                int w = 0, h = 0;

                SDL_GetRenderOutputSize(gRenderer, &w, &h);

                SDL_GetTextureSize(textTexture, &textDst.w, &textDst.h);
                if (center && posX && posY > 0)
                {
                        if (!inc)
                        {
                                textDst.x = static_cast<int>(WINDOW_WIDTH / 2) + posX;
                                textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) + posY;
                        }
                        else
                        {
                                textDst.x = static_cast<int>(WINDOW_WIDTH / 2) - posX;
                                textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) - posY;
                        }
                }
                else if (center && posX > 0)
                {
                        if (!inc)
                        {
                                textDst.x = static_cast<int>(WINDOW_WIDTH / 2) + posX;
                                textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) - static_cast<int>(textDst.h / 2);
                        }
                        else
                        {
                                textDst.x = static_cast<int>(WINDOW_WIDTH / 2) - posX;
                                textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) - static_cast<int>(textDst.h / 2);
                        }
                }
                else if (center && posY > 0)
                {

                        if (!inc)
                        {
                                textDst.x = static_cast<int>(WINDOW_WIDTH / 2) - static_cast<int>(textDst.w / 2);
                                textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) + posY;
                        }
                        else
                        {
                                textDst.x = static_cast<int>(WINDOW_WIDTH / 2) - static_cast<int>(textDst.w / 2);
                                textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) - posY;
                        }
                }
                else if (center)
                {
                        textDst.x = static_cast<int>(WINDOW_WIDTH / 2) - static_cast<int>(textDst.w / 2);
                        textDst.y = static_cast<int>(WINDOW_HEIGHT / 2) - static_cast<int>(textDst.h / 2);
                }

                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
                SDL_RenderTexture(gRenderer, textTexture, NULL, &textDst);
        }
        void introWindow()
        {
                getText("Welcome to Snake!", true, 0, 100.0f);
                getText("Press Space to start", true, 0, 100.0f, 0);
                SDL_RenderPresent(gRenderer);
        }

        void gameOverWindow()
        {
                SDL_RenderClear(gRenderer);
                getText("Game Over!", true, 0.0f, 200.0f);

                std::string scr = "Score: ";
                int score = snake.score;
                scr += std::to_string(score);

                getText(scr.c_str(), true, 0, 200.0f, 0);
                getText("Press Space to start again :3", true);
                getText("Press X to Exit", true, 0.0f, 50.0f, 0);
                SDL_RenderPresent(gRenderer);
        }

        void stateHandler()
        {
                if (state == 0)
                {
                        introWindow();
                }
                else if (state == 1)
                {
                        snake.gameLogic(gRenderer);
                        std::string scr = "Score: ";
                        int score = snake.score;
                        scr += std::to_string(score);

                        getText(scr.c_str(), true, 0, 200.0f, 1);
                        SDL_RenderPresent(gRenderer);
                }
                else if (state == 2)
                {
                        gameOverWindow();
                }
        }

        void updateWindow()
        {
                SDL_SetRenderDrawColor(gRenderer, 35, 18, 12, 255);
                SDL_RenderFillRect(gRenderer, nullptr);
                SDL_RenderClear(gRenderer);
        }

        void gameLoop()
        {
                while (running)
                {
                        updateWindow();
                        stateHandler();
                        eventHandler();
                        usleep(40000);
                }
        }
};

int main()
{
        gameWindow window("Snake", WINDOW_WIDTH, WINDOW_HEIGHT);
        window.gameLoop();

        return 0;
}
