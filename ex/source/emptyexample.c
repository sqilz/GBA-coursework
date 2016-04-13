
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_sprites.h>
#include <gba_input.h>
#include <gba_dma.h>
#include <gba_types.h>


#include "menu.h"
#include "background.h"
#include "background2.h"

#define VID_BUFFER ((u16*)0x0600A000)
#define IMG_PALETTE (BG_PALETTE )
//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
u16 test = 50;
u16 test2 = 30;
int flowerHoff, bgHoff;
int menu(int y);
int game(int a,int b);
enum { SPLASH, GAME };

void PlotPixel(int x,int y, unsigned short int c) 
{
	VID_BUFFER[(y) *120 + (x)] = (c);
}

//extern void PlotPixels();

typedef struct
{
	const u16* tileData;
	const u16* mapData;
	const u16* palData;	
	int tileLength;
	int mapLength;
	int palLength;
} Background;

Background background1 = {backgroundTiles, backgroundMap, backgroundPal, backgroundTilesLen, backgroundMapLen, backgroundPalLen };
Background bg_flowers = {background2Tiles, background2Map, background2Pal, background2TilesLen, background2MapLen, background2PalLen };
void setBG(Background* back, int screenBaseBlock, int charBaseBlock, bool pal);
int main(void) 
{
//---------------------------------------------------------------------------------

	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	irqInit();
	irqEnable(IRQ_VBLANK);
	int c =0;
	// generic setup function
	consoleDemoInit();
int state = 1;
	// main loop
	while (1) 
	{
		VBlankIntrWait();
		scanKeys();
		u16 keys = keysHeld();
		u16 down = keysDown();
		
		switch (state)
		{
			case SPLASH:
				menu(c);
				//	if(down & KEY_UP) c+=10;
				//	if(down & KEY_DOWN) c-=10;
				if(down & KEY_START) state = GAME;
			break;
			
			case GAME:
				game(test,test2);
				if(keys & KEY_UP)	 test-= 1;
				if(keys & KEY_DOWN)		test+=1;
				if(keys & KEY_RIGHT) test2+=1;
				if(keys & KEY_LEFT)  test2-=1;
				if(down & KEY_START) state = SPLASH;
			break;
		}
	}
}
int menu(int c)
{
	u16 down = keysDown();
	int x, y, loop; // variables

	REG_DISPCNT = (1<<4) | MODE_4 | BG2_ENABLE; // switch to mode4
	for(loop = 0; loop < 256; loop++)	// put pal in memory
		IMG_PALETTE[loop] = menuPal[loop];	
		// draw the picture
		for(y = 0; y < 160; y++)
		{
			for(x = 0; x < 120; x++)
			{
				PlotPixel(x,y,menuBitmap[y*120+x]);
			}
		}
		
	if(down & KEY_START) return 1;
	
		return 0;	
}
int game(int a, int b)
{
	
	REG_DISPCNT = MODE_0 | OBJ_1D_MAP | BG0_ENABLE| BG1_ENABLE| OBJ_ENABLE;

	
	//background control registers
	REG_BG0CNT = (3 << 0) | (2 << 2) | BG_256_COLOR | (30 << 8) | BG_SIZE_0;
	REG_BG0HOFS = 0;
	REG_BG1CNT = (2 << 0) | (3 << 2) | BG_256_COLOR | (31 << 8) | BG_SIZE_0;
	REG_BG1HOFS = 0;
	REG_BG2CNT = (1 << 0) | (0 << 2) | BG_16_COLOR | (9 << 8) | BG_SIZE_0;
	REG_BG3CNT = (0 << 0) | (0 << 2) | BG_16_COLOR | (8 << 8) | BG_SIZE_0;
	
	setBG(&background1, 30, 2, 1);
	setBG(&bg_flowers, 31, 3,0);
	bgHoff += 1;
	flowerHoff += 2;
	REG_BG0HOFS = bgHoff/2;
	REG_BG1HOFS = flowerHoff;
	u16* pal = BG_PALETTE;
	//pallette 0
	//pal[0] = RGB5(9,19,26); // transparent default blue, background
	//pal[1] = RGB5(0,31,0);
	//pal[2] = RGB5(0,0,31);
	//pal[3] = RGB5(15,15,15);
	//pallete 1
	pal[16] = RGB5(31,9,9);
	pal[17] = RGB5(24,7,7);
	pal[18] = RGB5(26,9,9);
	pal[19] = RGB5(27,11,11);
	pal[20] = RGB5(28,13,13);
	pal[21] = RGB5(29,14,14);
	pal[22] = RGB5(30,15,15);
	pal[23] = RGB5(12,2,2);
	pal[24] = RGB5(10,10,10);
	pal[25] = RGB5(10,10,10);
	pal[26] = RGB5(10,10,10);
	pal[27] = RGB5(10,10,10);
	pal[28] = RGB5(10,10,10);
	pal[29] = RGB5(10,10,10);
	pal[30] = RGB5(10,10,10);
	pal[31] = RGB5(10,10,10);

	//pallette 2 
	pal[32] = RGB5(10,10,10);
	//and so on up to pallette 16...
	
	u16* obj = SPRITE_PALETTE;
	obj[0] = RGB5(1,1,1);
	obj[1] = RGB5(30,21,2); 	// orange/gold
	obj[2] = RGB5(29,31,2); 	// yellow
	obj[3] = RGB5(31,26,28); 	// light pink
	obj[4] = RGB5(29,5,24);		// pink
	obj[5] = RGB5(14,2,12);		// purple
	obj[6] = RGB5(29,5,11);		// red
	obj[7] = RGB5(5,1,2);		// dark brown
	obj[8] = RGB5(8,1,2);		// beige ??
	obj[9] = RGB5(28,24,16);	// light orange
	obj[10] = RGB5(9,6,1);		// brown
	obj[11] = RGB5(15,14,2);	// olive
	obj[12] = RGB5(31,31,31);	// white
	obj[13] = RGB5(4,4,4);		// dark grey
	obj[14] = RGB5(0,0,0);
	obj[15] = RGB5(31,21,10);
	
	u16* ScreenBG0 = (u16*)0x06005800;// Map Base Adress, can be found in map view
	ScreenBG0[(1 * 32) + 0] = (77 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(1);

	u16* ScreenBG1 = (u16*)0x06005000;// Map Base Adress, can be found in map view
	ScreenBG1[(0 * 32) + 1] = (78 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(1);

	u16* ScreenBG2 = (u16*)0x06004800;	// Map Base Adress, can be found in map view
	ScreenBG2[(0 * 32) + 2] = (79 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(1);

	u16* ScreenBG3 = (u16*)0x06004000;// Map Base Adress, can be found in map view
	ScreenBG3[(0 * 32) + 3] = (1 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(1);
		
	// sprite 0 
	u32* firstSprite = OBJ_BASE_ADR; //sprite memory at tile 0
	for(int c=0; c<8;c++)	firstSprite[c] = 0;		// makes the first sprite blank

	
	// sprite 0 
	u32* sprite = OBJ_BASE_ADR + (0x20*1); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	sprite[0] = (1 << 4) | (1 << 12)| (1 << 20);
	sprite[1] = (1 << 4) | (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20);
	sprite[2] = (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20);
	sprite[3] = (2 << 4) | (2 << 8) | (7 << 12) | (3 << 16) | (7 << 20) | (2<<24);
	sprite[4] = (2 << 0) | (2 << 4) | (2 << 8) | (3 << 12) | (3 << 16) | (2 << 20) | (2<<24);
	sprite[5] = (2 << 0) | (2 << 4) | (2 << 8) | (3 << 12) | (4 << 16) | (2 << 20);
	sprite[6] = (2 << 0) | (2 << 4) | (5 << 8) | (5 << 12) | (5 << 16) | (5 << 20);
	sprite[7] = (3 << 8) | (5 << 12) | (5 << 16) | (3 <<20);
	sprite[8] = (3 << 8) | (5 << 12) | (5 << 16) | (3 <<20);
	sprite[9] = (5 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 <<20) | (5 << 24);
	sprite[10] = (5 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 <<20) | (5 << 24);
	sprite[11] = (5 << 0) | (4 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 <<20) | (4 << 24) | (5 << 28);
	sprite[12] = (4 << 0) | (5 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 <<20) | (5 << 24) | (4 << 28);
	sprite[13] = (4 << 8) | (4 << 20);
	sprite[14] = (4 << 8) | (4 << 20);
	sprite[15] = (4 << 8) | (4 << 12) | (4 << 20) | (4 << 24);
	
	u32* sprite2 = OBJ_BASE_ADR + (0x20*3); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	sprite2[0] = (12 << 8) | (12 << 12) | (12 << 16) | (12 <<20);
	sprite2[1] = (8 << 8) | (9 << 12) | (8 << 16) | (9 << 20);
	sprite2[2] = (9 << 8) | (9 << 12) | (9 << 16) | (9 << 20);
	sprite2[3] = (10 << 8) | (8 << 12) | (8 << 16) | (10 << 20);
	sprite2[4] = (13 << 0) | (1 << 4) | (10 << 8) | (10 << 12) | (10 << 16) | (10 << 20) | (1 << 24) | (13 << 28);
	sprite2[5] = (13 << 0) | (12 << 4) | (1 << 8) | (10 << 12) | (10 << 16) | (1 << 20) | (12 << 24) | (13 << 28);
	sprite2[6] = (13 << 0) | (12 << 4) | (14 << 8) | (1 << 12) | (1 << 16) | (14 << 20) | (12 << 24) | (13 << 28);
	sprite2[7] = (13 << 0) | (12 << 4) | (14 << 8) | (2 << 12) | (2 << 16) | (14 << 20) | (12 << 24) | (13 << 28);
	sprite2[8] = (13 << 0) | (14 << 4) | (14 << 8) | (12 << 12) | (12 << 16) | (14 << 20) | (14 << 24) | (13 << 28);
	sprite2[9] = (9 << 0) | (14 << 4) | (14 << 8) | (12 << 12) | (12 << 16) | (14 << 20) | (14 << 24) | (9 << 28);
	sprite2[10] = (0 << 0) | (13 << 4) | (14 << 8) | (12 << 12) | (12 << 16) | (14 << 20) | (13 << 24) | (0 << 28);
	sprite2[11] = (0 << 0) | (13 << 4) | (14 << 8) | (12 << 12) | (12 << 16) | (14 << 20) | (13 << 24) | (0 << 28);
	sprite2[12] = (0 << 0) | (13 << 4) | (14 << 8) | (12 << 12) | (12 << 16) | (14 << 20) | (13 << 24) | (0 << 28);
	sprite2[13] = (0 << 0) | (13 << 4) | (14 << 8) | (12 << 12) | (12 << 16) | (14 << 20) | (13 << 24) | (0 << 28);
	sprite2[14] = (0 << 0) | (13 << 4) | (11 << 8) | (12 << 12) | (12 << 16) | (11 << 20) | (13 << 24) | (0 << 28);
	sprite2[15] = (0 << 0) | (11 << 4) | (11 << 8) | (0 << 12) | (11 << 16) | (11 << 20) | (0 << 24) | (0 << 28);
	
	
	u16* oam = (u16*)OAM;
	oam[0] = OBJ_Y(a) | OBJ_SHAPE(2);
	oam[1] = OBJ_X(b) | ATTR1_SIZE_8;
	oam[2] = (1<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[3] = 0;
	

		
	
	oam[4] = OBJ_Y(60) | OBJ_SHAPE(2);
	oam[5] = OBJ_X(40) | ATTR1_SIZE_8;
	oam[6] = (3<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[7] = 0;
	
	// -- Base block 0
	u32* first = (u32*)0x06000000;
	first[0] = 0;
	first[1] = 0;
	first[2] = 0;
	first[3] = 0;
	first[4] = 0;
	first[5] = 0;
	first[6] = 0;
	first[7] = 0;

	u32* character = (u32*)0x06000020;
	character[0] = (1 << 12);
	character[1] = (1 << 8) | (1 << 16);
	character[2] = (1 << 12);
	character[3] = (1 << 8) | (1 << 12);
	character[4] = (1 << 4) | (1 << 16);
	character[5] = (1 << 4) | (1 << 20);
	character[6] = (1 << 4) | (1 << 20);
	character[7] = (1 << 8) | (1 << 16);
	
		
		
		return 0;

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


