
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_sprites.h>
#include <gba_input.h>

#include <stdio.h>
//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
u16 test = 20;
int main(void) {
//---------------------------------------------------------------------------------

	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	irqInit();
	irqEnable(IRQ_VBLANK);

	// generic setup function
	consoleDemoInit();

/*
	--- control register ---
	Bits 0-2 (reading from right to left) graphics mode (0-5) – set this to 0
	Bit 3                                 Ignore this
	Bit 4                                 Ignore this
	Bit 5                                 Ignore this for now
	Bit 6                                 OBJ data mode. Not needed today so set to 0
	Bit 7                                 Ignore this
	Bit 8                                 BG0 layer on / off – set this to 1
	Bit 9                                 BG1 layer on / off – set this to 1
	Bit 10                                BG2 layer on / off – set this to 1
	Bit 11                                BG3 layer on / off – set this to 1
	Bit 12                                OBJs on / off – not needed today so set to 0
	Bit 13                                Ignore this
	Bit 14                                Ignore this
	Bit 15                                Ignore this
*/
	// display control register
	REG_DISPCNT = MODE_0 | OBJ_1D_MAP | BG_ALL_ENABLE | OBJ_ENABLE;
/*
	---BG Layers---
	Bits 0-1 (reading from right to left)   Layer priority (0-3, with 0 closest to the viewer I think)
	Bits 2-3                                Character base block number to use for this layer (0-3)
	Bit 4                                   Ignore this
	Bit 5                                   Ignore this
	Bit 6                                   Ignore this
	Bit 7                                   Colour mode. Set this to 0 to tell the GBA we want 16 palettes of 16 colours)
	Bits 8-12                               Screen base block to use for this layer (0-31)
	Bit 13                                  Ignore this
	Bits 14-15                              Screen size setting. Set to 0 for now (256 * 256 pixels)
*/
	//background control registers
	REG_BG0CNT = (0 << 0) | (0 << 2) | BG_16_COLOR | (11 << 8)| BG_SIZE_0;
	REG_BG1CNT = (1 << 0) | (0 << 2) | BG_16_COLOR | (10 << 8)| BG_SIZE_0;
	REG_BG2CNT = (2 << 0) | (0 << 2) | BG_16_COLOR | (9 << 8) | BG_SIZE_0;
	REG_BG3CNT = (3 << 0) | (0 << 2) | BG_16_COLOR | (8 << 8) | BG_SIZE_0;
	
	
	
/*
	Palettes and colours BG&OBJ
*/
	u16* pal = BG_PALETTE;
	//pallette 0
	//pal[0] = RGB5(0,0,0); // transparent default blue, background
	pal[1] = RGB5(0,31,0);
	pal[2] = RGB5(0,0,31);
	pal[3] = RGB5(15,15,15);
	//pallete 1
	pal[16] = RGB5(10,10,10);
	//pallette 2 
	pal[32] = RGB5(10,10,10);
	//and so on up to pallette 16...
	
	u16* obj = SPRITE_PALETTE;
	obj[0] = RGB5(0,0,0);
	obj[1] = RGB5(10,0,0);
	obj[2] = RGB5(21,0,0);
	obj[3] = RGB5(31,0,0);
	obj[4] = RGB5(0,10,0);
	obj[5] = RGB5(0,21,0);
	obj[6] = RGB5(0,31,0);
	obj[7] = RGB5(0,0,10);
	obj[8] = RGB5(0,0,21);
	obj[9] = RGB5(0,0,31);
	obj[10] = RGB5(10,10,10);
	obj[11] = RGB5(21,21,21);
	obj[12] = RGB5(31,31,31);
	obj[13] = RGB5(31,10,21);
	obj[14] = RGB5(10,21,31);
	obj[15] = RGB5(31,21,10);
	
	
	
/*
	Bits 0-9                A 10-bit number representing the ID of the character to use (0-1024). For example, if in 
							character memory you wrote ‘a’, ’b’, ’c’ as your first three characters, a value of 2 here would be the ‘c’.
	Bit 10                 	Whether or not the character is flipped horizontally. Use 0 for now.
	Bit 11                  Whether or not the character is flipped vertically. Use 0 for now.
	Bits 12-15           	A 4-bit number indicating the colour palette to use for this character (0-15).
*/
	u16* ScreenBG0 = (u16*)0x06005800;// Map Base Adress, can be found in map view
	ScreenBG0[(1 * 32) + 0] = (77 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	u16* ScreenBG1 = (u16*)0x06005000;// Map Base Adress, can be found in map view
	ScreenBG1[(0 * 32) + 1] = (78 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	u16* ScreenBG2 = (u16*)0x06004800;	// Map Base Adress, can be found in map view
	ScreenBG2[(0 * 32) + 2] = (79 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	u16* ScreenBG3 = (u16*)0x06004000;// Map Base Adress, can be found in map view
	ScreenBG3[(0 * 32) + 3] = (1 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);
	
	
	
	
	
	
	// sprite 0 
	u32* firstSprite = OBJ_BASE_ADR; //sprite memory at tile 0
	firstSprite[0] = 0;
	firstSprite[1] = 0;
	firstSprite[2] = 0;
	firstSprite[3] = 0;
	firstSprite[4] = 0;
	firstSprite[5] = 0;
	firstSprite[6] = 0;
	firstSprite[7] = 0;
	// sprite 0 
	u32* sprite = OBJ_BASE_ADR + (0x20*1); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	sprite[0] = (13 << 12);
	sprite[1] = (13 << 8) | (13 << 16);
	sprite[2] = (13 << 12);
	sprite[3] = (13 << 8) | (13 << 12);
	sprite[4] = (13 << 4) | (13 << 16);
	sprite[5] = (13 << 4) | (13 << 20);
	sprite[6] = (13 << 4) | (13 << 20);
	sprite[7] = (13 << 0) | (13 << 4) | (13 << 8) | (13 << 16);
	
	u32* sprite2 = OBJ_BASE_ADR + (0x20*3); // sprite memory location at tile x - OBJ_BASE_ADR + (0x20 * x)
	sprite2[0] = (4 << 12);
	sprite2[1] = (4 << 8) | (4 << 16);
	sprite2[2] = (4 << 12);
	sprite2[3] = (13 << 8) | (13 << 12);
	sprite2[4] = (13 << 4) | (13 << 16);
	sprite2[5] = (13 << 4) | (13 << 20);
	sprite2[6] = (13 << 4) | (13 << 20);
	sprite2[7] = (13 << 0) | (13 << 4) | (13 << 8) | (13 << 16);
	
	u16* oam = (u16*)OAM;
	oam[0] = OBJ_Y(50) | OBJ_SHAPE(0);
	oam[1] = OBJ_X(50) | ATTR1_SIZE_8;
	oam[2] = (1<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[3] = 0;
	
	oam[4] = OBJ_Y(30) | OBJ_SHAPE(0);
	oam[5] = OBJ_X(30) | ATTR1_SIZE_8;
	oam[6] = (3<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[7] = 0;

	
	oam[8] = OBJ_Y(test) | OBJ_SHAPE(0);
	oam[9] = OBJ_X(30) | ATTR1_SIZE_8;
	oam[10] = (3<<0) | ATTR2_PRIORITY(0) | ATTR2_PALETTE(0);
	oam[11] = 0;
	//oam[11] = 0;
	
	
	
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

	// main loop
	while (1) 
	{
		VBlankIntrWait();
		scanKeys();
		u16 keys = keysHeld();
		
		if(keys & KEY_UP)	oam[8] = OBJ_Y(test++) | OBJ_SHAPE(0);
		if(keys & KEY_DOWN)	oam[8] = OBJ_Y(test--) | OBJ_SHAPE(0);
		//iprintf("\x1b[10;10Hresult = A a ");
	}
}


