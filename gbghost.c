#include <gb/gb.h> //main gbdk library header
#include <rand.h> //RNG header

#define GHOST_ANIM_SPEED 5 //number of frames (at 30 fps) before changing the sprite animation frame for ghost sprite
#define STARTX 80 //center object horizontally ([160 / 2])
#define STARTY 72 //center vertically ([144 / 2])
#define SCORE_SPRITE_X 80

const unsigned char ghost[] = //12 tiles, 3 ghost sprite frames of 4 tiles each
{
  0x01,0x01,0x02,0x03,0x05,0x06,0x0B,0x0C,
  0x17,0x18,0x3F,0x20,0x7F,0x40,0x67,0x58,
  0xEF,0x98,0x93,0xFC,0x90,0xFF,0xD0,0xFF,
  0x30,0x3F,0x0A,0x0F,0x04,0x07,0x07,0x07,
  0xF0,0xF0,0x08,0xF8,0xF4,0x0C,0xFE,0x12,
  0xFC,0x32,0xFC,0x33,0xFF,0x01,0xFD,0x03,
  0xF8,0x0E,0xFE,0x0E,0xFC,0x04,0x08,0xF8,
  0x30,0xF0,0x40,0xC0,0x80,0x80,0x00,0x00,
  0x01,0x01,0x02,0x03,0x05,0x06,0x0B,0x0C,
  0x17,0x18,0x1F,0x10,0x1F,0x10,0x2F,0x30,
  0xDF,0xE0,0xBE,0xC1,0x8C,0xF3,0x40,0x7F,
  0x31,0x3F,0x09,0x0F,0x06,0x07,0x01,0x01,
  0xF0,0xF0,0x08,0xF8,0xF4,0x0C,0xFE,0x12,
  0xFC,0x32,0xFC,0x33,0xFF,0x01,0xFD,0x03,
  0xF8,0x0E,0x7E,0x8E,0x7C,0x84,0x88,0xF8,
  0x10,0xF0,0x20,0xE0,0x40,0xC0,0x80,0x80,
  0x01,0x01,0x02,0x03,0x05,0x06,0x0B,0x0C,
  0x17,0x18,0x1F,0x10,0x1F,0x10,0x2F,0x30,
  0x3D,0x22,0x39,0x26,0x52,0x6F,0xC4,0xFF,
  0x24,0x3F,0x18,0x1F,0x06,0x07,0x01,0x01,
  0xF0,0xF0,0x08,0xF8,0xF4,0x0C,0xFE,0x12,
  0xFC,0x32,0xFC,0x33,0xFF,0x01,0xFD,0x03,
  0xF8,0x0E,0xFE,0x0E,0x7C,0x84,0x08,0xF8,
  0x10,0xF0,0x10,0xF0,0x20,0xE0,0xC0,0xC0
};

const unsigned char walls[] = //2 tiles, brick wall and stone end cap
{
  0xFF,0xFF,0x87,0x7C,0x06,0xFD,0x04,0xFF,
  0xFF,0xFF,0x3C,0xE3,0x30,0xEF,0x20,0xFF,
  0xFF,0xFF,0xFF,0x01,0xFD,0x03,0xC1,0x3F,
  0xC1,0x3F,0xC1,0x3F,0xC3,0x3F,0x87,0x7F
};

