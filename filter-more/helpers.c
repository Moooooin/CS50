#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Already did that in filter-less
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get average
            double av = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            int average = round(av);

            // Make image gray
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // That too
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Reflect Image
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // And that as well
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avR, avG, avB;

            if (j == 0)
            {
                // Left side
                if (i == 0)
                {

                    // Top left corner
                    avR = round((copy[i][j].rgbtRed + copy[(i + 1)][j].rgbtRed + copy[(i + 1)][(j + 1)].rgbtRed + copy[i][(j + 1)].rgbtRed) / 4.0);
                    avG = round((copy[i][j].rgbtGreen + copy[(i + 1)][j].rgbtGreen + copy[(i + 1)][(j + 1)].rgbtGreen + copy[i][(j + 1)].rgbtGreen) / 4.0);
                    avB = round((copy[i][j].rgbtBlue + copy[(i + 1)][j].rgbtBlue + copy[(i + 1)][(j + 1)].rgbtBlue + copy[i][(j + 1)].rgbtBlue) / 4.0);
                }
                else if (i == (height - 1))
                {
                    // Bottom left corner
                    avR = round((copy[i][j].rgbtRed + copy[(i - 1)][j].rgbtRed + copy[(i - 1)][(j + 1)].rgbtRed + copy[i][(j + 1)].rgbtRed) / 4.0);
                    avG = round((copy[i][j].rgbtGreen + copy[(i - 1)][j].rgbtGreen + copy[(i - 1)][(j + 1)].rgbtGreen + copy[i][(j + 1)].rgbtGreen) / 4.0);
                    avB = round((copy[i][j].rgbtBlue + copy[(i - 1)][j].rgbtBlue + copy[(i - 1)][(j + 1)].rgbtBlue + copy[i][(j + 1)].rgbtBlue) / 4.0);
                }
                else
                {
                    // Everything else on the left side
                    avR = round((copy[i][j].rgbtRed + copy[(i - 1)][j].rgbtRed + copy[(i - 1)][(j + 1)].rgbtRed + copy[i][(j + 1)].rgbtRed + copy[(i + 1)][j].rgbtRed + copy[(i + 1)][(j + 1)].rgbtRed) / 6.0);
                    avG = round((copy[i][j].rgbtGreen + copy[(i - 1)][j].rgbtGreen + copy[(i - 1)][(j + 1)].rgbtGreen + copy[i][(j + 1)].rgbtGreen + copy[(i + 1)][j].rgbtGreen + copy[(i + 1)][(j + 1)].rgbtGreen) / 6.0);
                    avB = round((copy[i][j].rgbtBlue + copy[(i - 1)][j].rgbtBlue + copy[(i - 1)][(j + 1)].rgbtBlue + copy[i][(j + 1)].rgbtBlue + copy[(i + 1)][j].rgbtBlue + copy[(i + 1)][(j + 1)].rgbtBlue) / 6.0);
                }
            }
            else if (j == (width - 1))
            {
                // Right side
                if (i == 0)
                {
                    // Top right corner
                    avR = round((copy[i][j].rgbtRed + copy[(i + 1)][j].rgbtRed + copy[(i + 1)][(j - 1)].rgbtRed + copy[i][(j - 1)].rgbtRed) / 4.0);
                    avG = round((copy[i][j].rgbtGreen + copy[(i + 1)][j].rgbtGreen + copy[(i + 1)][(j - 1)].rgbtGreen + copy[i][(j - 1)].rgbtGreen) / 4.0);
                    avB = round((copy[i][j].rgbtBlue + copy[(i + 1)][j].rgbtBlue + copy[(i + 1)][(j - 1)].rgbtBlue + copy[i][(j - 1)].rgbtBlue) / 4.0);
                }
                else if (i == (height - 1))
                {
                    // Bottom right corner
                    avR = round((copy[i][j].rgbtRed + copy[(i - 1)][j].rgbtRed + copy[(i - 1)][(j - 1)].rgbtRed + copy[i][(j - 1)].rgbtRed) / 4.0);
                    avG = round((copy[i][j].rgbtGreen + copy[(i - 1)][j].rgbtGreen + copy[(i - 1)][(j - 1)].rgbtGreen + copy[i][(j - 1)].rgbtGreen) / 4.0);
                    avB = round((copy[i][j].rgbtBlue + copy[(i - 1)][j].rgbtBlue + copy[(i - 1)][(j - 1)].rgbtBlue + copy[i][(j - 1)].rgbtBlue) / 4.0);
                }
                else
                {
                    // Everything else on the right side
                    avR = round((copy[i][j].rgbtRed + copy[(i - 1)][j].rgbtRed + copy[(i - 1)][(j - 1)].rgbtRed + copy[i][(j - 1)].rgbtRed + copy[(i + 1)][j].rgbtRed + copy[(i + 1)][(j - 1)].rgbtRed) / 6.0);
                    avG = round((copy[i][j].rgbtGreen + copy[(i - 1)][j].rgbtGreen + copy[(i - 1)][(j - 1)].rgbtGreen + copy[i][(j - 1)].rgbtGreen + copy[(i + 1)][j].rgbtGreen + copy[(i + 1)][(j - 1)].rgbtGreen) / 6.0);
                    avB = round((copy[i][j].rgbtBlue + copy[(i - 1)][j].rgbtBlue + copy[(i - 1)][(j - 1)].rgbtBlue + copy[i][(j - 1)].rgbtBlue + copy[(i + 1)][j].rgbtBlue + copy[(i + 1)][(j - 1)].rgbtBlue) / 6.0);
                }
            }
            else if (i == 0)
            {
                // Top
                avR = round((copy[i][j].rgbtRed + copy[i][(j - 1)].rgbtRed + copy[i][(j + 1)].rgbtRed + copy[(i + 1)][j].rgbtRed + copy[(i + 1)][(j - 1)].rgbtRed + copy[(i + 1)][(j + 1)].rgbtRed) / 6.0);
                avG = round((copy[i][j].rgbtGreen + copy[i][(j - 1)].rgbtGreen + copy[i][(j + 1)].rgbtGreen + copy[(i + 1)][j].rgbtGreen + copy[(i + 1)][(j - 1)].rgbtGreen + copy[(i + 1)][(j + 1)].rgbtGreen) / 6.0);
                avB = round((copy[i][j].rgbtBlue + copy[i][(j - 1)].rgbtBlue + copy[i][(j + 1)].rgbtBlue + copy[(i + 1)][j].rgbtBlue + copy[(i + 1)][(j - 1)].rgbtBlue + copy[(i + 1)][(j + 1)].rgbtBlue) / 6.0);
            }
            else if (i == (height - 1))
            {
                // Bottom
                avR = round((copy[i][j].rgbtRed + copy[i][(j - 1)].rgbtRed + copy[i][(j + 1)].rgbtRed + copy[(i - 1)][j].rgbtRed + copy[(i - 1)][(j - 1)].rgbtRed + copy[(i - 1)][(j + 1)].rgbtRed) / 6.0);
                avG = round((copy[i][j].rgbtGreen + copy[i][(j - 1)].rgbtGreen + copy[i][(j + 1)].rgbtGreen + copy[(i - 1)][j].rgbtGreen + copy[(i - 1)][(j - 1)].rgbtGreen + copy[(i - 1)][(j + 1)].rgbtGreen) / 6.0);
                avB = round((copy[i][j].rgbtBlue + copy[i][(j - 1)].rgbtBlue + copy[i][(j + 1)].rgbtBlue + copy[(i - 1)][j].rgbtBlue + copy[(i - 1)][(j - 1)].rgbtBlue + copy[(i - 1)][(j + 1)].rgbtBlue) / 6.0);
            }
            else
            {
                // Every pixel which isn't on the edge
                avR = round((copy[i][j].rgbtRed + copy[i][(j + 1)].rgbtRed + copy[i][(j - 1)].rgbtRed + copy[(i + 1)][j].rgbtRed + copy[(i - 1)][j].rgbtRed + copy[(i + 1)][(j + 1)].rgbtRed + copy[(i + 1)][(j - 1)].rgbtRed + copy[(i - 1)][(j + 1)].rgbtRed + copy[(i - 1)][(j - 1)].rgbtRed) / 9.0);
                avG = round((copy[i][j].rgbtGreen + copy[i][(j + 1)].rgbtGreen + copy[i][(j - 1)].rgbtGreen + copy[(i + 1)][j].rgbtGreen + copy[(i - 1)][j].rgbtGreen + copy[(i + 1)][(j + 1)].rgbtGreen + copy[(i + 1)][(j - 1)].rgbtGreen + copy[(i - 1)][(j + 1)].rgbtGreen + copy[(i - 1)][(j - 1)].rgbtGreen) / 9.0);
                avB = round((copy[i][j].rgbtBlue + copy[i][(j + 1)].rgbtBlue + copy[i][(j - 1)].rgbtBlue + copy[(i + 1)][j].rgbtBlue + copy[(i - 1)][j].rgbtBlue + copy[(i + 1)][(j + 1)].rgbtBlue + copy[(i + 1)][(j - 1)].rgbtBlue + copy[(i - 1)][(j + 1)].rgbtBlue + copy[(i - 1)][(j - 1)].rgbtBlue) / 9.0);
            }

            // Blur image
            image[i][j].rgbtRed = avR;
            image[i][j].rgbtGreen = avG;
            image[i][j].rgbtBlue = avB;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Make copy of image
    RGBTRIPLE copy[height][width];

    // Copy each pixel in image into copy array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Adjust calculations if pixel is on edge
            double GxRed, GyRed, GxGreen, GyGreen, GxBlue, GyBlue;

            RGBTRIPLE tl = copy[(i - 1)][(j - 1)];
            RGBTRIPLE tc = copy[(i - 1)][j];
            RGBTRIPLE tr = copy[(i - 1)][(j + 1)];
            RGBTRIPLE cl = copy[i][(j - 1)];
            RGBTRIPLE cr = copy[i][(j + 1)];
            RGBTRIPLE bl = copy[(i + 1)][(j - 1)];
            RGBTRIPLE bc = copy[(i + 1)][j];
            RGBTRIPLE br = copy[(i + 1)][(j + 1)];

            // t : top, c : center, b : bottom, r : right, l : left

            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                if (i == 0)
                {
                    tl.rgbtRed = 0;
                    tl.rgbtGreen = 0;
                    tl.rgbtBlue = 0;

                    tc.rgbtRed = 0;
                    tc.rgbtGreen = 0;
                    tc.rgbtBlue = 0;

                    tr.rgbtRed = 0;
                    tr.rgbtGreen = 0;
                    tr.rgbtBlue = 0;
                }

                if (j == 0)
                {
                    tl.rgbtRed = 0;
                    tl.rgbtGreen = 0;
                    tl.rgbtBlue = 0;

                    cl.rgbtRed = 0;
                    cl.rgbtGreen = 0;
                    cl.rgbtBlue = 0;

                    bl.rgbtRed = 0;
                    bl.rgbtGreen = 0;
                    bl.rgbtBlue = 0;
                }

                if (i == height - 1)
                {
                    bl.rgbtRed = 0;
                    bl.rgbtGreen = 0;
                    bl.rgbtBlue = 0;

                    bc.rgbtRed = 0;
                    bc.rgbtGreen = 0;
                    bc.rgbtBlue = 0;

                    br.rgbtRed = 0;
                    br.rgbtGreen = 0;
                    br.rgbtBlue = 0;
                }

                if (j == width - 1)
                {
                    tr.rgbtRed = 0;
                    tr.rgbtGreen = 0;
                    tr.rgbtBlue = 0;

                    cr.rgbtRed = 0;
                    cr.rgbtGreen = 0;
                    cr.rgbtBlue = 0;

                    br.rgbtRed = 0;
                    br.rgbtGreen = 0;
                    br.rgbtBlue = 0;
                }
            }
            else
            {

            }

            // Calculate Gx and Gy for each color value
            GxRed = (tl.rgbtRed * -1.0) + (cl.rgbtRed * -2.0) + (bl.rgbtRed * -1.0) + tr.rgbtRed + (cr.rgbtRed * 2.0) + br.rgbtRed;
            GyRed = (tl.rgbtRed * -1.0) + (tc.rgbtRed * -2.0) + (tr.rgbtRed * -1.0) + bl.rgbtRed + (bc.rgbtRed * 2.0) + br.rgbtRed;

            GxGreen = (tl.rgbtGreen * -1.0) + (cl.rgbtGreen * -2.0) + (bl.rgbtGreen * -1.0) + tr.rgbtGreen + (cr.rgbtGreen * 2.0) + br.rgbtGreen;
            GyGreen = (tl.rgbtGreen * -1.0) + (tc.rgbtGreen * -2.0) + (tr.rgbtGreen * -1.0) + bl.rgbtGreen + (bc.rgbtGreen * 2.0) + br.rgbtGreen;

            GxBlue = (tl.rgbtBlue * -1.0) + (cl.rgbtBlue * -2.0) + (bl.rgbtBlue * -1.0) + tr.rgbtBlue + (cr.rgbtBlue * 2.0) + br.rgbtBlue;
            GyBlue = (tl.rgbtBlue * -1.0) + (tc.rgbtBlue * -2.0) + (tr.rgbtBlue * -1.0) + bl.rgbtBlue + (bc.rgbtBlue * 2.0) + br.rgbtBlue;

            // Calculate new color values and make sure new color stays in range
            int GRed = round(sqrt((GxRed * GxRed) + (GyRed * GyRed)));

            int GGreen = round(sqrt((GxGreen * GxGreen) + (GyGreen * GyGreen)));

            int GBlue = round(sqrt((GxBlue * GxBlue) + (GyBlue * GyBlue)));

            if (GRed > 255)
            {
                GRed = 255;
            }

            if (GGreen > 255)
            {
                GGreen = 255;
            }

            if (GBlue > 255)
            {
                GBlue = 255;
            }

            // Change color to new value
            image[i][j].rgbtRed = GRed;
            image[i][j].rgbtGreen = GGreen;
            image[i][j].rgbtBlue = GBlue;
        }
    }
    return;
}