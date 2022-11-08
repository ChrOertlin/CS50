#include <cs50.h>
#include <stdio.h>
void drawNOf(char toDraw, int times);
int main(void)
{
    // ask user for height until inpuit is right
    bool correctInput = false;
    int height = 0;
    while (correctInput == false)
    {
        height = get_int("Specify a height as number between 1-8:\t");
        if (height >= 1 && height <= 8)
        {
            correctInput = true;
        }
    }
    // do the drawing
    // height is a row indicator
    // for each wanted row (i.e. height)
    for (int i = 1; i <= height; i++)
    {
        // generate offset from left side
        int offset = height - i;
        drawNOf(' ', offset);
        // print format n hash 2 space n hash
        drawNOf('#', i);
        drawNOf(' ', 2);
        drawNOf('#', i);
        //jump line
        printf("\n");
    }
}

void drawNOf(char toDraw, int times)\
{
    // print a specific character n times
    for (int i = 0; i != times; i++)
    {
        printf("%c", toDraw);
    }
}