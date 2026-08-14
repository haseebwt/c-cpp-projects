#include <iostream>
#include <raylib.h>
#include <vector>

#define WIDTH 1280
#define HEIGHT 720

#define SIGMA 10
#define RHO 28
#define BETA 8 / 3

struct Line
{
        Vector3 pos;
        float hue;
        float sat;
};

int main()
{
        InitWindow(WIDTH, HEIGHT, "Lorenz Attractor");
        SetTargetFPS(60);

        float x = 0.1, y = 0, z = 0;

        float dt = 0.01; // to add those sweet curves
        float dx = SIGMA * (y - x);
        float dy = x * (RHO - z) - y;
        float dz = x * y - (float)BETA * z;

        // First attractor colors
        float hue{48.f};
        float hueOffset{0.2};

        float sat{0.7};
        float satOffset{0.02};

        // First
        std::vector<Line> points;
        points.push_back((Line){(Vector3){x, y, z}, hue, sat});

        // Define the camera to look into our 3d world
        Camera3D camera = Camera3D{0};
        camera.position = (Vector3){-152.0f, 47.0f, 36.0f}; // Camera position
        camera.target = (Vector3){-5.0f, 5.0f, 26.0f};      // Camera looking at point
        camera.up = (Vector3){0.0f, 0.0f, 1.0f};            // Camera up vector (rotation towards target)
        camera.fovy = 45.0f;                                // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

        // To use less memory iguess
        Vector3 start{0};
        Vector3 end{0};
        float h{0};
        float s{0};

        bool isRunning = true;
        bool engageCamera = false;

        while (isRunning)
        {
                if (engageCamera)
                {
                        UpdateCamera(&camera, CAMERA_ORBITAL);
                }

                if (IsKeyPressed(KEY_Z))
                {
                        camera.target = (Vector3){0.0f, 0.0f, 0.0f};
                }
                else if (IsKeyPressed(KEY_O))
                {
                        engageCamera = (engageCamera) ? false : true; // toggle camera
                }
                else if (IsKeyPressed(KEY_X))
                {
                        isRunning = false;
                }
                else if (IsKeyPressed(KEY_BACKSPACE))
                {
                        x = 1, y = 1, z = 1;
                        points.clear();
                        points.push_back((Line){Vector3{x, y, z}});
                }
                else if (IsKeyPressed(KEY_M))
                {
                        std::cout << camera.position.x << " " << camera.position.y << " " << camera.position.z << "\n";
                        std::cout << camera.target.x << " " << camera.target.y << " " << camera.target.z << "\n";
                }

                // Compute
                // First
                dx = (SIGMA * (y - x)) * dt;
                dy = ((x * (RHO - z)) - y) * dt;
                dz = (x * y - (float)BETA * z) * dt;

                x = x + dx;
                y = y + dy;
                z = z + dz;

                hue += hueOffset;
                sat += satOffset;

                if (hue < 33)
                {
                        hueOffset = 0.2;
                }
                else if (hue > 47)
                {
                        hueOffset = -0.2;
                }

                if (sat > 0.9)
                {
                        satOffset = -0.02;
                }
                else if (sat < 0.7)
                {
                        satOffset = 0.02;
                }

                points.push_back((Line){Vector3{x, y, z}, hue, sat});

                // Draw
                BeginDrawing();

                ClearBackground(BLACK);

                // Begin 3D
                BeginMode3D(camera);

                for (unsigned long int i = 1; i < points.size() - 1; i++)
                {
                        start = points.at(i - 1).pos;
                        end = points.at(i).pos;
                        h = points.at(i).hue;
                        s = points.at(i).sat;

                        DrawLine3D(start, end, ColorFromHSV(h, s, 1));
                }

                EndMode3D();

                EndDrawing();
        }

        std::cout << "Lorenz Attractor!\n";
        return 0;
}