const unsigned char score[] = //20 tiles, digits 0 - 9, 2 tiles tall
{
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x93,
  0xFF,0x93,0xFF,0x93,0xFF,0x93,0xFF,0x93,
  0xFF,0x93,0xFF,0x93,0xFF,0x93,0xFF,0x93,
  0xFF,0x83,0xFF,0x83,0xFF,0xFF,0x7F,0x7F,
  0x7C,0x7C,0x7E,0x46,0x7E,0x46,0x7E,0x46,
  0x7E,0x46,0x7E,0x46,0x7E,0x46,0x7E,0x46,
  0x7E,0x46,0x7E,0x46,0x7E,0x46,0x7E,0x46,
  0x7E,0x46,0x7E,0x46,0x7E,0x7E,0x3E,0x3E,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x83,
  0xFF,0xE3,0xFF,0xE3,0xFF,0x83,0xFF,0x83,
  0xFF,0x83,0xFF,0x8F,0xFF,0x8F,0xFF,0x83,
  0xFF,0x83,0xFF,0x83,0xFF,0xFF,0x7F,0x7F,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x83,
  0xFF,0xE3,0xFF,0xE3,0xFF,0x83,0xFF,0x83,
  0xFF,0x83,0xFF,0xE3,0xFF,0xE3,0xFF,0x83,
  0xFF,0x83,0xFF,0x83,0xFF,0xFF,0x7F,0x7F,
  0xFE,0xFE,0xFF,0x93,0xFF,0x93,0xFF,0x93,
  0xFF,0x93,0xFF,0x93,0xFF,0x83,0xFF,0x83,
  0xFF,0xF3,0x1F,0x13,0x1F,0x13,0x1F,0x13,
  0x1F,0x13,0x1F,0x13,0x1F,0x1F,0x0F,0x0F,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x83,
  0xFF,0x8F,0xFF,0x8F,0xFF,0x83,0xFF,0x83,
  0xFF,0x83,0xFF,0xE3,0xFF,0xE3,0xFF,0x83,
  0xFF,0x83,0xFF,0x83,0xFF,0xFF,0x7F,0x7F,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x83,
  0xFF,0x8F,0xFF,0x8F,0xFF,0x83,0xFF,0x83,
  0xFF,0x83,0xFF,0x93,0xFF,0x93,0xFF,0x83,
  0xFF,0x83,0xFF,0x83,0xFF,0xFF,0x7F,0x7F,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x83,
  0xFF,0xF3,0x1F,0x13,0x1F,0x13,0x1F,0x13,
  0x1F,0x13,0x1F,0x13,0x1F,0x13,0x1F,0x13,
  0x1F,0x13,0x1F,0x13,0x1F,0x13,0x0F,0x0F,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x93,
  0xFF,0x93,0xFF,0x93,0xFF,0x83,0xFF,0x83,
  0xFF,0x83,0xFF,0x93,0xFF,0x93,0xFF,0x93,
  0xFF,0x83,0xFF,0x83,0xFF,0xFF,0x7F,0x7F,
  0xFE,0xFE,0xFF,0x83,0xFF,0x83,0xFF,0x93,
  0xFF,0x93,0xFF,0x93,0xFF,0x83,0xFF,0x83,
  0xFF,0xF3,0x1F,0x13,0x1F,0x13,0x1F,0x13,
  0x1F,0x13,0x1F,0x13,0x1F,0x1F,0x0F,0x0F
};

const unsigned char button[] = //4 tiles, single frame of button sprite
{
  0x00,0x00,0x00,0x00,0x03,0x03,0x0F,0x0C,
  0x1F,0x18,0x1F,0x11,0x3F,0x22,0x3F,0x23,
  0x3F,0x22,0x3F,0x22,0x1F,0x30,0x1F,0x38,
  0x0F,0x3C,0x03,0x1F,0x00,0x0F,0x00,0x03,
  0x00,0x00,0x00,0x00,0xC0,0xC0,0xF0,0x30,
  0xF8,0x18,0xF8,0x88,0xFC,0x44,0xFC,0xC4,
  0xFC,0x44,0xFC,0x44,0xF8,0x0C,0xF8,0x1C,
  0xF0,0x3C,0xC0,0xF8,0x00,0xF0,0x00,0xC0
};

const unsigned char tiles[] = //4 tiles, in-game background
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0xFF,0x78,0x04,0xF9,0x04,0xFB,0x04,
  0x00,0xFF,0xC3,0x20,0xCF,0x20,0xDF,0x20,
  0x00,0xFF,0x00,0x01,0x02,0x01,0x3E,0x01,
  0x3E,0x01,0x3E,0x01,0x3C,0x03,0x78,0x07,
  0x70,0x80,0x7F,0x80,0x80,0xFF,0xFF,0xFF,
  0x1C,0x10,0xF7,0x18,0xF7,0x18,0xF7,0x18
};

