
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_sprites.h>

#include <stdio.h>





//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
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
	REG_DISPCNT = MODE_0 | (0 << 6) | BG_ALL_ENABLE | (0 << 12);
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
	Palettes and colours
*/
	u16* pal = BG_PALETTE;
	//pallette 0
	pal[0] = RGB5(31,0,0);
	pal[1] = RGB5(0,31,0);
	pal[2] = RGB5(0,0,31);
	pal[3]= RGB5(15,15,15);
	//pallete 1
	pal[16] = RGB5(10,10,10);
	//pallette 2 
	pal[32] = RGB5(10,10,10);
	//and so on...
/*
	Bits 0-9                A 10-bit number representing the ID of the character to use (0-1024). For example, if in 
							character memory you wrote ‘a’, ’b’, ’c’ as your first three characters, a value of 2 here would be the ‘c’.
	Bit 10                 	Whether or not the character is flipped horizontally. Use 0 for now.
	Bit 11                  Whether or not the character is flipped vertically. Use 0 for now.
	Bits 12-15           	A 4-bit number indicating the colour palette to use for this character (0-15).
*/
	unsigned short* ScreenBG0 = (unsigned short*)0x6005800;// Map Base Adress, can be found in map view
	ScreenBG0[(0 * 32) + 0] = (77 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	unsigned short* ScreenBG1 = (unsigned short*)0x6005000;// Map Base Adress, can be found in map view
	ScreenBG1[(0 * 32) + 0] = (78 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	unsigned short* ScreenBG2 = (unsigned short*)0x6004800;	// Map Base Adress, can be found in map view
	ScreenBG2[(0 * 32) + 0] = (79 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	unsigned short* ScreenBG3 = (unsigned short*)0x6004000;// Map Base Adress, can be found in map view
	ScreenBG3[(0 * 32) + 0] = (1 << 0)| (0 << 10) | (0 << 11) | CHAR_PALETTE(0);

	
	
	// -- Base block 0
	unsigned int* first = (unsigned int*)0x6000000;
	first[0] = 0;
	first[1] = 0;
	first[2] = 0;
	first[3] = 0;
	first[4] = 0;
	first[5] = 0;
	first[6] = 0;
	first[7] = 0;

	unsigned int* character = (unsigned int*)0x6000020;
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
		//iprintf("\x1b[10;10Hresult = A a ");
	}
}


