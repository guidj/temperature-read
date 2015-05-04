#include <iostream>

int main(int charc, char** argv)
{
    srand (time(NULL));

    int temperature = rand() % 30 + 12;

    std::cout << temperature;
}