const unsigned char menu_tiles[] = //93 tiles, menu background
{
	0xFF,0x00,0x3F,0xC0,0x07,0xF8,0x03,0xFC,0x03,0xFC,0x03,0xFC,0x01,0xFE,0x01,0xFE,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xC7,0xFB,0xC7,0xFB,0xC7,0xFB,0xC7,0xFB,0xC7,0xFB,0xC7,0xFB,0xC7,0xFB,0xC7,0xFB,
	0xFF,0x00,0xFF,0x00,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,
	0xFF,0x00,0xFF,0x00,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0xFF,0x02,0xFF,0x02,0x07,0xFA,0x07,0xFA,0x07,0xFA,0x07,0xFA,0x07,0xFA,0x07,0xFA,
	0xFF,0x00,0xFF,0x00,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0xC3,0x3C,
	0xFF,0x3C,0xFF,0x3C,0x7E,0xBD,0x7E,0xBD,0x7E,0xBD,0x7E,0xBD,0x7E,0xBD,0xFF,0x3C,
	0xFF,0x00,0xFF,0x00,0x01,0xFE,0x01,0xFE,0x01,0xFE,0x01,0xFE,0x01,0xFE,0x87,0x78,
	0xFF,0x80,0xFF,0x80,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,
	0xFF,0x01,0xFF,0x01,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,
	0xFF,0x01,0xFF,0x01,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,
	0xFF,0x00,0xFF,0x00,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,
	0xFF,0x03,0xFF,0x03,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,
	0xFF,0x01,0xFF,0x01,0xC1,0x3E,0xC1,0x3E,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFE,0xFE,0x7D,0xFE,0x7D,0xFE,0x7D,0x7C,0xBB,
	0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0x1E,0xE1,0x1E,0xE1,0x7F,0x9E,0x7F,0x9F,0x7F,0x9F,
	0x07,0xFA,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0xFF,0x03,0xFF,0xFF,0xFF,0xFF,
	0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,
	0xFF,0x3C,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,
	0x87,0x78,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,
	0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,
	0x00,0xFF,0x00,0xFF,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,
	0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,0x07,0xF9,
	0x00,0xFF,0x00,0xFF,0x0C,0xF3,0x0C,0xF3,0x0C,0xF3,0x0C,0xF3,0x0C,0xF3,0x0F,0xF0,
	0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0xFF,0x01,
	0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xFE,0x01,0xFE,0x01,0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0x07,0xF8,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,
	0x07,0xFB,0x07,0xFB,0x07,0xFB,0xFF,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xE0,0xDF,0xF0,0xEF,
	0x38,0xD7,0x38,0xD7,0x10,0xEF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x07,0xFB,0x0F,0xF7,
	0x7F,0x9F,0x7F,0x9F,0x7F,0x9F,0x7F,0x9F,0x7F,0x9F,0xFF,0x00,0x80,0x7F,0x80,0x7F,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0x03,0xFD,0x03,0xFD,
	0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,
	0xFF,0xFE,0xFF,0xFE,0xFF,0x00,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0x87,0x7B,0x87,0x7B,0x87,0x7B,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,
	0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x3C,0xC3,0x3C,0xC3,
	0x0F,0xF0,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0xFF,0x01,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,
	0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,0xFE,0xF9,
	0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xF0,0xEF,0xF0,0xEF,0xF8,0xF7,0xFC,0xFB,0xFC,0xFB,0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0x1F,0xEF,0x1F,0xEF,0x3F,0xDF,0x7F,0xBF,0x7F,0xBF,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,
	0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0x80,0x7F,0xF8,0x07,0x00,0xFF,0x00,0xFF,
	0x03,0xFD,0x03,0xFD,0x0F,0xF1,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,
	0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC0,0x3F,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFE,0xFF,0xFE,
	0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x07,0xFB,0x87,0x7B,0x87,0x7B,0x87,0x7B,
	0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,0x3C,0xC3,
	0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,0xFF,0x80,0xFF,0x80,0xE0,0x9F,0xE0,0x9F,0xE0,0x9F,
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFC,0x03,0xFC,0x03,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,
	0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,0x03,0xFD,
	0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,0xFE,0xFD,
	0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,0xFF,0x7F,
	0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,0x0F,0xF7,
	0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,
	0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,0x87,0x7B,
	0x3C,0xC3,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0x1C,0xE3,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
	0xC0,0x3F,0xFF,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x1F,0xE7,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xC3,0x3C,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFE,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x87,0x7B,0xFF,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xE0,0x9F,0xFF,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x07,0xF9,0xFF,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x03,0xFD,0xFF,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFE,0xF9,0xFF,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x07,0xFB,0xFF,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFE,0xFD,0xFF,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0x7F,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFC,0xF8,0xF8,0xF8,0xF8,0xE0,0xE0,0xC1,0xC1,
	0xE0,0xE0,0xC3,0xC3,0x1F,0x0F,0x7F,0x1F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,
	0xC1,0xC1,0xC1,0xC1,0x83,0x83,0x83,0x83,0x83,0x83,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0xC0,0xC0,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x1F,0x1F,0x0F,0x0F,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xFD,0xFB,0xF9,0xF3,0xF3,0xE3,0xE3,0xC7,0xC7,
	0xC0,0xC0,0xC0,0xC0,0xE0,0xE0,0xF8,0xF8,0xF8,0xF8,0xFC,0xFC,0xFF,0xFF,0xFF,0xFF,
	0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xE0,0xE0,
	0xFF,0xFF,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x07,0x07,0x1F,0x1F,
	0x8F,0x8F,0x1F,0x1F,0x3F,0x3F,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x03,0x03,0x31,0x31,0x31,0x31,0x31,0x31,0x03,0x03,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
	0x03,0x03,0x31,0x31,0x31,0x31,0x03,0x03,0x0F,0x0F,0x27,0x27,0x33,0x33,0x39,0x39,
	0x03,0x03,0x3F,0x3F,0x3F,0x3F,0x0F,0x0F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x03,0x03,
	0x83,0x83,0x19,0x19,0x0F,0x0F,0xC7,0xC7,0xE3,0xE3,0xF1,0xF1,0x11,0x11,0x83,0x83,
	0x01,0x01,0x01,0x01,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,
	0xC7,0xC7,0x83,0x83,0x93,0x93,0xBB,0xBB,0x39,0x39,0x01,0x01,0x39,0x39,0x39,0x39,
	0x01,0x01,0x38,0x38,0x38,0x38,0x01,0x01,0x07,0x07,0x33,0x33,0x39,0x39,0x3C,0x3C
};

