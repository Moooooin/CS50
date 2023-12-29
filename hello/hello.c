#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //prompt for name and greet
    string name = get_string("What is your name? ");
    printf("hello, %s\n", name);
}