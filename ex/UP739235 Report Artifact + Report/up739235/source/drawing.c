#include "drawing.h"
#include "background.h"
#include "background2.h"

void PlotPixel(int row, int col, u16 colour)
{
	VID_BUFFER[(col)* 120 + (row)] = (colour);
}

void setBG(Background* back, int screenBaseBlock, int charBaseBlock, bool pal) {
	// tile image storing
	dmaCopy(back->tileData, (u16*)CHAR_BASE_BLOCK(charBaseBlock), back->tileLength);

	// map storing
	dmaCopy(back->mapData, (u16*)SCREEN_BASE_BLOCK(screenBaseBlock), back->mapLength);

	if (pal) {
		// palette storing
		dmaCopy(back->palData, BG_PALETTE, back->palLength);
	}
}

void drawBitmap(const u16 *bitmapData, const u16 * paletteData)
{
	int x, y, loop;
	// put palette in memory
	for (loop = 0; loop < 256; loop++)
		IMG_PALETTE[loop] = paletteData[loop];

	// draws the menu bitmap
	for (y = 0; y < 160; y++)
	{
		for (x = 0; x < 120; x++)
		{
			PlotPixel(x, y, bitmapData[y * 120 + x]);
		}
	}
}

void drawString(int x, int y, char str[])
{
	//BG3 layer adress
	u16* ScreenBG3 = (u16*)0x06004000;

	int len = strlen(str);
	int loop;
	for (loop = 0; loop <= len; loop++)
	{
		ScreenBG3[(y * 32) + (x+loop)] = (str[loop] << 0) | (0 << 10) | (0 << 11) | CHAR_PALETTE(1);
	}
}