const unsigned char menu_map[] = //menu background tile map, 20w x 18h, uses menu_tiles tileset
{
	0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
	0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x04,0x0A,0x09,0x04,0x0B,0x0C,0x04,0x0D,0x0E,0x0F,0x0B,0x02,
	0x02,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x16,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x02,
	0x02,0x10,0x21,0x22,0x23,0x24,0x25,0x16,0x26,0x18,0x16,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x02,
	0x02,0x10,0x2F,0x30,0x31,0x32,0x33,0x16,0x34,0x18,0x35,0x36,0x37,0x29,0x2A,0x2B,0x38,0x2D,0x39,0x02,
	0x02,0x10,0x2D,0x3A,0x13,0x3B,0x3C,0x16,0x3D,0x18,0x16,0x3E,0x37,0x29,0x2A,0x2B,0x38,0x2D,0x39,0x02,
	0x02,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x40,0x46,0x45,0x40,0x47,0x48,0x49,0x2B,0x4A,0x40,0x4B,0x02,
	0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x2B,0x2B,0x2B,0x2B,0x2B,0x09,0x04,0x0B,0x0C,0x04,0x0D,0x2B,0x02,
	0x02,0x10,0x11,0x12,0x13,0x14,0x15,0x2B,0x4C,0x4D,0x2B,0x2B,0x16,0x19,0x1A,0x1B,0x1C,0x1D,0x2B,0x02,
	0x02,0x10,0x21,0x22,0x23,0x24,0x25,0x2B,0x4E,0x2B,0x2B,0x2B,0x16,0x27,0x28,0x29,0x2A,0x2B,0x2B,0x02,
	0x02,0x10,0x2F,0x30,0x31,0x32,0x33,0x2B,0x4F,0x50,0x2B,0x51,0x35,0x36,0x37,0x29,0x2A,0x2B,0x2B,0x02,
	0x02,0x10,0x2D,0x3A,0x13,0x3B,0x3C,0x2B,0x52,0x53,0x54,0x55,0x16,0x3E,0x37,0x29,0x2A,0x2B,0x2B,0x02,
	0x02,0x3F,0x40,0x41,0x42,0x43,0x44,0x2B,0x2B,0x2B,0x2B,0x2B,0x45,0x40,0x47,0x48,0x49,0x2B,0x2B,0x02,
	0x02,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x02,
	0x02,0x2B,0x2B,0x2B,0x56,0x57,0x58,0x59,0x59,0x2B,0x2B,0x59,0x5A,0x5B,0x5C,0x5A,0x2B,0x2B,0x2B,0x02,
	0x02,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x02,
	0x02,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x02,
	0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00
};

const unsigned char bgmap[] = //in game background tile map, 32w x 18h, uses tiles tileset
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
};

enum GAME_STATE {GAME_MENU, PRE_PLAY, PLAY, GAME_OVER, RESET}; //used for the game's overall state machine
const UINT8 xspeed = 2; //horizontal perceived speed of character. we scroll the background and pipes by this value each frame
UINT8 	counter, //generic reused variable. used in for loops and whatever else
		y, //ghost sprite's y value. needed for collision checking mostly
		anim_frame, //tracks animation frame of ghost sprite
		button_frame, //TODO: delete if remains unused
		anim_counter, //counter variable specifically for timing the ghost animation
		grav_counter, //counter variable to apply gravity every nth frame, allows finer control of gravity with integers
		wall_x, //track scrolling wall's x position relative to the background's absolute 0
		wall_rand, //random offset used for the placement of the gap in the wall
		current_score; //score tracker. resets on death. not saved naywhere TODO: save high scores
		//wall_tiles_location; //SCX and tile offset when regenerating wall. tracks tile column the obstacle is in relative to tile columns of room TODO: delete if remains unused
