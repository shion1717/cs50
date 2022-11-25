#include <stdio.h>
#include <cs50.h>

//Main program. Asks a user for the name, and returns a greeting to the user.
int main(void)
{
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}