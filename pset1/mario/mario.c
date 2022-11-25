#include <stdio.h>
#include <cs50.h>

int get_positive_int(void);
void print_spaces(int n);
void print_hashes(int);


//Main program to print a tower of spaces and hashes
int main(void)
{
    int height = get_positive_int();
    for (int i = 1; i <= height; i++)
    {
        print_spaces(height - i);
        print_hashes(i);
        print_spaces(2);
        print_hashes(i);
        printf("\n");
    }
}


// Prompt user for an integer between 1 and 8 (inclusive)
int get_positive_int(void)
{
    int n;
    do
    {
        n = get_int("Enter height (positive integers only): ");
    }
    while (n < 1 || n > 8);
    return n;
}

//Function to print x number of spaces
void print_spaces(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}

//Function to print x number of hashes
void print_hashes(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}