UINT8 is_pressed_J_A; //boolean algebra value, state tracker for A button
INT8 yspeed; //vertical speed of ghost character. applied every frame (30 fps) during gameplay
enum GAME_STATE current_state; //holds overall game state machine's current state

void init_all_sprites(); //assign the correct tiles to each sprite at the start of the game
void reset_all_sprites(); //move all sprites to their starting positions
void update_ghost_sprites(); //reposition and animate ghost
void update_wall_sprites(); //reposition wall sprites, change gap in wall, update score
void update_score(); //iterate the score variable (in bcd)
void check_coll(); //run crappy box collision detection for ghost and wall
void animate_ghost(); //iterate ghost sprite frames when called
void play_sound_jump(); //plays "jump" sound effect on channels 1 and 4
void play_sound_crash(); //plays "crash" sound effect on chnnel 4

void main()
{
	//do all of our basic setup with the screen off (so we can edit VRAM) and interrupts disabled
	disable_interrupts();
	DISPLAY_OFF;
	
	// load background
	set_bkg_data(0,93,menu_tiles);
	set_bkg_tiles( 0, 0, 20, 18, menu_map);
	
	//initial setup of variables. anything non-const is initialized here so as not to have the init value in rom and ram both (pretty sure that's a thing)
	current_state = GAME_MENU; //set game state
	yspeed = 0; //initialize ghost sprite's y speed to 0
	is_pressed_J_A = 0;	//reset the A button state tracker to off
	wall_rand=0; //used for randomizing wall height.
	current_score = 0x00; //init score to 0
	anim_frame=0; //set ghost animation to it's first frame
	anim_counter = 0; //the counter/timer to change the ghost animation frame
	grav_counter = 0; //counter/timer for adding gravity to yspeed
	wall_x = 232U; //track wall location
	y = STARTY; //initialize ghost y value to STARTY
	SPRITES_8x8; //use 8x8 (vice 8x16) sprites
	init_all_sprites(); //set all of our sprite data and positions
	
	NR52_REG = 0x80U; //Turns on sound hardware
	NR51_REG = 0x99U; //turn channels 1 and 3 on, 2 and 3 off
	NR50_REG = 0x77U; //Sets volume to max for left and right speakers
	
	initrand((UINT16)DIV_REG); //init the RNG. RNG used for the height of the wall opening

	SHOW_BKG; //draw bkg layer
	SHOW_SPRITES; //draw sprite layer
	DISPLAY_ON; //turn on LCD, limited access to VRAM after this unless in vsync
	enable_interrupts(); //turn on all interrupts

	while(1) {
		wait_vbl_done();//skip a frame for speed reasons. not the best solution, but an easy one. game is now effectively 30 fps
		wait_vbl_done();//wait until vsync to update game logic
		
		switch(current_state) //game state machine
		{
			case GAME_MENU: //the initial splash screen
			{
				if(joypad()) //check for ANY button presses
				{
					// load background for actual game
					set_bkg_data(0,4,tiles); //
					set_bkg_tiles( 0, 0, 32, 18, bgmap);
					reset_all_sprites(); //reset all sprites to their initial positions
					if(counter & J_A)
						is_pressed_J_A = 1; //the splash says press start,
						                    //but ANY button can be used to go to the game,
											//and if they press A we don't want to skip the pre-play phase
					current_state = PRE_PLAY; //move on to the game
				}
				break;
			}
			case PRE_PLAY: //phase of game with no walls, that says to press A and just waits for input
			{
				anim_counter++; //incrememnt counter/timer used for updating the ghost sprite's animation
				
				//Get and respond to joypad input
				counter = joypad();
				if(counter & J_A) //A button pressed, this is the jump button
				{
					if(is_pressed_J_A == 0)//used to only respond to the inital press of the button, not to held buttons
					{
						yspeed = (INT8)-4; //if A pressed, jump by setting yspeed to -4
						grav_counter = 0; //reset gravity wait counter when jumping, gives consistent jump results

						move_sprite(32,112,0); //move button off screen (4 sprites in A button mega-sprite)
						move_sprite(33,112,0);
						move_sprite(34,120,0);
						move_sprite(35,120,0);

						play_sound_jump();
						wall_x = 232U; //reset all position to a consistent starter value at the beginning of the game
						current_state = PLAY; //move game state to main game, moving walls etc
					}
					is_pressed_J_A = 1; //note that we're holding the button down now
				}
				else //A button is not pressed
				{
					is_pressed_J_A = 0; //reset A button tracker
				}
				scroll_bkg(xspeed,0); //move background tiles left relative to current position
				animate_ghost(); //update ghost animation
				update_ghost_sprites(); //move ghost sprite position
				break;
			}
			case PLAY:
			{
				grav_counter++;//apply gravity to ghost every other tick
				if(grav_counter == 2) //update gravity every-other game update
				{
					grav_counter = 0; //reset gravity counter
					yspeed++; //add 1 to y speed (gravity value is effectively 1)
				}

				anim_counter++; //count gameplay frames that elapse.
				
				//Get and respond to joypad input
				counter = joypad();
				if(counter & J_A) //A button pressed
				{
					if(is_pressed_J_A == 0)//used to only respond to the inital press of the button, not to held buttons
					{
						yspeed = (INT8)-4; //if A pressed, jump by setting yspeed to -4
						grav_counter = 0; //reset gravity wait counter when jumping, gives consistent jump results
						play_sound_jump();
					}
					is_pressed_J_A = 1; //note that we're holding the button down now
				}
				else
				{
					is_pressed_J_A = 0; //A isn't pressed, make sure we properly set our state tracker for it
				}
				wall_x-=xspeed; //used to track walls shifting left
				update_wall_sprites();	//move wall sprites to their new position	
				scroll_bkg(xspeed,0); //move background tiles left relative to current position
				animate_ghost(); //update ghost sprite animation
				update_ghost_sprites(); //move ghost sprite position

				check_coll(); //test if ghost is colliding with wall

				break;
			}
			case GAME_OVER:
			{
				//slide to bottom of screen
				if(y < 141) //if current y is above the bottom of the screen (including the ghost sprite height)
					yspeed = 1; //counteract gravity by setting y speed to exactly 1, causing a slow slide down
				else
					yspeed = 0; //we're at the bottom of the screen, stop vertical movement
				update_ghost_sprites(); //update ghost position
				counter = joypad();
				if((counter & J_A)) //A pressed, so unless it's just held down from during game play, we'll move on and reset to the pre-play phase
				{
					if(is_pressed_J_A == 0) //A button not pressed last game update, so it's JUST been pressed down
					{
						current_state = RESET;
					}
					is_pressed_J_A = 1; //update the A button tracker so we don't blaze through the pre-play phase
				}
				else
				{
					is_pressed_J_A = 0; //reset A tracker
				}
				break;
			}
			case RESET: //reset all of our important values for the start of a new game
			{
				yspeed = 0;
				wall_rand=0;
				current_score = 0x00;
				anim_frame=0;
				anim_counter = 0;
				grav_counter = 0;
				current_score = 0;
				wall_x = 232U; //track wall location
				y = STARTY;
				reset_all_sprites();
				current_state = PRE_PLAY;
				break;
			}
			default:
				break;
		}
	}
}

