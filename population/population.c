#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //declaring variables
    int start;
    int end;
    int n = 0;

    //propmt for start while it's smaller than 9
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);


    //propmt for end until it's larger than start
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);


    //calculate the number of years
    while (start < end)
    {
        start += (start / 3) - (start / 4);
        n++;
    }

    //print number of years
    printf("Years: %i\n", n);
}
