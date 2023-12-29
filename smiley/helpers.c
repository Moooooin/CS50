#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (image[j][i].rgbtBlue == 0x00 && image[j][i].rgbtRed == 0x00 && image[j][i].rgbtGreen == 0x00)
            {
                // Germany flag
                if (j < 3)
                {
                    // Black
                    image[j][i].rgbtRed = 0x08;
                    image[j][i].rgbtGreen = 0x08;
                    image[j][i].rgbtBlue = 0x08;
                }
                else if (j > 2 && j < 5)
                {
                    // Red
                    image[j][i].rgbtRed = 0xdb;
                    image[j][i].rgbtGreen = 0x14;
                    image[j][i].rgbtBlue = 0x14;
                }
                else
                {
                    // Yellow
                    image[j][i].rgbtRed = 0xdb;
                    image[j][i].rgbtGreen = 0xba;
                    image[j][i].rgbtBlue = 0x14;
                }
            }
        }
    }
}