void init_all_sprites()
{
	set_sprite_data(0, 12, ghost);//load tiles for ghost sprites
	set_sprite_data(12, 2, walls);//load tiles for wall sprites
	
	set_sprite_data(14, 20, score);//load tiles for score sprites
	set_sprite_data(34, 4, button);//load tiles for A button sprite
}

void reset_all_sprites()
{
	//Setup Ghost mega-sprite
	set_sprite_tile(0,0);//define and place all 4 sprites in the mega sprite
	move_sprite(0,STARTX,STARTY);
	set_sprite_tile(1,1);
	move_sprite(1,STARTX,STARTY+8);
	set_sprite_tile(2,2);
	move_sprite(2,STARTX+8,STARTY);
	set_sprite_tile(3,3);
	move_sprite(3,STARTX+8,STARTY+8);

	//make wall sprites
	for (counter = 4; counter < 24; counter++)
	{
		set_sprite_tile(counter, 12); 
	}

	//make wall capstone sprites
	for (counter = 24; counter < 28; counter++)
	{
		set_sprite_tile(counter, 13); 
	}

	//move wall sprites
	for (counter = 4; counter < 8; counter++) //upper half of wall, left column
	{
		if ((wall_rand+(counter-5)) > 1) //these if-else statements inside the for loops place the sprites in their correct normal place, or wrap around if off-screen
			move_sprite(counter, wall_x, (wall_rand+(counter-5)) << 3);
		else
			move_sprite(counter, wall_x, (wall_rand+(counter+13)) << 3);
	}
	for (counter = 8; counter < 12; counter++) //upper half of wall, right column
	{
		if((wall_rand+(counter-9)) > 1)
			move_sprite(counter, wall_x+8, (wall_rand+(counter-9)) << 3);
		else
			move_sprite(counter, wall_x+8, (wall_rand+(counter+9)) << 3);
	}
	for (counter = 12; counter < 18; counter++) //lower half of wall, left column
	{
		if(wall_rand+(counter-1) < 20)
			move_sprite(counter, wall_x, (wall_rand+(counter-1)) << 3);
		else
			move_sprite(counter, wall_x, (wall_rand+(counter-19)) << 3);
	}
	for (counter = 18; counter < 24; counter++) //lower half, right column
	{
		if(wall_rand+(counter-7) < 20)
			move_sprite(counter, wall_x+8, (wall_rand+(counter-7)) << 3);
		else
			move_sprite(counter, wall_x+8, (wall_rand+(counter-25)) << 3);
	}
	move_sprite(24, wall_x, (wall_rand+3) << 3); //move capstone tiles. way easier than the wall tiles because there's no off screen wrap condition
	move_sprite(25, wall_x+8, (wall_rand+3) << 3);
	move_sprite(26, wall_x, (wall_rand+10) << 3);
	move_sprite(27, wall_x+8, (wall_rand+10) << 3);

	set_sprite_tile(28, 14); //set right digit to 0
	set_sprite_tile(29, 15);
	move_sprite(28,SCORE_SPRITE_X+8,24); //place right digit in center
	move_sprite(29,SCORE_SPRITE_X+8,32);
	set_sprite_tile(30, 14);//set left digit to 0
	set_sprite_tile(31, 15);
	move_sprite(30,SCORE_SPRITE_X,0);//move left digit off screen for now
	move_sprite(31,SCORE_SPRITE_X,8);

	//set button sprites
	set_sprite_tile(32, 34);
	set_sprite_tile(33, 35);
	set_sprite_tile(34, 36);
	set_sprite_tile(35, 37);

	move_sprite(32,112,68); //move button
	move_sprite(33,112,76);
	move_sprite(34,120,68);
	move_sprite(35,120,76);
}

