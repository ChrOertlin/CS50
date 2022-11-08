#include "helpers.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Average Method for Grayscale calculation
            int grayscale = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / (float) 3);
            image[i][j].rgbtBlue = grayscale ;
            image[i][j].rgbtRed = grayscale;
            image[i][j].rgbtGreen = grayscale;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // revert each pixel row
    for (int h = 0; h < height; h++){
        // initialize a single row
        RGBTRIPLE revRow[1][width];
        for (int w = 0; w < width; w++)
            {
            // store values backwards
            revRow[0][width - 1 - w].rgbtRed = image[h][w].rgbtRed;
            revRow[0][width - 1 - w].rgbtGreen = image[h][w].rgbtGreen;
            revRow[0][width - 1 - w].rgbtBlue = image[h][w].rgbtBlue;

            }
        // assign the backwards values to the image
         for (int r = 0; r < width; r++){
            image[h][r] = revRow[0][r];
            }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy of image to retain original values
    RGBTRIPLE imageCopy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            imageCopy[h][w] = image[h][w];
        }
    }



    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
        float redsSum = 0;
        float bluesSum = 0;
        float greensSum = 0;
        int pixels = 0;
        // loop through positions before and next of the pixel
        for (int ar = -1 ; ar < 2; ar++)
        {
            for (int ac = -1; ac < 2; ac++)
            {

                // does the ar (adjacent row fall below 0 or above height or is height)
                if (h + ar < 0 || h + ar >= height)
                {
                    continue;
                }
                // does the a (adjacent column fall below 0 or above width is width)
                 if (w + ac < 0 || w + ac >= width)
                {
                    continue;
                }
                // if falls in range of the picture add the RGB values up
                redsSum += imageCopy[h + ar][w + ac].rgbtRed;
                bluesSum += imageCopy[h + ar][w + ac].rgbtBlue;
                greensSum += imageCopy[h + ar][w + ac].rgbtGreen;
                // memorize how many pixel are taken
                pixels++;
            }
        }

        image[h][w].rgbtRed = round(redsSum / (float) pixels);
        image[h][w].rgbtBlue = round(bluesSum / (float) pixels);
        image[h][w].rgbtGreen = round(greensSum / (float) pixels);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy of image to retain original values
    RGBTRIPLE imageCopy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            imageCopy[h][w] = image[h][w];
        }
    }

    // vertical matrix
    int gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    // horizontal matrix
    int gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0 ,1}
    };


     for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int gyRed = 0;
            int gyGreen = 0;
            int gyBlue = 0;
            int gxRed = 0;
            int gxGreen = 0;
            int gxBlue = 0;
        // loop through positions before and next of the pixel
        for (int ar = -1 ; ar < 2; ar++)
        {
            for (int ac = -1; ac < 2; ac++)
            {
                // does the ar (adjacent row fall below 0 or above height or is height)
                if (h + ar < 0 || h + ar > height - 1)
                {
                    // do nothing; i.e. treat all pixels as black (0,0,0)
                    continue;
                }
                // does the a (adjacent column fall below 0 or above width is width)
              else if (w + ac < 0 || w + ac > width - 1)
                {
                    continue;
                }
                else
                {
                    // gy calculation
                    gyRed += imageCopy[h + ar][w + ac].rgbtRed * gy[ar + 1][ac + 1];
                    gyBlue += imageCopy[h + ar][w + ac].rgbtBlue * gy[ar + 1][ac + 1];
                    gyGreen += imageCopy[h + ar][w + ac].rgbtGreen * gy[ar + 1][ac + 1];
                    // gx calculation
                    gxRed += imageCopy[h + ar][w + ac].rgbtRed * gx[ar + 1][ac + 1];
                    gxBlue += imageCopy[h + ar][w + ac].rgbtBlue * gx[ar + 1][ac + 1];
                    gxGreen += imageCopy[h + ar][w + ac].rgbtGreen * gx[ar + 1][ac + 1];
                }
                //

            }

        }
        // adjust image gx^2 + gy ^ 2
        image[h][w].rgbtRed = fmin(round(sqrt((gyRed * gyRed) + (float)(gxRed * gxRed))), 255);
        image[h][w].rgbtBlue = fmin(round(sqrt((gyBlue * gyBlue) + (float)(gxBlue * gxBlue))), 255);
        image[h][w].rgbtGreen = fmin(round(sqrt((gyGreen * gyGreen) + (float)(gxGreen * gxGreen))), 255);
        }
    }
    return;
}
