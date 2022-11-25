#include "helpers.h"
#include <math.h>
#include <stdlib.h>

int min(int x, int y);
int max(int x, int y);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = (image[i][j].rgbtRed +
                             image[i][j].rgbtGreen +
                             image[i][j].rgbtBlue) / 3.0;
            average = round(average);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = round((image[i][j].rgbtRed * 0.393 +
                               image[i][j].rgbtGreen * 0.769 +
                               image[i][j].rgbtBlue * 0.189));
            float green = round((image[i][j].rgbtRed * 0.349 +
                                 image[i][j].rgbtGreen * 0.686 +
                                 image[i][j].rgbtBlue * 0.168));
            float blue = round((image[i][j].rgbtRed * 0.272 +
                                image[i][j].rgbtGreen * 0.534 +
                                image[i][j].rgbtBlue * 0.131));

            image[i][j].rgbtRed = min(red, 255);
            image[i][j].rgbtGreen = min(green, 255);
            image[i][j].rgbtBlue = min(blue, 255);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int red[width];
        int green[width];
        int blue[width];

        for (int j = 0; j < width; j++)
        {
            red[j] = image[i][width - j - 1].rgbtRed;
            green[j] = image[i][width - j - 1].rgbtGreen;
            blue[j] = image[i][width - j - 1].rgbtBlue;
        }
        for (int k = 0; k < width; k++)
        {
            image[i][k].rgbtRed = red[k];
            image[i][k].rgbtGreen = green[k];
            image[i][k].rgbtBlue = blue[k];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Set up temporary container
    int red[height][width];
    int green[height][width];
    int blue[height][width];
    //Loop through height of image
    for (int i = 0; i < height; i++)
    {
        //Loop through width of image
        for (int j = 0; j < width; j++)
        {
            int pixred = 0;
            int pixgreen = 0;
            int pixblue = 0;
            int counter = 0;
            //Loop through surrounding pixels to get average
            //Y axis
            for (int y = max(min(i - 1, height), 0); y <= max(min(i + 1, height - 1), 0); y++)
            {
                //X axis
                for (int x = max(min(j - 1, width), 0); x <= max(min(j + 1, width - 1), 0); x++)
                {
                    pixred += image[y][x].rgbtRed;
                    pixgreen += image[y][x].rgbtGreen;
                    pixblue += image[y][x].rgbtBlue;
                    counter++;
                }
            }
            //Get the average of surrounding pixels
            int avgr = round(pixred / (1.0 * counter));
            int avgg = round(pixgreen / (1.0 * counter));
            int avgb = round(pixblue / (1.0 * counter));

            //Set the temp container to the average of surrounding pixels
            red[i][j] = avgr;
            green[i][j] = avgg;
            blue[i][j] = avgb;
        }
    }
    //Loop through the image to make it equal to the temporary container
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = red[i][j];
            image[i][j].rgbtGreen = green[i][j];
            image[i][j].rgbtBlue = blue[i][j];
        }
    }
    return;
}

//Function to select the smaller value of 2 integers
int min(int x, int y)
{
    if (x < y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

//Function to select the larger value of 2 integers
int max(int x, int y)
{
    if (x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}