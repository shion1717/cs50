#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>

int check_digits(string key);
void error_message(void);
void encrypt(string ptext, int key);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (check_digits(argv[1]) > 0)
        {
            error_message();
            return 1;
        }
        else
        {
            int key = atoi(argv[1]);
            printf("Key: %s\n", argv[1]);
            string ptext = get_string("plaintext : ");
            printf("ciphertext: ");
            encrypt(ptext, key);
            printf("\n");
        }
    }
    else
    {
        error_message();
        return 1;
    }
}

int check_digits(string key)
{
    int nonnumeric = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        if (key[i] > 57  || key[i] < 48)
        {
            nonnumeric++;
        }
    }
    return nonnumeric;
}

void encrypt(string ptext, int key)
{
    for (int i = 0; i < strlen(ptext); i++)
    {
        if (ptext[i] >= 65 && ptext[i] <= 90)
        {
            printf("%c", 65 + (ptext[i] - 65 + key) % 26);
        }
        else if (ptext[i] >= 97 && ptext[i] <= 122)
        {
            printf("%c", 97 + (ptext[i] - 97 + key) % 26);
        }
        else
        {
            printf("%c", ptext[i]);
        }
    }
}

void error_message(void)
{
    printf("Usage: ./caesar key\n");
}