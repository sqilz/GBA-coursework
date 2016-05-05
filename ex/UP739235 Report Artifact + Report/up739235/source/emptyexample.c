#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_sprites.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fade.h>
#include "menu.h"
#include "lost.h"
#include "background.h"
#include "background2.h"
#include "drawing.h"

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------

int flowerHoff, bgHoff;
int menu();
int lost();
void collide(int characterX, int characterY, int enemyX, int enemyY, int enemy2X, int enemy2Y);
// this is bad
int game(int characterx, int charactery,
	int enemyx,  int enemyy, 
	int enemy2x, int enemy2y, 
	int enemy3x, int enemy3y, 
	int enemy4x, int enemy4y, 
	int enemy5x, int enemy5y, 
	int enemy6x, int enemy6y, 
	// bound boxes
	int enemyx2, int enemyy2, 
	int enemyx3, int enemyy3,

	int enemy2x2, int enemy2y2,
	int enemy2x3, int enemy2y3,

	int enemy3x2, int enemy3y2,
	int enemy3x3, int enemy3y3, 

	int enemy4x2, int enemy4y2,
	int enemy4x3, int enemy4y3, 

	int enemy5x2, int enemy5y2,
	int enemy5x3, int enemy5y3, 

	int enemy6x2, int enemy6y2,
	int enemy6x3, int enemy6y3
	);

void spritesAndPalettes();
extern void Score(int *score,  int * scoreMultiplier, int * output);
extern void palettes();
int score = 1;
int multiplier =10;
enum { SPLASH, GAME, LOST };

Background background1 = {backgroundTiles, backgroundMap, backgroundPal, backgroundTilesLen, backgroundMapLen, backgroundPalLen };
Background bg_flowers = {background2Tiles, background2Map, background2Pal, background2TilesLen, background2MapLen, background2PalLen };

