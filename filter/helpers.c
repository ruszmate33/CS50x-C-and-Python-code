#include "helpers.h"
#include "math.h"
#include "stdio.h"
#include <stdbool.h>
#include <stdlib.h>

//deal with possible overflow with sepia function
bool isOverflow(int colorIntensity);
//replace overflow integer with max
int avoidOverflow(int colorIntensity);



// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //go through the pixels of 2D array image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //take the average of the red, green, and blue values in every pixel and assign to BGR
            int averageColor = round(((float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtBlue) / 3);

            //assign the average value to the new pixels RGB
            image[i][j].rgbtRed = averageColor;
            image[i][j].rgbtGreen = averageColor;
            image[i][j].rgbtBlue = averageColor;
        }
    }
    return;
}

//deal with possible overflow with sepia function
bool isOverflow(int colorIntensity)
{
    if (colorIntensity > 255)
    {
        return true;
    }
    return false;
}

//replace overflow integer with max
int avoidOverflow(int colorIntensity)
{
    if (isOverflow(colorIntensity))
    {
        return 255;
    }
    else
    {
        return colorIntensity;
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    //go through the pixels of 2D array image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //take the original red, green, and blue values
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            //caqlculate new values in temporary variables
            int newRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int newGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int newBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            //put this pixels back into image pixel
            image[i][j].rgbtRed = avoidOverflow(newRed);
            image[i][j].rgbtGreen = avoidOverflow(newGreen);
            image[i][j].rgbtBlue = avoidOverflow(newBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{


    //go through each row from top, but read the last to first pixel in each row
    //go through the pixels of 2D array image
    for (int i = 0; i < height; i++)
    {
        //from last to first pixel in each row, but only halfway through (not to reflect twice -> identity)
        for (int j = 0; j < floor(((float) width) / 2); j++)
        {
            RGBTRIPLE firstPixel = image[i][j];
            RGBTRIPLE lastPixel = image[i][width - 1 - j];
            image[i][j] = lastPixel;
            image[i][width - 1 - j] = firstPixel;

        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make a copy image
    RGBTRIPLE copyImage[height][width];

    //for every pixel (not on edge??) calculate averageValue from 9 neighbouring pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //average Red, average Green, average Blue for 9 neighbours of pixel [i,j]
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int numPixels = 0;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    //if neighbouring pixel exists
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        sumRed += image[k][l].rgbtRed;
                        sumGreen += image[k][l].rgbtGreen;
                        sumBlue += image[k][l].rgbtBlue;
                        numPixels++;
                    }
                }
            }
            int avgRed = round(((float) sumRed) / numPixels);
            int avgGreen = round(((float) sumGreen) / numPixels);
            int avgBlue = round(((float) sumBlue) / numPixels);

            //assign new value to copy image`s pixel
            copyImage[i][j].rgbtRed = avgRed;
            copyImage[i][j].rgbtGreen = avgGreen;
            copyImage[i][j].rgbtBlue = avgBlue;
        }
    }
    //copy back from copyImage to output image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copyImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = copyImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copyImage[i][j].rgbtBlue;
        }
    }
    return;
}
