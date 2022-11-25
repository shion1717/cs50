#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

int check_firstdigit(long long number);
int check_first2digits_amex(long long number);
int check_first2digits_mc(long long number);
int get_length(long long number);
int valid_number(long long number);

//Main function that does all the checks
int main(void)
{
    long long number = get_long_long("Number :");
    if (valid_number(number))
    {
        if (get_length(number) == 15)
        {
            if (check_first2digits_amex(number))
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (get_length(number) == 16)
        {
            if (check_first2digits_mc(number))
            {
                printf("MASTERCARD\n");
            }
            else if (check_firstdigit(number))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (get_length(number) == 13)
        {
            if (check_firstdigit(number))
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    //printf("Is valid? : %d\n", valid_number(number));
    //printf("Is VISA? : %d\n",check_firstdigit(number));
    //printf("Is AMEX? : %d\n",check_first2digits_amex(number));
    //printf("Is Mastercard? : %d\n",check_first2digits_mc(number));
    //printf("Length of number: %d\n", get_length(number));
}

//Checks if the first digit is 4
int check_firstdigit(long long number)
{
    while (number >= 10)
    {
        number = number / 10;
    }
    return (number == 4);
}

//Checks if the first two digits are 34, 37
int check_first2digits_amex(long long number)
{
    while (number >= 100)
    {
        number = number / 10;
    }
    return (number == 34 || number == 37);
}

//Checks if the first two digits are 51,52,53,54 or 55
int check_first2digits_mc(long long number)
{
    while (number >= 100)
    {
        number = number / 10;
    }
    return (number == 51 || number == 52 || number == 53 || number == 54 || number == 55);
}


//Function that returns the length of the number
int get_length(long long number)
{
    return floor(log10(number)) + 1;
}

//Function that checks if the number is valid as per Luhn's Algorithm
int valid_number(long long number)
{
    int i = 0;
    int evens = 0;
    int odds = 0;
    while (number != 0)
    {
        if (i % 2 == 0)
        {
            odds = odds + (number % 10);
        }
        else
        {
            int evendigit = (number % 10) * 2;
            while (evendigit != 0)
            {
                evens = evens + (evendigit % 10);
                evendigit /= 10;
            }
        }
        number /= 10;
        i++;
    }
    return (odds + evens) % 10 == 0;
}