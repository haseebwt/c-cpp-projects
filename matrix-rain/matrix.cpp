#include <iostream>
#include <ncurses.h>
#include <random>
#include <string>

int getRandomValue(int min, int max);
int HEIGHT;
int WIDTH;
struct Color
{
        int r;
        int g;
        int b;
        int a;
};

Color GREEN = {0, 255, 0, 255};
Color LIME = {104, 255, 104, 255};

struct Drop
{
        int x, y;
        int speed;
        std::string symbol;

        Drop(int pX, int pY, int pSpeed, Color pColor = GREEN)
        {
                x = pX;
                y = pY;
                speed = pSpeed;
                symbol = getRandomSymbol();
        }

        void render()
        {
                mvprintw(y, x, symbol.c_str(), "%s");
        }

        void update()
        {
                if (y >= WIDTH)
                {
                        y = 0;
                }
                else
                {
                        y += speed;
                }
        }

        std::string getRandomSymbol()
        {
                std::string c;
                c.push_back((char)getRandomValue(33, 125));

                return c;
        }
};

int main(int argc, char *argv[])
{
        std::cout << "Matrix ncurses!\n";
        Drop d(20, 20, 2);

        initscr();
        curs_set(0);
        getmaxyx(stdscr, HEIGHT, WIDTH);

        bool isRunning = true;

        while (isRunning)
        {
                napms(100); // 10 cycles per second

                d.update();
                d.render();
                refresh();
        }
        endwin();
        return 0;
}

int getRandomValue(int min, int max)
{
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);

        return distrib(gen);
}
