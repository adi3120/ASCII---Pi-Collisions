#include <iostream>
#include <windows.h>
#include <cmath>

using std::cin;
using std::cout;
const int width = 80;
const int height = 35;
int count = 0;

class Block
{
public:
    float x;
    float y;
    float vel;
    float mass;
    char cell = char(219);

    Block(float lx, float ly, float v, float m)
    {
        x = lx;
        y = ly;
        vel = v;
        mass = m;
    }
    void drawBlock1(char *canvas)
    {

        int xarr[15] = {-1, 0, +1, -1, 0, +1, -1, 0, +1, -1, 0, +1, -1, 0, +1};
        int yarr[15] = {-2, -2, -2, -1, -1, -1, 0, 0, 0, +1, +1, +1, +2, +2, +2};

        for (int i = 0; i < 15; i++)
        {
            canvas[int(x + xarr[i]) + width * int(y + yarr[i])] = cell;
        }
    }

    void drawBlock2(char *canvas)
    {

        int xarr[9] = {-1, 0, +1, -1, 0, +1, -1, 0, +1};
        int yarr[9] = {-1, -1, -1, 0, 0, 0, +1, +1, +1};

        for (int i = 0; i < 9; i++)
        {
            canvas[int(x + xarr[i]) + width * int(y + yarr[i])] = cell;
        }
    }
    void move()
    {
        x += vel;
    }
    void checkBoundaries()
    {
        if (x <= 2)
        {
            vel = -vel;
            count += 1;
        }
        else if (x >= width - 1.5)
        {
            cell = ' ';
        }
    }
    float bounce(Block other)
    {
        float sumM = this->mass + other.mass;
        float newV = ((this->mass - other.mass) / sumM) * this->vel;
        newV += ((2 * other.mass) / sumM) * other.vel;

        return newV;
    }
    bool collide(Block &other)
    {

        return !(this->x - 2 > other.x + 1 || this->x + 1 < other.x - 2);
    }

    void update()
    {
        x += vel;
    }
};

void fillcanvas(char *canvas)
{
    for (int i = 0; i < width * height; i++)
    {
        canvas[i] = ' ';
    }
}

int main()
{
    int digits;
    cout << "Enter The number of digits of Pi you want : ";
    cin >> digits;

    char *canvas = new char[width * height];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    const float timesteps = pow(10, digits - 1);

    Block b1(30, 17, -1 / timesteps, pow(100, digits - 1));
    Block b2(20, 18, 0, 1);

    float v1, v2;

    while (1)
    {
        for (int i = 0; i < timesteps; i++)
        {
            fillcanvas(canvas);
            b1.drawBlock1(canvas);
            b2.drawBlock2(canvas);
            b1.checkBoundaries();
            b2.checkBoundaries();
            if (b1.collide(b2))
            {
                v1 = b1.bounce(b2);
                v2 = b2.bounce(b1);
                b1.vel = v1;
                b2.vel = v2;
                count += 1;
            }
            b1.update();
            b2.update();

            snprintf(canvas, 40, "Number of Collisions :  %d", count);
            if (digits <= 2) 
            Sleep(10);
            WriteConsoleOutputCharacterA(hConsole, canvas, width * height, {0, 0}, &dwBytesWritten);
        }
    }
}