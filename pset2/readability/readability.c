#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

int get_letters(string text);
int get_words(string text);
int get_sentences(string text);
void get_grade(float grade);

int main(void)
{
    string text = get_string("Text: ");
    float lettercount = get_letters(text);
    float wordcount = get_words(text);
    float sencount = get_sentences(text);
    float grade = ((0.0588 * lettercount) - (0.296 * sencount)) / (wordcount / 100) - 15.8;
    printf("%f\n", grade);
    get_grade(grade);
}


int get_letters(string text)
{
    int lettercount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] >= 97 && text[i] <= 122)
        {
            lettercount++;
        }
        else if (text[i] >= 65 && text[i] <= 90)
        {
            lettercount++;
        }
    }
    return lettercount;
}

int get_words(string text)
{
    int wordcount = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == 32)
        {
            wordcount++;
        }
    }
    return wordcount;
}

int get_sentences(string text)
{
    int sencount = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sencount++;
        }
    }
    return sencount;
}

void get_grade(float grade)
{
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", grade);
    }
}