#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_sprites.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "background.h"
#include "background2.h"
#include "drawing.h"

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
u16 test = 50;
u16 test2 = 30;
int flowerHoff, bgHoff;
int menu(int y);
int game(int a,int b, int c, int d);
enum { SPLASH, GAME };

Background background1 = {backgroundTiles, backgroundMap, backgroundPal, backgroundTilesLen, backgroundMapLen, backgroundPalLen };
Background bg_flowers = {background2Tiles, background2Map, background2Pal, background2TilesLen, background2MapLen, background2PalLen };

typedef struct
{
	int x;
	int y;
}Enemy;
typedef struct
{
	int x;
	int y;
}Character;

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
	Enemy enemy = { enemy.x = 90, enemy.y = 40 };
	Character character = { character.x = 10, character.y = 50};
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
				game(character.x,character.y, enemy.x, enemy.y); 
				//character and enemy movement updating, and sending to 
				if(keys & KEY_UP)	 character.y-= 1;
				if(keys & KEY_DOWN)	 character.y+=1;
				if(down & KEY_START) state = SPLASH;
				
				enemy.x -= 1;
				
			break;
		}
	}
}
int menu(int c)
{
	u16 down = keysDown();
	int x, y, loop; // variables

	// switch to mode4
	REG_DISPCNT = (1<<4) | MODE_4 | BG2_ENABLE; 
	// put palette in memory
	for(loop = 0; loop < 256; loop++)	
		IMG_PALETTE[loop] = menuPal[loop];	
		// draws the menu bitmap
		for(y = 0; y < 160; y++)
		{
			for(x = 0; x < 120; x++)
			{
				PlotPixel(x,y,menuBitmap[y*120+x]);
			}
		}
	// moves from menu state to game once pressed
	if(down & KEY_START) return 1;
	
		return 0;	
}
int game(int characterX, int characterY, int enemyX, int enemyY)
{
	// display control register
	REG_DISPCNT = MODE_0 | OBJ_1D_MAP | BG_ALL_ENABLE | OBJ_ENABLE;
	//background control registers
	REG_BG0CNT = (3 << 0) | (2 << 2) | BG_256_COLOR | (30 << 8) | BG_SIZE_0;
	REG_BG0HOFS = 0;
	REG_BG1CNT = (2 << 0) | (3 << 2) | BG_256_COLOR | (31 << 8) | BG_SIZE_0;
	REG_BG1HOFS = 0;
	REG_BG2CNT = (1 << 0) | (0 << 2) | BG_16_COLOR | (9 << 8) | BG_SIZE_0;
	REG_BG3CNT = (0 << 0) | (0<< 2) | BG_16_COLOR |(8 << 8) | BG_SIZE_0;
	// setting and moving background and flowers layer
	setBG(&background1, 30, 2, 1);
	setBG(&bg_flowers, 31, 3,0);
	bgHoff += 1;
	flowerHoff += 2;
	REG_BG0HOFS = bgHoff/2;
	REG_BG1HOFS = flowerHoff;

	u16* pal = BG_PALETTE;
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
	// -- Base block 0.
	//first character tile. for clearing screen i think, without this squares show on screen
	u32* first = (u32*)0x06000000;
	for (int c = 0; c<8; c++) first[c] = 0;

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
	obj[16] = RGB5(3, 7, 8);
	obj[17] = RGB5(3, 7, 8);
	obj[18] = RGB5(2, 14, 9);
	obj[19] = RGB5(0, 24, 7);

	char str[] = "";
	sprintf(str,"%d", enemyX);
	drawString(90, 1, str);
	drawString(84, 1, "Score ");
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
	
	
	u32* sprite3 = OBJ_BASE_ADR + (0x20 * 5); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	// tile 1
	sprite3[0] = (1 << 12) | (1 << 16);
	sprite3[1] = (1 << 8) | (1 << 12) | (1 << 16);
	sprite3[2] = (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20) | (1 << 24);
	sprite3[3] = (1 << 4) | (1 << 8) | (2 << 12) | (2 << 16) | (1 << 20) | (1 << 24) | (1 << 28);
	sprite3[4] = (1 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (1 << 24) | (1 << 28);
	sprite3[5] = (1 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (3 << 16) | (2 << 20) | (2 << 24) | (2 << 28);
	sprite3[6] = (1 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (3 << 16) | (2 << 20) | (2 << 24) | (2 << 28);
	sprite3[7] = (1 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (2 << 28);
	// tile 2
	sprite3[8] = (1 << 12) | (1 << 16);
	sprite3[9] = (1 << 12) | (1 << 16) | (1 << 20);
	sprite3[10] = (1 << 4) | (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20);
	sprite3[11] = (1 << 0) | (1 << 4) | (1 << 8) | (2 << 12) | (2 << 16) | (1 << 20) | (1 << 24);
	sprite3[12] = (1 << 0) | (1 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (1 << 28);
	sprite3[13] = (2 << 0) | (2 << 4) | (2 << 8) | (3 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (1 << 28);
	sprite3[14] = (2 << 0) | (2 << 4) | (2 << 8) | (3 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (1 << 28);
	sprite3[15] = (2 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (1 << 28);
	// tile 3
	sprite3[16] = (1 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (2 << 28);
	sprite3[17] = (1 << 4) | (2 << 8) | (2 << 12) | (3 << 16) | (3 << 20) | (3 << 24) | (3 << 28);
	sprite3[18] = (1 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (3 << 28);
	sprite3[19] = (1 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (2 << 28);
	sprite3[20] = (1 << 12) | (2 << 16) | (2 << 20) | (2 << 24) | (2 << 28);
	sprite3[21] = (1 << 16) | (1 << 20) | (2 << 24) | (2 << 28);
	sprite3[22] = (1 << 24) | (2 << 28);
	sprite3[23] = (1 << 28);
	// tile 4
	sprite3[24] = (2 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20) | (1 << 24);
	sprite3[25] = (3 << 0) | (3 << 4) | (3 << 8) | (3 << 12) | (2 << 16) | (2 << 20) | (1 << 24);
	sprite3[26] = (3 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (1 << 20);
	sprite3[27] = (2 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (1 << 20);
	sprite3[28] = (2 << 0) | (2 << 4) | (2 << 8) | (2 << 12) | (1 << 16);
	sprite3[29] = (2 << 0) | (2 << 4) | (1 << 8) | (1 << 12);
	sprite3[30] = (2 << 0) | (1 << 4);
	sprite3[31] = (1 << 0);


	u16* oam = (u16*)OAM;
	oam[0] = OBJ_Y(characterY) | OBJ_SHAPE(2);
	oam[1] = OBJ_X(characterX) | ATTR1_SIZE_8;
	oam[2] = (1<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[3] = 0;	

	oam[4] = OBJ_Y(60) | OBJ_SHAPE(2);
	oam[5] = OBJ_X(40) | ATTR1_SIZE_8;
	oam[6] = (3<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[7] = 0;

	oam[8] = OBJ_Y(enemyY) | OBJ_SHAPE(0);
	oam[9] = OBJ_X(enemyX) | ATTR1_SIZE_16;
	oam[10] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[11] = 0;

	oam[12] = OBJ_Y(enemyY+20) | OBJ_SHAPE(0);
	oam[13] = OBJ_X(enemyX+20) | ATTR1_SIZE_16;
	oam[14] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[15] = 0;

	oam[16] = OBJ_Y(enemyY + 130) | OBJ_SHAPE(0);
	oam[17] = OBJ_X(enemyX + 30) | ATTR1_SIZE_16;
	oam[18] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[19] = 0;

	oam[20] = OBJ_Y(enemyY + 240) | OBJ_SHAPE(0);
	oam[21] = OBJ_X(enemyX + 40) | ATTR1_SIZE_16;
	oam[22] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[23] = 0;

	oam[24] = OBJ_Y(enemyY + 350) | OBJ_SHAPE(0);
	oam[25] = OBJ_X(enemyX + 50) | ATTR1_SIZE_16;
	oam[26] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[27] = 0;

	oam[28] = OBJ_Y(enemyY +460) | OBJ_SHAPE(0);
	oam[29] = OBJ_X(enemyX + 70) | ATTR1_SIZE_16;
	oam[30] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[31] = 0;
	
	return 0;
}




