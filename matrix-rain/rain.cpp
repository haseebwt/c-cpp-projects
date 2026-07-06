#include <raylib.h>
#include <string>
#include <vector>

#define WIDTH 1920
#define HEIGHT 1080
#define DROP_SIZE 30

struct Drop
{
        int gX, gY;
        Color dropColor;
        std::string symbol;
        Font mFont;
        int intervalTime;
        int speed;

        Drop(int x, int y, Font font, int pSpeed, Color dropC = GREEN)
        {
                gX = x;
                gY = y;
                dropColor = dropC;
                symbol = getRandomDrop();
                mFont = font;
                speed = pSpeed;
        }

        void render()
        {

                DrawTextEx(mFont, symbol.c_str(), Vector2{(float)gX, (float)gY}, DROP_SIZE, DROP_SIZE, dropColor);
        }

        void update()
        {
                intervalTime = GetRandomValue(2, 20);
                int t = GetRandomValue(2, 3000);
                if (t % intervalTime == 0)
                {
                        this->symbol = getRandomDrop();
                }

                if (this->gY >= HEIGHT)
                {
                        this->gY = 0;
                }
                else
                {
                        this->gY += speed;
                }
        }

        std::string getRandomDrop()
        {
                std::string c;
                c.push_back((char)GetRandomValue(33, 125));

                return c;
        }
};

struct Stream
{
        int size;
        int y, x;
        Font gFont;
        std::vector<Drop> stream;
        bool isFirst;
        int speed;
        float b, decrement;

        Stream(int pX, Font font)
        {
                size = GetRandomValue(5, HEIGHT / DROP_SIZE);
                x = pX;
                y = -200;
                gFont = font;
                isFirst = true;
                speed = GetRandomValue(2, 18);
                b = 1.f;
                decrement = 0.01;
        }

        void init()
        {
                for (int i = 0; i <= size; i++)
                {
                        if (isFirst)
                        {
                                stream.push_back(Drop(x, y, gFont, speed, ColorFromHSV(107, 0.41, 0.99)));
                        }
                        else
                        {
                                stream.push_back(Drop(x, y, gFont, speed, ColorFromHSV(120, 1.f, b)));
                        }

                        y -= DROP_SIZE;
                        b -= decrement;
                        isFirst = false;
                }
        }

        void update()
        {
                for (Drop &d : stream)
                {
                        d.render();
                }
                for (Drop &d : stream)
                {
                        d.update();
                }
        }
};

int main(int argc, char *argv[])
{
        InitWindow(WIDTH, HEIGHT, "Matrix Rain");
        SetTargetFPS(60);
        Font font = LoadFont("./mononoki.ttf");

        std::vector<Stream> streams;
        for (int i = 0; i < WIDTH - 1; i += DROP_SIZE)
        {
                streams.push_back(Stream(i, font));
                streams.back().init();
        }

        bool isRunning = true;
        while (isRunning)
        {
                if (IsKeyDown(KEY_X))
                {
                        isRunning = false;
                }
                if (IsKeyDown(KEY_SPACE))
                {
                }
                BeginDrawing();
                ClearBackground(Color{0, 0, 0, 100});
                for (Stream &stream : streams)
                {
                        stream.update();
                }
                EndDrawing();
        }

        CloseWindow();
        UnloadFont(font);
        return 0;
}