typedef struct
{
	int x;
	int y;
	int x2;
	int y2;
	int x3;
	int y3;
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

	// generic setup function
	consoleDemoInit();
	

	
	
	int state = 0;		
	srand(time(NULL));
	
	Character character = { character.x = 10, character.y = 50 };
	Enemy enemy =  {enemy.x = 250, enemy.y = 0};
	Enemy enemy2 = {enemy2.x = 250, enemy2.y = 40};
	Enemy enemy3 = {enemy3.x = 350, enemy3.y = 60};
	Enemy enemy4 = {enemy4.x = 250, enemy4.y = 80};
	Enemy enemy5 = {enemy5.x = 350, enemy5.y = 100};
	Enemy enemy6 = {enemy6.x = 250, enemy6.y = 120};
	
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
					menu();
					if (down & KEY_START)
						state = GAME;
				
				break;
				
			case GAME:
				game(character.x, character.y, enemy.x, enemy.y, enemy2.x, enemy2.y, enemy3.x, enemy3.y, enemy4.x, enemy4.y, enemy5.x, enemy5.y, enemy6.x, enemy6.y,
					enemy.x2, enemy.y2, enemy.x3, enemy.y3,
					enemy2.x2, enemy2.y2, enemy2.x3, enemy2.y3,
					enemy3.x2, enemy3.y2, enemy3.x3, enemy3.y3,
					enemy4.x2, enemy4.y2, enemy4.x3, enemy4.y3,
					enemy5.x2, enemy5.y2, enemy5.x3, enemy5.y3,
					enemy6.x2, enemy6.y2, enemy6.x3, enemy6.y3);
					//character and enemy movement updating, and sending to 
					if(keys & KEY_UP)	 character.y-= 1;
					if(keys & KEY_DOWN)	 character.y+=1;
					if(down & KEY_START) state = SPLASH;

					// randomizes x to an off screen value
					if (enemy.x < -16)	{ enemy.x = rand() % 256 + 240;	 enemy.y = rand() % 144; }
					else //moves enemies 1 tile to the left every frame
					{
						enemy.x -= 1;
					}

					if (enemy2.x < -16)	{ enemy2.x = rand() % 256 + 240;	 enemy2.y = rand() % 144; }
					else 
					{
						enemy2.x -= 1;
					}

					if (enemy3.x < -16)	{ enemy3.x = rand() % 256 + 240;	 enemy3.y = rand() % 144; }
					else
					{
						enemy3.x -= 1;
					}

					if (enemy4.x < -16)	{ enemy4.x = rand() % 256 + 240;	 enemy4.y = rand() % 144; }
					else
					{
						enemy4.x -= 1;
					}

					if (enemy5.x < -16)	{ enemy5.x = rand() % 256 + 240;	 enemy5.y = rand() % 144; }
					else
					{
						enemy5.x -= 1;
					}

					if (enemy6.x < -16)	{ enemy6.x = rand() % 256 + 240;	 enemy6.y = rand() % 144; }
					else
					{
						enemy6.x -= 1;
					}

				break;

			case LOST:
				lost();
				if (down & KEY_START)
					state = GAME;

				break;
					
		}	
		palettes();
	}
}
int menu()
{
	u16 down = keysDown();
	// switch to mode4
	REG_DISPCNT = (1<<4) | MODE_4 | BG2_ENABLE; 
	// put palette in memory
		drawBitmap(menuBitmap, menuPal);
	// moves from menu state to game once pressed
		if (down & KEY_START) return 1; 
		return 0;	
}
int lost()
{
	u16 down = keysDown();
	// switch to mode4
	REG_DISPCNT = (1 << 4) | MODE_4 | BG2_ENABLE;
	// put palette in memory
	drawBitmap(lostBitmap, lostPal);
	// moves from menu state to game once pressed
	if (down & KEY_START) return 1;
	return 0;
}
int game(int characterX, int characterY, int enemyX, int enemyY, int enemy2X, int enemy2Y, int enemy3X, int enemy3Y, int enemy4X, int enemy4Y, int enemy5X, int enemy5Y, int enemy6X, int enemy6Y, 
	int EnemBoundBoxX, int EnemBoundBoxY, int EnemBoundBoxX2,int EnemBoundBoxY2,
	int Enem2BoundBoxX, int Enem2BoundBoxY, int Enem2BoundBoxX2, int Enem2BoundBoxY2,
	int Enem3BoundBoxX, int Enem3BoundBoxY, int Enem3BoundBoxX2, int Enem3BoundBoxY2,
	int Enem4BoundBoxX, int Enem4BoundBoxY, int Enem4BoundBoxX2, int Enem4BoundBoxY2,
	int Enem5BoundBoxX, int Enem5BoundBoxY, int Enem5BoundBoxX2, int Enem5BoundBoxY2,
	int Enem6BoundBoxX, int Enem6BoundBoxY, int Enem6BoundBoxX2, int Enem6BoundBoxY2)
{
	EnemBoundBoxY =		(enemyY - 8);
	EnemBoundBoxY2 =	(enemyY + 8);
	EnemBoundBoxX =		(enemyX -8);
	EnemBoundBoxX2 =	(enemyX + 8);
	
	Enem2BoundBoxY =	(enemy2Y - 8);
	Enem2BoundBoxY2 =	(enemy2Y + 8);
	Enem2BoundBoxX =	(enemy2X - 8);
	Enem2BoundBoxX2 =	(enemy2X + 8);
	
	Enem3BoundBoxY =	(enemy3Y - 8);
	Enem3BoundBoxY2 =	(enemy3Y + 8);
	Enem3BoundBoxX =	(enemy3X - 8);
	Enem3BoundBoxX2 =	(enemy3X + 8);

	Enem4BoundBoxY =	(enemy4Y - 8);
	Enem4BoundBoxY2 =	(enemy4Y + 8);
	Enem4BoundBoxX =	(enemy4X - 8);
	Enem4BoundBoxX2 =	(enemy4X + 8);

	Enem5BoundBoxY =	(enemy5Y - 8);
	Enem5BoundBoxY2 =	(enemy5Y + 8);
	Enem5BoundBoxX =	(enemy5X - 8);
	Enem5BoundBoxX2 =	(enemy5X + 8);

	Enem6BoundBoxY =	(enemy6Y - 8);
	Enem6BoundBoxY2 =	(enemy6Y + 8);
	Enem6BoundBoxX =	(enemy6X - 8);
	Enem6BoundBoxX2 =	(enemy6X + 8);
	
	int enemy_OneX = enemyX;
	int enemy_OneY = enemyY;
	int enemy_TwoX = enemy2X;
	int enemy_TwoY = enemy2Y;
	int enemy_ThreeX = enemy3X;
	int enemy_ThreeY = enemy3Y;
	int enemy_FourX = enemy4X;
	int enemy_FourY = enemy4Y;
	int enemy_FiveX = enemy5X;
	int enemy_FiveY = enemy5Y;
	int enemy_SixX = enemy6X;
	int enemy_SixY = enemy6Y;
	
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
	// moves background
	bgHoff += 1;
	flowerHoff += 2;
	REG_BG0HOFS = bgHoff/2;
	REG_BG1HOFS = flowerHoff;
	spritesAndPalettes();
	int output;
	if (enemy_OneX - characterX == 0 || enemy_TwoX - characterX == 0 || enemy_ThreeX - characterX == 0 || enemy_FourX - characterX == 0 || enemy_FiveX - characterX == 0 || enemy_SixX - characterX == 0)
	{
		score += 1;
		Score(&score, &multiplier,&output);
	}
	/*
if ((enemy_OneX - characterX <= 1 && enemy_OneY - characterY <= 3) || (enemy_TwoX - characterX <= 1 && enemy_TwoY - characterY <= 3) || (enemy_ThreeX - characterX <= 1 && enemy_ThreeY - characterY <= 3) ||
		(enemy_FourX - characterX <= 1 && enemy_FourY - characterY <= 3) || (enemy_FiveX - characterX <= 1 && enemy_FiveY - characterY <= 3) || (enemy_SixX - characterX <= 1 && enemy_SixY - characterY <= 3))
	{
		lost();
	}*/
	collide(characterX, characterY, EnemBoundBoxX, EnemBoundBoxY, EnemBoundBoxX2, EnemBoundBoxY2);
	collide(characterX, characterY, Enem2BoundBoxX, Enem2BoundBoxY, Enem2BoundBoxX2, Enem2BoundBoxY2);
	collide(characterX, characterY, Enem3BoundBoxX, Enem3BoundBoxY, Enem3BoundBoxX2, Enem3BoundBoxY2);
	collide(characterX, characterY, Enem4BoundBoxX, Enem4BoundBoxY, Enem4BoundBoxX2, Enem4BoundBoxY2);
	collide(characterX, characterY, Enem5BoundBoxX, Enem5BoundBoxY, Enem5BoundBoxX2, Enem5BoundBoxY2);
	collide(characterX, characterY, Enem6BoundBoxX, Enem6BoundBoxY, Enem6BoundBoxX2, Enem6BoundBoxY2);



	char str[] = "";

	sprintf(str, "%d", output);
	drawString(80, 1, str);
	drawString(65, 1, "Score");

	// setting sprite oam memory, positions how many tiles form a sprite etc.
	u16* oam = (u16*)OAM;
	oam[0] = OBJ_Y(characterY) | OBJ_SHAPE(2);
	oam[1] = OBJ_X(characterX) | ATTR1_SIZE_8;
	oam[2] = (1<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[3] = 0;	

	oam[4] = OBJ_Y(60) | OBJ_SHAPE(2);
	oam[5] = OBJ_X(40) | ATTR1_SIZE_8;
	oam[6] = (3<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[7] = 0;

	oam[8] = OBJ_Y(enemy_OneY) | OBJ_SHAPE(0);
	oam[9] = OBJ_X(enemy_OneX) | ATTR1_SIZE_16;
	oam[10] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[11] = 0;
	
	oam[12] = OBJ_Y(enemy_TwoY) | OBJ_SHAPE(0); 
	oam[13] = OBJ_X(enemy_TwoX) | ATTR1_SIZE_16;
	oam[14] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[15] = 0;
	
	oam[16] = OBJ_Y(enemy_ThreeY) | OBJ_SHAPE(0); 
	oam[17] = OBJ_X(enemy_ThreeX) | ATTR1_SIZE_16;
	oam[18] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[19] = 0;

	oam[20] = OBJ_Y(enemy_FourY) | OBJ_SHAPE(0); 
	oam[21] = OBJ_X(enemy_FourX) | ATTR1_SIZE_16;
	oam[22] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[23] = 0;

	oam[24] = OBJ_Y(enemy_FiveY) | OBJ_SHAPE(0); 
	oam[25] = OBJ_X(enemy_FiveX) | ATTR1_SIZE_16;
	oam[26] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[27] = 0;

	oam[28] = OBJ_Y(enemy_SixY) | OBJ_SHAPE(0); 
	oam[29] = OBJ_X(enemy_SixX) | ATTR1_SIZE_16;
	oam[30] = (5 << 0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(1);
	oam[31] = 0;
	
	return 0;
}
void collide(int characterX, int characterY, int enemyX, int enemyY, int enemy2X, int enemy2Y)
{
	if (characterX >= enemyX && characterX <= enemy2X && characterY >= enemyY && characterY <= enemy2Y)
	{
		lost();
		score = 0;
	}
}

void spritesAndPalettes()
{
	// -- Base block 0.
	//first character tile. for clearing screen i think, without this squares show on screen
	u32* first = (u32*)0x06000000;
	for (int c = 0; c<8; c++) first[c] = 0;

	u16* obj = SPRITE_PALETTE;
	obj[0] = RGB5(1, 1, 1);
	obj[1] = RGB5(30, 21, 2); 	// orange/gold
	obj[2] = RGB5(29, 31, 2); 	// yellow
	obj[3] = RGB5(31, 26, 28); 	// light pink
	obj[4] = RGB5(29, 5, 24);		// pink
	obj[5] = RGB5(14, 2, 12);		// purple
	obj[6] = RGB5(29, 5, 11);		// red
	obj[7] = RGB5(5, 1, 2);		// dark brown
	obj[8] = RGB5(8, 1, 2);		// beige ??
	obj[9] = RGB5(28, 24, 16);	// light orange
	obj[10] = RGB5(9, 6, 1);		// brown
	obj[11] = RGB5(15, 14, 2);	// olive
	obj[12] = RGB5(31, 31, 31);	// white
	obj[13] = RGB5(4, 4, 4);		// dark grey
	obj[14] = RGB5(0, 0, 0);
	obj[15] = RGB5(31, 21, 10);
	obj[16] = RGB5(3, 7, 8);
	obj[17] = RGB5(3, 7, 8);
	obj[18] = RGB5(2, 14, 9);
	obj[19] = RGB5(0, 24, 7);

	// sprite 0 
	u32* firstSprite = OBJ_BASE_ADR; //sprite memory at tile 0
	for (int c = 0; c<8; c++)	firstSprite[c] = 0;		// makes the first sprite blank

	// sprite 1 
	u32* sprite = OBJ_BASE_ADR + (0x20 * 1); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	// tile 1
	sprite[0] = (1 << 4) | (1 << 12) | (1 << 20);
	sprite[1] = (1 << 4) | (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20);
	sprite[2] = (2 << 4) | (2 << 8) | (2 << 12) | (2 << 16) | (2 << 20);
	sprite[3] = (2 << 4) | (2 << 8) | (7 << 12) | (3 << 16) | (7 << 20) | (2 << 24);
	sprite[4] = (2 << 0) | (2 << 4) | (2 << 8) | (3 << 12) | (3 << 16) | (2 << 20) | (2 << 24);
	sprite[5] = (2 << 0) | (2 << 4) | (2 << 8) | (3 << 12) | (4 << 16) | (2 << 20);
	sprite[6] = (2 << 0) | (2 << 4) | (5 << 8) | (5 << 12) | (5 << 16) | (5 << 20);
	sprite[7] = (3 << 8) | (5 << 12) | (5 << 16) | (3 << 20);
	// tile 2
	sprite[8] = (3 << 8) | (5 << 12) | (5 << 16) | (3 << 20);
	sprite[9] = (5 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 << 20) | (5 << 24);
	sprite[10] = (5 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 << 20) | (5 << 24);
	sprite[11] = (5 << 0) | (4 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 << 20) | (4 << 24) | (5 << 28);
	sprite[12] = (4 << 0) | (5 << 4) | (3 << 8) | (5 << 12) | (5 << 16) | (5 << 20) | (5 << 24) | (4 << 28);
	sprite[13] = (4 << 8) | (4 << 20);
	sprite[14] = (4 << 8) | (4 << 20);
	sprite[15] = (4 << 8) | (4 << 12) | (4 << 20) | (4 << 24);

	u32* sprite2 = OBJ_BASE_ADR + (0x20 * 3); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	// tile 1
	sprite2[0] = (12 << 8) | (12 << 12) | (12 << 16) | (12 << 20);
	sprite2[1] = (8 << 8) | (9 << 12) | (8 << 16) | (9 << 20);
	sprite2[2] = (9 << 8) | (9 << 12) | (9 << 16) | (9 << 20);
	sprite2[3] = (10 << 8) | (8 << 12) | (8 << 16) | (10 << 20);
	sprite2[4] = (13 << 0) | (1 << 4) | (10 << 8) | (10 << 12) | (10 << 16) | (10 << 20) | (1 << 24) | (13 << 28);
	sprite2[5] = (13 << 0) | (12 << 4) | (1 << 8) | (10 << 12) | (10 << 16) | (1 << 20) | (12 << 24) | (13 << 28);
	sprite2[6] = (13 << 0) | (12 << 4) | (14 << 8) | (1 << 12) | (1 << 16) | (14 << 20) | (12 << 24) | (13 << 28);
	sprite2[7] = (13 << 0) | (12 << 4) | (14 << 8) | (2 << 12) | (2 << 16) | (14 << 20) | (12 << 24) | (13 << 28);
	// tile 2
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
}


