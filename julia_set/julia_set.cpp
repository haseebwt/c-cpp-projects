/*

By: Haseeb Khan ( Beeb )
Followed multiple documentations
Feel free to use

*/

#include <cmath>
#include <iostream>
#include <raylib.h>

#define WIDTH 1280
#define HEIGHT 720
#define MAX_ITERATION 100

double map_value(float value, float input_start, float input_end, float output_start, float output_end);

int main()
{
        float a{0}, b{0}, ca{0}, cb{0}, za{0}, zb{0};
        float bright{1}, sat{0.39f}, hue{331};
        float ANGLE{0.5};

        InitWindow(WIDTH, HEIGHT, "Julia Set");
        SetTargetFPS(60);
        bool isRunning = true;

        while (isRunning)
        {
                if (IsKeyPressed(KEY_X))
                {
                        isRunning = false;
                }

                unsigned int n{0};

                ca = ::cos(ANGLE * 4.1021);
                cb = ::sin(ANGLE);

                ANGLE += 0.05;

                BeginDrawing();
                for (unsigned int x = 0; x < WIDTH; x++)
                {
                        for (unsigned int y = 0; y < HEIGHT; y++)
                        {
                                a = map_value(x, 0, WIDTH, -3.5, 3.5);
                                b = map_value(y, 0, HEIGHT, -2.0, 2.0);

                                // ca = 0.285;
                                // cb = 0.01;

                                for (n = 0; n < MAX_ITERATION; n++)
                                {

                                        za = a * a - b * b;
                                        zb = 2.f * a * b;

                                        a = za + ca;
                                        b = zb + cb;

                                        if (a * a + b * b > 4.f)
                                        {
                                                break;
                                        }
                                }

                                bright = map_value(n, 0, MAX_ITERATION, 0.1f, 1.0f);
                                sat = map_value(n, 0, MAX_ITERATION, 0.39f, 0.99f);

                                if (n == MAX_ITERATION)
                                {
                                        bright = 0;
                                }

                                DrawPixelV(Vector2{float(x), float(y)}, ColorFromHSV(hue, sat, bright));
                        }
                }
                EndDrawing();
        }

        std::cout << "Mandlebrot Set!\n";
        return 0;
}

double map_value(float value, float input_start, float input_end, float output_start, float output_end)
{
        // Since the range [input_start, input_end] has input_end - input_start + 1 numbers. its equivallent to below
        // range
        double r = input_end - input_start;

        // Similarly [output_start, output_end] has output_end - output_start + 1 numbers.
        double R = output_end - output_start;

        // input is equivalent to
        double x = value - input_start;

        double y = 1.0 * (R / r) * x; // to get more accurate values we multiply 1.0

        double output = output_start + y;
        return output;
}
