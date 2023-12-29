#include <cs50.h>
#include <stdio.h>

int getHeight(void);
void makePyramid(int n);

int main(void)
{
    //get the height
    int height = getHeight();

    //print the pyramid
    makePyramid(height);
}


void makePyramid(int n)
{
    for (int i = 1; i <= n; i++)
    {
        //print spaces
        for (int j = 0; j < n - i; j++)
        {
            printf(" ");
        }

        //print hashes
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        //new line
        printf("\n");
    }
}

int getHeight(void)
{
    //prompt for height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n > 8 || n <= 0);

    return n;
}