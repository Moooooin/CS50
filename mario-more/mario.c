#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;

    //prompt for height
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);


    //draw pyramids
    for (int i = 1; i <= h; i++)
    {
        //determine number of spaces
        int j = h - i;

        //print spaces
        while (j > 0)
        {
            printf(" ");
            j--;
        }

        //print hashes 1
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }

        //print two spaces
        printf("  ");

        //print hashes 2
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }

        //new line
        printf("\n");
    }
}