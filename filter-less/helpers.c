#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get sepia colors
            int sRed = round((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue));
            int sGreen = round((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue));
            int sBlue = round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));

            // Make sure to stay in range
            if (sRed > 255)
            {
                sRed = 255;
            }

            if (sGreen > 255)
            {
                sGreen = 255;
            }

            if (sBlue > 255)
            {
                sBlue = 255;
            }

            // Make image sepia
            image[i][j].rgbtRed = sRed;
            image[i][j].rgbtGreen = sGreen;
            image[i][j].rgbtBlue = sBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
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