void update_ghost_sprites()
{
	y += yspeed; //update ghost y position based on current yspeed
	if(y < 16) //we don't want the ghost to be able to fly off the top of the screen, but it also isn't supposed to be game over
	{          //so we just limit the minimum height they can climb to
		y = 16;
		move_sprite(0,STARTX,y);
		set_sprite_tile(1,1);
		move_sprite(1,STARTX,y+8);
		set_sprite_tile(2,2);
		move_sprite(2,STARTX+8,y);
		set_sprite_tile(3,3);
		move_sprite(3,STARTX+8,y+8);
	}
	else //we're not in a special minimum-height edge case, so for all other cases, just update position normally
	{
		for(counter = 0; counter < 4; counter++)
		{
			scroll_sprite(counter,0,yspeed); //move all 4 ghost sprites relative to current position
		}
	}
}

void update_wall_sprites()
{
	switch(wall_x)
	{
		case 224U: //randomize wall each time it's at a specific column off screen
		{
			wall_rand = ((UINT8)rand() & 7); //calculate new random height offset. &7 limits range to sub-8
			//if (wall_rand > 5U) //limit random wall height value to 0 - 5
			//	wall_rand = 5U;	
			//wall_tiles_location = (SCX_REG >> 3U) + 27U; //
			wall_x -= (UINT8)(SCX_REG & 7U); //offset wall_x to align with a bkg tile column			
			for (counter = 4; counter < 8; counter++)//move wall sprites
			{
				if ((wall_rand+(counter-5)) > 1)
					move_sprite(counter, wall_x, (wall_rand+(counter-5)) << 3);
				else
					move_sprite(counter, wall_x, (wall_rand+(counter+13)) << 3);
			}
			for (counter = 8; counter < 12; counter++)
			{
				if((wall_rand+(counter-9)) > 1)
					move_sprite(counter, wall_x+8, (wall_rand+(counter-9)) << 3);
				else
					move_sprite(counter, wall_x+8, (wall_rand+(counter+9)) << 3);
			}
			for (counter = 12; counter < 18; counter++)
			{
				if(wall_rand+(counter-1) < 20)
					move_sprite(counter, wall_x, (wall_rand+(counter-1)) << 3);
				else
					move_sprite(counter, wall_x, (wall_rand+(counter-19)) << 3);
			}
			for (counter = 18; counter < 24; counter++)
			{
				if(wall_rand+(counter-7) < 20)
					move_sprite(counter, wall_x+8, (wall_rand+(counter-7)) << 3);
				else
					move_sprite(counter, wall_x+8, (wall_rand+(counter-25)) << 3);
			}
			move_sprite(24, wall_x, (wall_rand+3) << 3); //move capstone tiles
			move_sprite(25, wall_x+8, (wall_rand+3) << 3);
			move_sprite(26, wall_x, (wall_rand+10) << 3);
			move_sprite(27, wall_x+8, (wall_rand+10) << 3);
			break;
		}
		case 80: //wall_x is far enough left we need to add to the score.
		{
			current_score++; //add 1 to score
			if(current_score > 0x99) //limit score to 99
				current_score = 0x99;
			if((current_score & 0x0f) > 0x09) //BCD conversion for all values over 9
			{
				current_score+=6;
			}
			update_score(); //update score sprites now that the score changed
			break;
		}
		default:
			break;
	}
	//move wall sprites
	for(counter = 4; counter < 28; counter++)
	{
		//move wall each time it's off screen
		scroll_sprite(counter, -xspeed, 0); // move wall 2 pixels left
	}
}

