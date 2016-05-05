
//{{BLOCK(background)

//======================================================================
//
//	background, 256x256@8, 
//	+ palette 255 entries, not compressed
//	+ 201 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 510 + 12864 + 2048 = 15422
//
//	Time-stamp: 2016-04-13, 23:51:18
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKGROUND_H
#define GRIT_BACKGROUND_H

#define backgroundTilesLen 12864
extern const unsigned short backgroundTiles[6432];

#define backgroundMapLen 2048
extern const unsigned short backgroundMap[1024];

#define backgroundPalLen 510
extern const unsigned short backgroundPal[256];

#endif // GRIT_BACKGROUND_H

//}}BLOCK(background)
