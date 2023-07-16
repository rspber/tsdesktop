/*
  It's a modified version of Adafruit's glcdfont.c

  Copyright (c) 2022, rspber (https://github.com/rspber)

  More information in TSD_GFX.cpp
*/

// code page CP437

static const uint8_t default_font[] = {

  0x00, 0x00, 0x00, 0x00, 0x00, // 0x00 char
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E, // 0x01 char ☺
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E, // 0x02 char ☻
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C, // 0x03 char ♥
  0x18, 0x3C, 0x7E, 0x3C, 0x18, // 0x04 char ♦
  0x1C, 0x57, 0x7D, 0x57, 0x1C, // 0x05 char ♣
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C, // 0x06 char ♠
  0x00, 0x18, 0x3C, 0x18, 0x00, // 0x07 char •
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF, // 0x08 char ◘
  0x00, 0x18, 0x24, 0x18, 0x00, // 0x09 char   O
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF, // 0x0A char    \n
  0x30, 0x48, 0x3A, 0x06, 0x0E, // 0x0B char ♂
  0x26, 0x29, 0x79, 0x29, 0x26, // 0x0C char ♀
  0x40, 0x7F, 0x05, 0x05, 0x07, // 0x0D char    \r
  0x40, 0x7F, 0x05, 0x25, 0x3F, // 0x0E char ♫
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A, // 0x0F char ☼
  0x7F, 0x3E, 0x1C, 0x1C, 0x08, // 0x10 char ►
  0x08, 0x1C, 0x1C, 0x3E, 0x7F, // 0x11 char ◄
  0x14, 0x22, 0x7F, 0x22, 0x14, // 0x12 char ↕
  0x5F, 0x5F, 0x00, 0x5F, 0x5F, // 0x13 char ‼
  0x06, 0x09, 0x7F, 0x01, 0x7F, // 0x14 char ¶
  0x00, 0x66, 0x89, 0x95, 0x6A, // 0x15 char §
  0x60, 0x60, 0x60, 0x60, 0x60, // 0x16 char ▬
  0x94, 0xA2, 0xFF, 0xA2, 0x94, // 0x17 char ↨
  0x08, 0x04, 0x7E, 0x04, 0x08, // 0x18 char ↑
  0x10, 0x20, 0x7E, 0x20, 0x10, // 0x19 char ↓
  0x08, 0x08, 0x2A, 0x1C, 0x08, // 0x1A char →
  0x08, 0x1C, 0x2A, 0x08, 0x08, // 0x1B char ←
  0x1E, 0x10, 0x10, 0x10, 0x10, // 0x1C char ∟
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C, // 0x1D char ↔
  0x30, 0x38, 0x3E, 0x38, 0x30, // 0x1E char ▲
  0x06, 0x0E, 0x3E, 0x0E, 0x06, // 0x1F char ▼
  0x00, 0x00, 0x00, 0x00, 0x00, // 0x20 char
  0x00, 0x00, 0x5F, 0x00, 0x00, // 0x21 char !
  0x00, 0x07, 0x00, 0x07, 0x00, // 0x22 char "
  0x14, 0x7F, 0x14, 0x7F, 0x14, // 0x23 char #
  0x24, 0x2A, 0x7F, 0x2A, 0x12, // 0x24 char $
  0x23, 0x13, 0x08, 0x64, 0x62, // 0x25 char %
  0x36, 0x49, 0x56, 0x20, 0x50, // 0x26 char &
  0x00, 0x08, 0x07, 0x03, 0x00, // 0x27 char '
  0x00, 0x1C, 0x22, 0x41, 0x00, // 0x28 char (
  0x00, 0x41, 0x22, 0x1C, 0x00, // 0x29 char )
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A, // 0x2A char *
  0x08, 0x08, 0x3E, 0x08, 0x08, // 0x2B char +
  0x00, 0x80, 0x70, 0x30, 0x00, // 0x2C char ,
  0x08, 0x08, 0x08, 0x08, 0x08, // 0x2D char -
  0x00, 0x00, 0x60, 0x60, 0x00, // 0x2E char .
  0x20, 0x10, 0x08, 0x04, 0x02, // 0x2F char /
  0x3E, 0x51, 0x49, 0x45, 0x3E, // 0x30 char 0
  0x00, 0x42, 0x7F, 0x40, 0x00, // 0x31 char 1
  0x72, 0x49, 0x49, 0x49, 0x46, // 0x32 char 2
  0x21, 0x41, 0x49, 0x4D, 0x33, // 0x33 char 3
  0x18, 0x14, 0x12, 0x7F, 0x10, // 0x34 char 4
  0x27, 0x45, 0x45, 0x45, 0x39, // 0x35 char 5
  0x3C, 0x4A, 0x49, 0x49, 0x31, // 0x36 char 6
  0x41, 0x21, 0x11, 0x09, 0x07, // 0x37 char 7
  0x36, 0x49, 0x49, 0x49, 0x36, // 0x38 char 8
  0x46, 0x49, 0x49, 0x29, 0x1E, // 0x39 char 9
  0x00, 0x00, 0x14, 0x00, 0x00, // 0x3A char :
  0x00, 0x40, 0x34, 0x00, 0x00, // 0x3B char ;
  0x00, 0x08, 0x14, 0x22, 0x41, // 0x3C char <  Slash
  0x14, 0x14, 0x14, 0x14, 0x14, // 0x3D char =
  0x00, 0x41, 0x22, 0x14, 0x08, // 0x3E char >
  0x02, 0x01, 0x59, 0x09, 0x06, // 0x3F char ?
  0x3E, 0x41, 0x5D, 0x59, 0x4E, // 0x40 char @
  0x7C, 0x12, 0x11, 0x12, 0x7C, // 0x41 char A
  0x7F, 0x49, 0x49, 0x49, 0x36, // 0x42 char B
  0x3E, 0x41, 0x41, 0x41, 0x22, // 0x43 char C
  0x7F, 0x41, 0x41, 0x41, 0x3E, // 0x44 char D
  0x7F, 0x49, 0x49, 0x49, 0x41, // 0x45 char E
  0x7F, 0x09, 0x09, 0x09, 0x01, // 0x46 char F
  0x3E, 0x41, 0x41, 0x51, 0x73, // 0x47 char G
  0x7F, 0x08, 0x08, 0x08, 0x7F, // 0x48 char H
  0x00, 0x41, 0x7F, 0x41, 0x00, // 0x49 char I
  0x20, 0x40, 0x41, 0x3F, 0x01, // 0x4A char J
  0x7F, 0x08, 0x14, 0x22, 0x41, // 0x4B char K
  0x7F, 0x40, 0x40, 0x40, 0x40, // 0x4C char L
  0x7F, 0x02, 0x1C, 0x02, 0x7F, // 0x4D char M
  0x7F, 0x04, 0x08, 0x10, 0x7F, // 0x4E char N
  0x3E, 0x41, 0x41, 0x41, 0x3E, // 0x4F char O
  0x7F, 0x09, 0x09, 0x09, 0x06, // 0x50 char P
  0x3E, 0x41, 0x51, 0x21, 0x5E, // 0x51 char Q
  0x7F, 0x09, 0x19, 0x29, 0x46, // 0x52 char R
  0x26, 0x49, 0x49, 0x49, 0x32, // 0x53 char S
  0x03, 0x01, 0x7F, 0x01, 0x03, // 0x54 char T
  0x3F, 0x40, 0x40, 0x40, 0x3F, // 0x55 char U
  0x1F, 0x20, 0x40, 0x20, 0x1F, // 0x56 char V
  0x3F, 0x40, 0x38, 0x40, 0x3F, // 0x57 char W
  0x63, 0x14, 0x08, 0x14, 0x63, // 0x58 char X
  0x03, 0x04, 0x78, 0x04, 0x03, // 0x59 char Y
  0x61, 0x59, 0x49, 0x4D, 0x43, // 0x5A char Z
  0x00, 0x7F, 0x41, 0x41, 0x41, // 0x5B char [
  0x02, 0x04, 0x08, 0x10, 0x20, // 0x5C char \  BackSlash
  0x00, 0x41, 0x41, 0x41, 0x7F, // 0x5D char ]
  0x04, 0x02, 0x01, 0x02, 0x04, // 0x5E char ^
  0x40, 0x40, 0x40, 0x40, 0x40, // 0x5F char _
  0x00, 0x03, 0x07, 0x08, 0x00, // 0x60 char `
  0x20, 0x54, 0x54, 0x78, 0x40, // 0x61 char a
  0x7F, 0x28, 0x44, 0x44, 0x38, // 0x62 char b
  0x38, 0x44, 0x44, 0x44, 0x28, // 0x63 char c
  0x38, 0x44, 0x44, 0x28, 0x7F, // 0x64 char d
  0x38, 0x54, 0x54, 0x54, 0x18, // 0x65 char e
  0x00, 0x08, 0x7E, 0x09, 0x02, // 0x66 char f
  0x18, 0xA4, 0xA4, 0x9C, 0x78, // 0x67 char g
  0x7F, 0x08, 0x04, 0x04, 0x78, // 0x68 char h
  0x00, 0x44, 0x7D, 0x40, 0x00, // 0x69 char i
  0x20, 0x40, 0x40, 0x3D, 0x00, // 0x6A char j
  0x7F, 0x10, 0x28, 0x44, 0x00, // 0x6B char k
  0x00, 0x41, 0x7F, 0x40, 0x00, // 0x6C char l
  0x7C, 0x04, 0x78, 0x04, 0x78, // 0x6D char m
  0x7C, 0x08, 0x04, 0x04, 0x78, // 0x6E char n
  0x38, 0x44, 0x44, 0x44, 0x38, // 0x6F char o
  0xFC, 0x18, 0x24, 0x24, 0x18, // 0x70 char p
  0x18, 0x24, 0x24, 0x18, 0xFC, // 0x71 char q
  0x7C, 0x08, 0x04, 0x04, 0x08, // 0x72 char r
  0x48, 0x54, 0x54, 0x54, 0x24, // 0x73 char s
  0x04, 0x04, 0x3F, 0x44, 0x24, // 0x74 char t
  0x3C, 0x40, 0x40, 0x20, 0x7C, // 0x75 char u
  0x1C, 0x20, 0x40, 0x20, 0x1C, // 0x76 char v
  0x3C, 0x40, 0x30, 0x40, 0x3C, // 0x77 char w
  0x44, 0x28, 0x10, 0x28, 0x44, // 0x78 char x
  0x4C, 0x90, 0x90, 0x90, 0x7C, // 0x79 char y
  0x44, 0x64, 0x54, 0x4C, 0x44, // 0x7A char z
  0x00, 0x08, 0x36, 0x41, 0x00, // 0x7B char {
  0x00, 0x00, 0x77, 0x00, 0x00, // 0x7C char |
  0x00, 0x41, 0x36, 0x08, 0x00, // 0x7D char }
  0x02, 0x01, 0x02, 0x04, 0x02, // 0x7E char ~
  0x3C, 0x26, 0x23, 0x26, 0x3C, // 0x7F char ⌂  DEL
  0x1E, 0xA1, 0xA1, 0x61, 0x12, // 0x80 char Ç
  0x3A, 0x40, 0x40, 0x20, 0x7A, // 0x81 char ü
  0x38, 0x54, 0x54, 0x55, 0x59, // 0x82 char é
  0x21, 0x55, 0x55, 0x79, 0x41, // 0x83 char â
  0x22, 0x54, 0x54, 0x78, 0x42, // 0x84 char ä  a-umlaut
  0x21, 0x55, 0x54, 0x78, 0x40, // 0x85 char à
  0x20, 0x54, 0x55, 0x79, 0x40, // 0x86 char å
  0x0C, 0x1E, 0x52, 0x72, 0x12, // 0x87 char ç
  0x39, 0x55, 0x55, 0x55, 0x59, // 0x88 char ê
  0x39, 0x54, 0x54, 0x54, 0x59, // 0x89 char ë
  0x39, 0x55, 0x54, 0x54, 0x58, // 0x8A char è
  0x00, 0x00, 0x45, 0x7C, 0x41, // 0x8B char ï
  0x00, 0x02, 0x45, 0x7D, 0x42, // 0x8C char î
  0x00, 0x01, 0x45, 0x7C, 0x40, // 0x8D char ì
  0x7D, 0x12, 0x11, 0x12, 0x7D, // 0x8E char Ä  A-umlaut
  0xF0, 0x28, 0x25, 0x28, 0xF0, // 0x8F char Å
  0x7C, 0x54, 0x55, 0x45, 0x00, // 0x90 char É
  0x20, 0x54, 0x54, 0x7C, 0x54, // 0x91 char æ
  0x7C, 0x0A, 0x09, 0x7F, 0x49, // 0x92 char Æ
  0x32, 0x49, 0x49, 0x49, 0x32, // 0x93 char ô
  0x3A, 0x44, 0x44, 0x44, 0x3A, // 0x94 char ö  o-umlaut
  0x32, 0x4A, 0x48, 0x48, 0x30, // 0x95 char ò
  0x3A, 0x41, 0x41, 0x21, 0x7A, // 0x96 char û
  0x3A, 0x42, 0x40, 0x20, 0x78, // 0x97 char ù
  0x00, 0x9D, 0xA0, 0xA0, 0x7D, // 0x98 char ÿ
  0x3D, 0x42, 0x42, 0x42, 0x3D, // 0x99 char Ö  O-umlaut
  0x3D, 0x40, 0x40, 0x40, 0x3D, // 0x9A char Ü
  0x3C, 0x24, 0xFF, 0x24, 0x24, // 0x9B char ¢
  0x48, 0x7E, 0x49, 0x43, 0x66, // 0x9C char £
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B, // 0x9D char ¥
  0xFF, 0x09, 0x29, 0xF6, 0x20, // 0x9E char ₧
  0xC0, 0x88, 0x7E, 0x09, 0x03, // 0x9F char ƒ
  0x20, 0x54, 0x54, 0x79, 0x41, // 0xA0 char á
  0x00, 0x00, 0x44, 0x7D, 0x41, // 0xA1 char í
  0x30, 0x48, 0x48, 0x4A, 0x32, // 0xA2 char ó
  0x38, 0x40, 0x40, 0x22, 0x7A, // 0xA3 char ú
  0x00, 0x7A, 0x0A, 0x0A, 0x72, // 0xA4 char ñ
  0x7D, 0x0D, 0x19, 0x31, 0x7D, // 0xA5 char Ñ
  0x26, 0x29, 0x29, 0x2F, 0x28, // 0xA6 char ª
  0x26, 0x29, 0x29, 0x29, 0x26, // 0xA7 char º
  0x30, 0x48, 0x4D, 0x40, 0x20, // 0xA8 char ¿
  0x38, 0x08, 0x08, 0x08, 0x08, // 0xA9 char ⌐
  0x08, 0x08, 0x08, 0x08, 0x38, // 0xAA char ¬
  0x2F, 0x10, 0xC8, 0xAC, 0xBA, // 0xAB char ½
  0x2F, 0x10, 0x28, 0x34, 0xFA, // 0xAC char ¼
  0x00, 0x00, 0x7B, 0x00, 0x00, // 0xAD char ¡
  0x08, 0x14, 0x2A, 0x14, 0x22, // 0xAE char «
  0x22, 0x14, 0x2A, 0x14, 0x08, // 0xAF char »
  0x55, 0x00, 0x55, 0x00, 0x55, // 0xB0 char ░  25% block #176 missing in old code
  0xAA, 0x55, 0xAA, 0x55, 0xAA, // 0xB1 char ▒  50% block
  0xFF, 0x55, 0xFF, 0x55, 0xFF, // 0xB2 char ▓  75% block
  0x00, 0x00, 0x00, 0xFF, 0x00, // 0xB3 char │
  0x10, 0x10, 0x10, 0xFF, 0x00, // 0xB4 char ┤
  0x14, 0x14, 0x14, 0xFF, 0x00, // 0xB5 char ╡
  0x10, 0x10, 0xFF, 0x00, 0xFF, // 0xB6 char ╢
  0x10, 0x10, 0xF0, 0x10, 0xF0, // 0xB7 char ╖
  0x14, 0x14, 0x14, 0xFC, 0x00, // 0xB8 char ╕
  0x14, 0x14, 0xF7, 0x00, 0xFF, // 0xB9 char ╣
  0x00, 0x00, 0xFF, 0x00, 0xFF, // 0xBA char ║
  0x14, 0x14, 0xF4, 0x04, 0xFC, // 0xBB char ╗
  0x14, 0x14, 0x17, 0x10, 0x1F, // 0xBC char ╝
  0x10, 0x10, 0x1F, 0x10, 0x1F, // 0xBD char ╜
  0x14, 0x14, 0x14, 0x1F, 0x00, // 0xBE char ╛
  0x10, 0x10, 0x10, 0xF0, 0x00, // 0xBF char ┐
  0x00, 0x00, 0x00, 0x1F, 0x10, // 0xC0 char └
  0x10, 0x10, 0x10, 0x1F, 0x10, // 0xC1 char ┴
  0x10, 0x10, 0x10, 0xF0, 0x10, // 0xC2 char ┬
  0x00, 0x00, 0x00, 0xFF, 0x10, // 0xC3 char ├
  0x10, 0x10, 0x10, 0x10, 0x10, // 0xC4 char ─
  0x10, 0x10, 0x10, 0xFF, 0x10, // 0xC5 char ┼
  0x00, 0x00, 0x00, 0xFF, 0x14, // 0xC6 char ╞
  0x00, 0x00, 0xFF, 0x00, 0xFF, // 0xC7 char ╟
  0x00, 0x00, 0x1F, 0x10, 0x17, // 0xC8 char ╚
  0x00, 0x00, 0xFC, 0x04, 0xF4, // 0xC9 char ╔
  0x14, 0x14, 0x17, 0x10, 0x17, // 0xCA char ╩
  0x14, 0x14, 0xF4, 0x04, 0xF4, // 0xCB char ╦
  0x00, 0x00, 0xFF, 0x00, 0xF7, // 0xCC char ╠
  0x14, 0x14, 0x14, 0x14, 0x14, // 0xCD char ═
  0x14, 0x14, 0xF7, 0x00, 0xF7, // 0xCE char ╬
  0x14, 0x14, 0x14, 0x17, 0x14, // 0xCF char ╧
  0x10, 0x10, 0x1F, 0x10, 0x1F, // 0xD0 char ╨
  0x14, 0x14, 0x14, 0xF4, 0x14, // 0xD1 char ╤
  0x10, 0x10, 0xF0, 0x10, 0xF0, // 0xD2 char ╥
  0x00, 0x00, 0x1F, 0x10, 0x1F, // 0xD3 char ╙
  0x00, 0x00, 0x00, 0x1F, 0x14, // 0xD4 char ╘
  0x00, 0x00, 0x00, 0xFC, 0x14, // 0xD5 char ╒
  0x00, 0x00, 0xF0, 0x10, 0xF0, // 0xD6 char ╓
  0x10, 0x10, 0xFF, 0x10, 0xFF, // 0xD7 char ╫
  0x14, 0x14, 0x14, 0xFF, 0x14, // 0xD8 char ╪
  0x10, 0x10, 0x10, 0x1F, 0x00, // 0xD9 char ┘
  0x00, 0x00, 0x00, 0xF0, 0x10, // 0xDA char ┌
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xDB char █
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, // 0xDC char ▄
  0xFF, 0xFF, 0xFF, 0x00, 0x00, // 0xDD char ▌
  0x00, 0x00, 0x00, 0xFF, 0xFF, // 0xDE char ▐
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F, // 0xDF char ▀
  0x38, 0x44, 0x44, 0x38, 0x44, // 0xE0 char α
  0xFC, 0x4A, 0x4A, 0x4A, 0x34, // 0xE1 char ß  sharp-s or beta
  0x7E, 0x02, 0x02, 0x06, 0x06, // 0xE2 char Γ
  0x02, 0x7E, 0x02, 0x7E, 0x02, // 0xE3 char π
  0x63, 0x55, 0x49, 0x41, 0x63, // 0xE4 char Σ
  0x38, 0x44, 0x44, 0x3C, 0x04, // 0xE5 char σ
  0x40, 0x7E, 0x20, 0x1E, 0x20, // 0xE6 char µ
  0x06, 0x02, 0x7E, 0x02, 0x02, // 0xE7 char τ
  0x99, 0xA5, 0xE7, 0xA5, 0x99, // 0xE8 char Φ
  0x1C, 0x2A, 0x49, 0x2A, 0x1C, // 0xE9 char Θ
  0x4C, 0x72, 0x01, 0x72, 0x4C, // 0xEA char Ω
  0x30, 0x4A, 0x4D, 0x4D, 0x30, // 0xEB char δ
  0x30, 0x48, 0x78, 0x48, 0x30, // 0xEC char ∞
  0xBC, 0x62, 0x5A, 0x46, 0x3D, // 0xED char φ
  0x3E, 0x49, 0x49, 0x49, 0x00, // 0xEE char ε
  0x7E, 0x01, 0x01, 0x01, 0x7E, // 0xEF char ∩
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A, // 0xF0 char ≡
  0x44, 0x44, 0x5F, 0x44, 0x44, // 0xF1 char ±
  0x40, 0x51, 0x4A, 0x44, 0x40, // 0xF2 char ≥
  0x40, 0x44, 0x4A, 0x51, 0x40, // 0xF3 char ≤
  0x00, 0x00, 0xFF, 0x01, 0x03, // 0xF4 char ⌠
  0xE0, 0x80, 0xFF, 0x00, 0x00, // 0xF5 char ⌡
  0x08, 0x08, 0x6B, 0x6B, 0x08, // 0xF6 char ÷
  0x36, 0x12, 0x36, 0x24, 0x36, // 0xF7 char ≈
  0x06, 0x0F, 0x09, 0x0F, 0x06, // 0xF8 char °
  0x00, 0x00, 0x18, 0x18, 0x00, // 0xF9 char ∙
  0x00, 0x00, 0x10, 0x10, 0x00, // 0xFA char ·
  0x30, 0x40, 0xFF, 0x01, 0x01, // 0xFB char √
  0x00, 0x1F, 0x01, 0x01, 0x1E, // 0xFC char ⁿ
  0x00, 0x19, 0x1D, 0x17, 0x12, // 0xFD char ²
  0x00, 0x3C, 0x3C, 0x3C, 0x3C, // 0xFE char ■
  0x00, 0x00, 0x00, 0x00, 0x00  // 0xFF char    #255 NBSP
};