void update_score()
{
	set_sprite_tile(28, 14 + ((current_score & 0x0f) << 1)); //set right hand digit tile to 0-9, baed on bcd score
	set_sprite_tile(29, 15 + ((current_score & 0x0f) << 1));
	if(current_score > 0x09) //if the score's over 9, set left hand digit to 0-9 based on bcd score
	{
		set_sprite_tile(30, 14 + ((current_score & 0xF0) >> 3));
		set_sprite_tile(31, 15 + ((current_score & 0xF0) >> 3));
		if(current_score == 0x10) //if score = exactly 10, we need to move the left digit from off screen to its place on screen
		{
			move_sprite(30,SCORE_SPRITE_X,24);
			move_sprite(31,SCORE_SPRITE_X,32);
		}
	}
}

void check_coll()
{
	//really janky collision detection
	//we check if the wall is in a range where we could touch
	//then we see if our y position is too low or too high
	//that's it


	if(wall_x < 96 && wall_x > 67) //left hand edge of wall is between 67 and 96.
	{                              //would be 64 but added 3 so the last 3 pixels on the left hand side of the ghost don't count
		if(((wall_rand+4 << 3) > y) || ((wall_rand+8 << 3) < y-3)) //there's an 8 tile gap in the wall. wall_rand, 4-11 tiles down, <<3 is because tiles are 8 tall
		{															//we're overlapping the walls horizontally, and not completely in the wall opening, so game over
			play_sound_crash();
			current_state = GAME_OVER;
		}

	}
	else if(y > 140) //regardless of our horizontal position, we're too far down and hit the floor, so game over
	{
		play_sound_crash();
		current_state = GAME_OVER;
	}
}

void animate_ghost()
{
	if(anim_counter > GHOST_ANIM_SPEED)  //if enough game play frames have elapsed, update ghost animation
	{
		anim_counter = 1; //reset frame counter
		if(anim_frame < 2) //loop through sprite frames
		{
			anim_frame++;
		}
		else
		{
			anim_frame = 0;
		}
		set_sprite_tile(0,0 + (anim_frame << 2)); //iterate the sprites according to the animation frame tracker. 
		set_sprite_tile(1,1 + (anim_frame << 2)); //there are 4 8x8 sprites in the ghost mega-sprite,
		set_sprite_tile(2,2 + (anim_frame << 2)); //so the "<< 2" is to multiply by four, accounting for that offset
		set_sprite_tile(3,3 + (anim_frame << 2));

	}
}

void play_sound_jump()
{
	//to play a sound effect, simply set the sound register values. the gb will output sound according to the register values

	//channel 1 and 4 register settings found through trial and error using the GBDK sound demo, updated by Zal0
	//The GBSOUND reference has a good breakdown of what each value means.
	//Channel 1 is a square wave channel, and channel 4 is a noise channel. I'm using them both to get a flap sound and rising jump sound
	
	//Sound channel 1 values
	NR10_REG = 0x76U;
	NR11_REG = 0x80U;
	NR12_REG = 0x53U;
	NR13_REG = 0xFAU;
	NR14_REG = 0x85U;
	
	//sound channel 4 values
	NR41_REG = 0x00U;
	NR42_REG = 0x31U;
	NR43_REG = 0x30U;
	NR44_REG = 0x80U;

}

void play_sound_crash()
{
	//to play a sound effect, simply set the sound register values. the gb will output sound according to the register values

	//channel 1 and 4 register settings found through trial and error using the GBDK sound demo, updated by Zal0
	//The GBSOUND reference has a good breakdown of what each value means.
	//Channel 1 is a square wave channel, and channel 4 is a noise channel.
	//Channel 1 here is used to get a lowering game over sound
	//Channel 4 is the initial crash sound for the collision with the wall/floor

	//set channel 1 register values
	NR10_REG = 0x7BU;
	NR11_REG = 0x41U;
	NR12_REG = 0x57U;
	NR13_REG = 0xDCU;
	NR14_REG = 0x85U;
	
	//set channel 4 register values
	NR41_REG = 0x10U;
	NR42_REG = 0x71U;
	NR43_REG = 0x61U;
	NR44_REG = 0xC0U;
}