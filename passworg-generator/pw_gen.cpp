#include <cstdlib>
#include <ctime>
#include <iostream>

char PASSWORD_CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$^&*()_-+";

int main(int argc, char *argv[])
{
        if (argc < 2)
        {
                std::cout << "Usage - pw-gen n \nn: length of password\n";
                return 1;
        }

        ::srand(time(NULL));
        int PASSWORD_SIZE = ::atoi(argv[1]);

        int i{0};
        while (i < PASSWORD_SIZE)
        {
                int random_number = ::rand() % (sizeof(PASSWORD_CHARACTERS) - 1);
                std::cout << PASSWORD_CHARACTERS[random_number];
                i++;
        }

        std::cout << "\n";
        return 0;
}
