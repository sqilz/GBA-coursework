#include <gba_types.h>
#include <gba_dma.h>
#include <gba_video.h>
#include <string.h>
#define VID_BUFFER ((u16*)0x0600A000)
#define TXT_BUFFER ((u16*)0x06000000)
#define IMG_PALETTE (BG_PALETTE )
#define OFFSET(r,c,numcols) ((r)*(numcols)+(c))

typedef struct
{
	const u16* tileData;
	const u16* mapData;
	const u16* palData;
	int tileLength;
	int mapLength;
	int palLength;
} Background;

void PlotPixel(int row, int col, u16 colour);
void setBG(Background* back, int screenBaseBlock, int charBaseBlock, bool pal);
void drawBitmap(const u16 *bitmapData, const u16 * paletteData);
void drawString(int x, int y, char str[]);

