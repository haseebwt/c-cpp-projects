#include <iostream>
#include <raylib.h>
#include <vector>

#define WIDTH 1280
#define HEIGHT 720
#define RES 10
#define COLS WIDTH / RES
#define ROWS HEIGHT / RES
#define GRAVITY 0.9

std::vector<std::vector<int>> make2DArray()
{
        std::vector<std::vector<int>> vec(COLS, std::vector<int>(ROWS, 0));
        return vec;
}

std::vector<std::vector<float>> make2DFloatArray()
{
        std::vector<std::vector<float>> vec(COLS, std::vector<float>(ROWS, 0));
        return vec;
}

bool withinBounds(int val, int counter)
{
        return val >= 0 && val <= counter - 1;
}

int main()
{
        std::vector<std::vector<int>> canvas;
        std::vector<std::vector<int>> newCanvas;

        canvas = make2DArray();

        std::vector<std::vector<float>> gravityGrid;
        std::vector<std::vector<float>> newGraviyGrid;

        std::cout << ROWS << std::endl;

        Vector2 mouseState{-1};
        int mX{-1}, mY{-1};

        int matrix{-1}, extent{-1};
        matrix = 2;
        extent = 2;

        int hue{-1}, hueIncrement{-1};
        Color currColor;
        hue = 1;
        hueIncrement = 1;

        int state{-1}, bottom{-1};
        int dir{-1}, belowA{-1}, belowB{-1};
        dir = 1;

        InitWindow(WIDTH, HEIGHT, "Falling Sand");
        SetTargetFPS(60);

        bool isRunning = true;
        while (isRunning)
        {
                if (IsKeyPressed(KEY_X))
                {
                        isRunning = false;
                }
                if (IsKeyPressed(KEY_N))
                {
                        canvas = make2DArray();
                }

                hue = (hue >= 360) ? 0 : ++hueIncrement;

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                {
                        mouseState = GetMousePosition();
                        if (withinBounds(mouseState.x, WIDTH) && withinBounds(mouseState.y, HEIGHT))
                        {
                                mX = mouseState.x / RES;
                                mY = mouseState.y / RES;
                        }

                        // canvas[mX][mY] = hue;
                        for (int i = -extent; i < matrix; i++)
                        {
                                for (int j = -extent; j < matrix; j++)
                                {
                                        if (withinBounds(mX + i, COLS) && withinBounds(mY + j, ROWS) &&
                                            canvas[mX][mY] == 0)
                                        {
                                                canvas[mX + i][mY + j] = hue;
                                        }
                                }
                        }
                }

                BeginDrawing();
                ClearBackground(BLACK);
                for (int x = 0; x < COLS; x++)
                {
                        for (int y = 0; y < ROWS; y++)
                        {
                                if (canvas[x][y] > 0)
                                {
                                        currColor = ColorFromHSV(canvas[x][y], 1.f, 1.f);
                                        DrawRectangle(x * RES, y * RES, RES, RES, currColor);
                                }
                        }
                }
                EndDrawing();

                newCanvas = make2DArray();
                newGraviyGrid = make2DFloatArray();
                for (int x = 0; x < COLS; x++)
                {
                        for (int y = 0; y < ROWS; y++)
                        {
                                state = canvas[x][y];

                                bottom = -1;
                                belowA = -1;
                                belowB = -1;

                                if (state > 0)
                                {
                                        if (y == ROWS - 1)
                                        {
                                                newCanvas[x][y] = state;
                                                break;
                                        }

                                        bottom = canvas[x][y + 1];
                                        if (withinBounds(x + dir, COLS))
                                                belowA = canvas[x + dir][y + 1];

                                        if (withinBounds(x - dir, COLS))
                                                belowB = canvas[x - dir][y + 1];

                                        if (bottom == 0)
                                        {
                                                newCanvas[x][y + 1] = state;
                                        }
                                        else if (belowA == 0 && withinBounds(x + 1, COLS))
                                        {
                                                newCanvas[x + 1][y + 1] = state;
                                        }
                                        else if (belowB == 0 && withinBounds(x - 1, COLS))
                                        {
                                                newCanvas[x - 1][y + 1] = state;
                                        }
                                        else
                                        {
                                                newCanvas[x][y] = state;
                                        }
                                }
                        }
                }

                canvas = newCanvas;
        }

        std::cout << "Program Commenced!\n";
        return 0;
}
