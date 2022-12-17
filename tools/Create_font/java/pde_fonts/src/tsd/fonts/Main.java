/*
  pde_fonts

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*

1. Install java 11

2. Install eclipse

3. Copy pde_fonts project to eclipse workspace

4. Download the 4.0.1 version of processing, maybe others will as well

5. Target the following jars: 
 
  core.jar, flatlaf.jar, gluegen-rt.jar, jna.jar, jna-platform.jar, jogl-all.jar, pde.jar

  and place them in the project jars folder
 
6. In eclipse you should see:

  Project / Properties
    Java Build Path
      Source
        > pde_fonts/src
      Libraries
        Modulepath
          > JRE System Library [JRE]
        Classpath
          > core.jar - pde_fonts/jars
          > flatlaf.jar - pde_fonts/jars
          > gluegen-rt.jar - pde_fonts/jars
          > jna.jar - pde_fonts/jars   
          > jna-platform.jar - pde_fonts/jars   
          > jogl-all.jar - pde_fonts/jars   
          > pde.jar - pde_fonts/jars   

 - none file in red color

If you don't like working with jars, here are the sources:
https://github.com/ssilverman/processing (but in 3. version)

*/

package tsd.fonts;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PFont.Glyph;
import processing.core.PGraphics;
import processing.core.PImage;

class PDE extends PGraphics {
	
	public PFont getFont(String fontPathName, int fontSize, boolean smooth, char[] charset) {
		return createFont(fontPathName, fontSize, smooth, charset); //<>//
	}	
}

class ToFile {

	final Writer writer = new Writer();
	final Writer footer = new Writer();

	final String fontName;
	final int fontHeight;
	
	public ToFile(final String fontName, final int fontHeight)
	{
		this.fontName = fontName;
		this.fontHeight = fontHeight;
	}
	
	// ----------------------------------------------------------------------------
	int proc_char(Glyph glyph, short z, short w, short c) throws IOException
	{
	  PImage image = glyph.image;
	  byte[] gphbuf = new byte[image.pixelHeight * image.pixelWidth / 8 + 1 ];
	  int ngph = 0;
	  int j = 0;
	  byte sum = 0;
	  short bit = 0x80;
	  for (int y = 0; y < image.pixelHeight; y++) {
	    for (int x = 0; x < image.pixelWidth; x++) {
	      if (image.pixels[j++] >= 0x87)
		    sum |= bit;
		  if ((bit >>= 1) == 0) {
		    gphbuf[ngph++] = sum;
		    sum = 0;
		    bit = 0x80;
		  }
	    }
	  }
	  int n = 7;
	  while (ngph < gphbuf.length) {
    	  if ((n >>= 1) != 0)
    		  sum |= bit;
    	  if ((bit >>= 1) == 0) {
    		  gphbuf[ngph++] = sum;
    		  sum = 0;
    		  bit = 0x80;
    	  }
	  }
	//      GFXglyph t = new GFXglyph(); 
	  short code = c;
	  // mode 0
	  // mode 1
	  byte width = (byte)image.width;
	  byte height = (byte)image.height;
	  byte xAdvance = (byte)glyph.setWidth;
	  byte xOffset = image.pixels.length == 0 || image.pixels.length == 1 && image.pixels[0] == 0 ? 0 : (byte)glyph.leftExtent;
	  byte yOffset = image.pixels.length == 0 || image.pixels.length == 1 && image.pixels[0] == 0 ? 0 : (byte)glyph.topExtent;
	  byte len0 = (byte)(ngph % 0x100);
	  byte len1 = (byte)(ngph / 0x100);
	  writer.write("  0x%02X, 0x%02X, 0x%02X, %d, %d, %d, 0x%02x, 0x%02X, %d, %d, ", code, 0, 0, width, height, xAdvance, xOffset, yOffset, len0, len1);
	    
	  j = 9;
	  for (int i = 0; i < ngph; ++i) {
		writer.write("0x%02X, ", gphbuf[i]);
	    if (++j > 15) {
	    	writer.write("\n    ");
	    	j = 0;
	    }
	  }
	  if (j > 14 - (z != 0 ? 1 : 0) - (w != 0 ? 2 : 0) ) {
    	writer.write("\n    ");
	  }

	  if (z == 0)
		  if (w == 0)
			  writer.write("// '%c'", c != 0 && c != 9 && c != 10 && c != 13 ? c : 'o');
		  else
			  writer.write("// '%c' 0x%02X, 0x%02X,", glyph.value, w, c);
	  else
		  writer.write("// '%c' 0x%02X, 0x%02X, 0x%02X,", glyph.value, z, w, c);
	  writer.write("\n");
	
	//  int len = (bitmap->width * bitmap->rows + 7) / 8;
	  return 10 + ngph;
	}

	public int decodeUnicode( short[] buf, final int code )
	{
		if (code >= 0x10000) {
			return 0;
		}
		if (code >= 0x2000) {	// 3 byte utf-8
			buf[0] = (short)(0xE0 + ((code >> 12) & 0x0f));
			buf[1] = (short)(0x80 + ((code >> 6) & 0x3f));
			buf[2] = (short)(0x80 + (code & 0x3f));
			return 3;
		}
		if (code >= 0x0100) {	// 2 byte utf-8
			buf[0] = (short)(0xC0 + ((code >> 6) & 0x1f));
			buf[1] = (short)(0x80 + (code & 0x3f));
			return 2;
		}
		buf[0] = (short)code;
		return 1;
	}

	short z = 0, w = 0, fr = 0x20, to = 0x7e;
	String fract = null;

	private int page_start()
	{
		writer.write("static const uint8_t %s_Glyphs_%s[] {\n", fontName, fract);
	    footer.write("  &%s_%s,\n", fontName, fract);
	    return 4 + 6 + 4;
	}

	private void page_end()
	{
		writer.write("0};\n\n");

		writer.write("static const GFXfont %s_%s {\n", fontName, fract);
		writer.write("  %s_Glyphs_%s,\n", fontName, fract);
		int c1 = z > 0 ? z : w;
		int c2 = z > 0 ? w : 0;
		writer.write("  0x%x, 0x%x, 0, 0x%02X, 0x%02X, %d\n};\n\n", c1, c2, fr, to, fontHeight);
	}

	public void run(PFont font) throws IOException
	{
		footer.write("static const GFXfont* %s[] {\n", fontName);
        
		short[] buf = new short[8];
		
		int totsize = 0;
		int len0 = 0;
		for (int i = 0; i < font.getGlyphCount(); ++i) {
			Glyph glyph = font.getGlyph(i);
			int len = decodeUnicode(buf, glyph.value);
			if (len < 1) {
				break;
			}
			if (len > len0) {
				if (len0 > 0) {
					page_end();
				}
				if (len == 1) {
					z = 0;
					w = 0;
					fr = 0x20;
					to = (short)(fr + 0x1f);
					fract = String.format("%02X", fr);
				}
				else
				if (len == 2) {
					z = 0;
					w = buf[0];
					fract = String.format("%02X", w);
					fr = 0x80;
					to = (short)(fr + 0x3f);
				} else
				if (len == 3) {
					z = buf[0];
					w = buf[1];
					fract = String.format("%02X%02X", z, w);
					fr = 0x80;
					to = (short)(fr + 0x3f);
				}
				totsize += page_start();
				len0 = len;
			}
			boolean chg = false;
			if (len == 1) {
				if (buf[0] > to) {
					page_end();
					fr = buf[0];
					to = (short)(fr + 0x1f);
					fract = String.format("%02X", fr);
					chg = true;
				}
			}
			else
			if (len == 2) {
				if (buf[0] > w) {
					page_end();
					w = buf[0];
					fract = String.format("%02X", w);
					chg = true;
				}
			}
			else
			if (len == 3) {
				if (buf[1] > w) {
					page_end();
					w = buf[1];
					chg = true;
				}
				else {
					if (buf[0] > z) {
						page_end();
						z = buf[0];
						w = buf[1];
						chg = true;
					}
				}
				if (chg) {
					fract = String.format("%02X%02X", z, w);
				}
			}
			if (chg) {
				totsize += page_start();
			}
		    totsize += proc_char(glyph, z, w, buf[len-1]);
		}
		if (len0 > 0) {
			page_end();
		}
	    footer.write("\n0};\n\n");
		
	}

	
}

public class Main {

// Below's table of unicodes is copied from:
	
// https://github.com/Bodmer/TFT_eSPI/Tools/Create_Smooth_Font/Create_font/Create_font.pde

/*
Software License Agreement (FreeBSD License)
 
 Copyright (c) 2018 Bodmer (https://github.com/Bodmer)
 
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Next we specify which unicode blocks from the the Basic Multilingual Plane (BMP) are included in the final font file. //
	// Note: The ttf/otf font file MAY NOT contain all possible Unicode characters, refer to the fonts online documentation. //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static final int[] unicodeBlocks = {
	  // The list below has been created from the table here: https://en.wikipedia.org/wiki/Unicode_block
	  // Remove // at start of lines below to include that unicode block, different code ranges can also be specified by
	  // editing the start and end-of-range values. Multiple lines from the list below can be included, limited only by
	  // the final font file size!

	  // Block range,   //Block name, Code points, Assigned characters, Scripts
	  // First, last,   //Range is inclusive of first and last codes
	  0x0020, 0x007E, //Basic Latin, 128, 128, Latin (52 characters), Common (76 characters)
	  //0x0080, 0x00FF, //Latin-1 Supplement, 128, 128, Latin (64 characters), Common (64 characters)
	  0x0100, 0x017F, //Latin Extended-A, 128, 128, Latin
	  //0x0180, 0x024F, //Latin Extended-B, 208, 208, Latin
	  //0x0250, 0x02AF, //IPA Extensions, 96, 96, Latin
	  //0x02B0, 0x02FF, //Spacing Modifier Letters, 80, 80, Bopomofo (2 characters), Latin (14 characters), Common (64 characters)
	  //0x0300, 0x036F, //Combining Diacritical Marks, 112, 112, Inherited
	  //0x0370, 0x03FF, //Greek and Coptic, 144, 135, Coptic (14 characters), Greek (117 characters), Common (4 characters)
	  //0x0400, 0x04FF, //Cyrillic, 256, 256, Cyrillic (254 characters), Inherited (2 characters)
	  //0x0500, 0x052F, //Cyrillic Supplement, 48, 48, Cyrillic
	  //0x0530, 0x058F, //Armenian, 96, 89, Armenian (88 characters), Common (1 character)
	  //0x0590, 0x05FF, //Hebrew, 112, 87, Hebrew
	  //0x0600, 0x06FF, //Arabic, 256, 255, Arabic (237 characters), Common (6 characters), Inherited (12 characters)
	  //0x0700, 0x074F, //Syriac, 80, 77, Syriac
	  //0x0750, 0x077F, //Arabic Supplement, 48, 48, Arabic
	  //0x0780, 0x07BF, //Thaana, 64, 50, Thaana
	  //0x07C0, 0x07FF, //NKo, 64, 59, Nko
	  //0x0800, 0x083F, //Samaritan, 64, 61, Samaritan
	  //0x0840, 0x085F, //Mandaic, 32, 29, Mandaic
	  //0x0860, 0x086F, //Syriac Supplement, 16, 11, Syriac
	  //0x08A0, 0x08FF, //Arabic Extended-A, 96, 73, Arabic (72 characters), Common (1 character)
	  //0x0900, 0x097F, //Devanagari, 128, 128, Devanagari (124 characters), Common (2 characters), Inherited (2 characters)
	  //0x0980, 0x09FF, //Bengali, 128, 95, Bengali
	  //0x0A00, 0x0A7F, //Gurmukhi, 128, 79, Gurmukhi
	  //0x0A80, 0x0AFF, //Gujarati, 128, 91, Gujarati
	  //0x0B00, 0x0B7F, //Oriya, 128, 90, Oriya
	  //0x0B80, 0x0BFF, //Tamil, 128, 72, Tamil
	  //0x0C00, 0x0C7F, //Telugu, 128, 96, Telugu
	  //0x0C80, 0x0CFF, //Kannada, 128, 88, Kannada
	  //0x0D00, 0x0D7F, //Malayalam, 128, 117, Malayalam
	  //0x0D80, 0x0DFF, //Sinhala, 128, 90, Sinhala
	  //0x0E00, 0x0E7F, //Thai, 128, 87, Thai (86 characters), Common (1 character)
	  //0x0E80, 0x0EFF, //Lao, 128, 67, Lao
	  //0x0F00, 0x0FFF, //Tibetan, 256, 211, Tibetan (207 characters), Common (4 characters)
	  //0x1000, 0x109F, //Myanmar, 160, 160, Myanmar
	  //0x10A0, 0x10FF, //Georgian, 96, 88, Georgian (87 characters), Common (1 character)
	  //0x1100, 0x11FF, //Hangul Jamo, 256, 256, Hangul
	  //0x1200, 0x137F, //Ethiopic, 384, 358, Ethiopic
	  //0x1380, 0x139F, //Ethiopic Supplement, 32, 26, Ethiopic
	  //0x13A0, 0x13FF, //Cherokee, 96, 92, Cherokee
	  //0x1400, 0x167F, //Unified Canadian Aboriginal Syllabics, 640, 640, Canadian Aboriginal
	  //0x1680, 0x169F, //Ogham, 32, 29, Ogham
	  //0x16A0, 0x16FF, //Runic, 96, 89, Runic (86 characters), Common (3 characters)
	  //0x1700, 0x171F, //Tagalog, 32, 20, Tagalog
	  //0x1720, 0x173F, //Hanunoo, 32, 23, Hanunoo (21 characters), Common (2 characters)
	  //0x1740, 0x175F, //Buhid, 32, 20, Buhid
	  //0x1760, 0x177F, //Tagbanwa, 32, 18, Tagbanwa
	  //0x1780, 0x17FF, //Khmer, 128, 114, Khmer
	  //0x1800, 0x18AF, //Mongolian, 176, 156, Mongolian (153 characters), Common (3 characters)
	  //0x18B0, 0x18FF, //Unified Canadian Aboriginal Syllabics Extended, 80, 70, Canadian Aboriginal
	  //0x1900, 0x194F, //Limbu, 80, 68, Limbu
	  //0x1950, 0x197F, //Tai Le, 48, 35, Tai Le
	  //0x1980, 0x19DF, //New Tai Lue, 96, 83, New Tai Lue
	  //0x19E0, 0x19FF, //Khmer Symbols, 32, 32, Khmer
	  //0x1A00, 0x1A1F, //Buginese, 32, 30, Buginese
	  //0x1A20, 0x1AAF, //Tai Tham, 144, 127, Tai Tham
	  //0x1AB0, 0x1AFF, //Combining Diacritical Marks Extended, 80, 15, Inherited
	  //0x1B00, 0x1B7F, //Balinese, 128, 121, Balinese
	  //0x1B80, 0x1BBF, //Sundanese, 64, 64, Sundanese
	  //0x1BC0, 0x1BFF, //Batak, 64, 56, Batak
	  //0x1C00, 0x1C4F, //Lepcha, 80, 74, Lepcha
	  //0x1C50, 0x1C7F, //Ol Chiki, 48, 48, Ol Chiki
	  //0x1C80, 0x1C8F, //Cyrillic Extended-C, 16, 9, Cyrillic
	  //0x1CC0, 0x1CCF, //Sundanese Supplement, 16, 8, Sundanese
	  //0x1CD0, 0x1CFF, //Vedic Extensions, 48, 42, Common (15 characters), Inherited (27 characters)
	  //0x1D00, 0x1D7F, //Phonetic Extensions, 128, 128, Cyrillic (2 characters), Greek (15 characters), Latin (111 characters)
	  //0x1D80, 0x1DBF, //Phonetic Extensions Supplement, 64, 64, Greek (1 character), Latin (63 characters)
	  //0x1DC0, 0x1DFF, //Combining Diacritical Marks Supplement, 64, 63, Inherited
	  //0x1E00, 0x1EFF, //Latin Extended Additional, 256, 256, Latin
	  //0x1F00, 0x1FFF, //Greek Extended, 256, 233, Greek
	  //0x2000, 0x206F, //General Punctuation, 112, 111, Common (109 characters), Inherited (2 characters)
	  //0x2070, 0x209F, //Superscripts and Subscripts, 48, 42, Latin (15 characters), Common (27 characters)
	  //0x20A0, 0x20CF, //Currency Symbols, 48, 32, Common
	  //0x20D0, 0x20FF, //Combining Diacritical Marks for Symbols, 48, 33, Inherited
	  //0x2100, 0x214F, //Letterlike Symbols, 80, 80, Greek (1 character), Latin (4 characters), Common (75 characters)
	  //0x2150, 0x218F, //Number Forms, 64, 60, Latin (41 characters), Common (19 characters)
	  //0x2190, 0x21FF, //Arrows, 112, 112, Common
	  //0x2200, 0x22FF, //Mathematical Operators, 256, 256, Common
	  //0x2300, 0x23FF, //Miscellaneous Technical, 256, 256, Common
	  //0x2400, 0x243F, //Control Pictures, 64, 39, Common
	  //0x2440, 0x245F, //Optical Character Recognition, 32, 11, Common
	  //0x2460, 0x24FF, //Enclosed Alphanumerics, 160, 160, Common
	  //0x2500, 0x257F, //Box Drawing, 128, 128, Common
	  //0x2580, 0x259F, //Block Elements, 32, 32, Common
	  //0x25A0, 0x25FF, //Geometric Shapes, 96, 96, Common
	  //0x2600, 0x26FF, //Miscellaneous Symbols, 256, 256, Common
	  //0x2700, 0x27BF, //Dingbats, 192, 192, Common
	  //0x27C0, 0x27EF, //Miscellaneous Mathematical Symbols-A, 48, 48, Common
	  //0x27F0, 0x27FF, //Supplemental Arrows-A, 16, 16, Common
	  //0x2800, 0x28FF, //Braille Patterns, 256, 256, Braille
	  //0x2900, 0x297F, //Supplemental Arrows-B, 128, 128, Common
	  //0x2980, 0x29FF, //Miscellaneous Mathematical Symbols-B, 128, 128, Common
	  //0x2A00, 0x2AFF, //Supplemental Mathematical Operators, 256, 256, Common
	  //0x2B00, 0x2BFF, //Miscellaneous Symbols and Arrows, 256, 207, Common
	  //0x2C00, 0x2C5F, //Glagolitic, 96, 94, Glagolitic
	  //0x2C60, 0x2C7F, //Latin Extended-C, 32, 32, Latin
	  //0x2C80, 0x2CFF, //Coptic, 128, 123, Coptic
	  //0x2D00, 0x2D2F, //Georgian Supplement, 48, 40, Georgian
	  //0x2D30, 0x2D7F, //Tifinagh, 80, 59, Tifinagh
	  //0x2D80, 0x2DDF, //Ethiopic Extended, 96, 79, Ethiopic
	  //0x2DE0, 0x2DFF, //Cyrillic Extended-A, 32, 32, Cyrillic
	  //0x2E00, 0x2E7F, //Supplemental Punctuation, 128, 74, Common
	  //0x2E80, 0x2EFF, //CJK Radicals Supplement, 128, 115, Han
	  //0x2F00, 0x2FDF, //Kangxi Radicals, 224, 214, Han
	  //0x2FF0, 0x2FFF, //Ideographic Description Characters, 16, 12, Common
	  //0x3000, 0x303F, //CJK Symbols and Punctuation, 64, 64, Han (15 characters), Hangul (2 characters), Common (43 characters), Inherited (4 characters)
	  //0x3040, 0x309F, //Hiragana, 96, 93, Hiragana (89 characters), Common (2 characters), Inherited (2 characters)
	  //0x30A0, 0x30FF, //Katakana, 96, 96, Katakana (93 characters), Common (3 characters)
	  //0x3100, 0x312F, //Bopomofo, 48, 42, Bopomofo
	  //0x3130, 0x318F, //Hangul Compatibility Jamo, 96, 94, Hangul
	  //0x3190, 0x319F, //Kanbun, 16, 16, Common
	  //0x31A0, 0x31BF, //Bopomofo Extended, 32, 27, Bopomofo
	  //0x31C0, 0x31EF, //CJK Strokes, 48, 36, Common
	  //0x31F0, 0x31FF, //Katakana Phonetic Extensions, 16, 16, Katakana
	  //0x3200, 0x32FF, //Enclosed CJK Letters and Months, 256, 254, Hangul (62 characters), Katakana (47 characters), Common (145 characters)
	  //0x3300, 0x33FF, //CJK Compatibility, 256, 256, Katakana (88 characters), Common (168 characters)
	  //0x3400, 0x4DBF, //CJK Unified Ideographs Extension A, 6,592, 6,582, Han
	  //0x4DC0, 0x4DFF, //Yijing Hexagram Symbols, 64, 64, Common
	  //0x4E00, 0x9FFF, //CJK Unified Ideographs, 20,992, 20,971, Han
	  //0xA000, 0xA48F, //Yi Syllables, 1,168, 1,165, Yi
	  //0xA490, 0xA4CF, //Yi Radicals, 64, 55, Yi
	  //0xA4D0, 0xA4FF, //Lisu, 48, 48, Lisu
	  //0xA500, 0xA63F, //Vai, 320, 300, Vai
	  //0xA640, 0xA69F, //Cyrillic Extended-B, 96, 96, Cyrillic
	  //0xA6A0, 0xA6FF, //Bamum, 96, 88, Bamum
	  //0xA700, 0xA71F, //Modifier Tone Letters, 32, 32, Common
	  //0xA720, 0xA7FF, //Latin Extended-D, 224, 160, Latin (155 characters), Common (5 characters)
	  //0xA800, 0xA82F, //Syloti Nagri, 48, 44, Syloti Nagri
	  //0xA830, 0xA83F, //Common Indic Number Forms, 16, 10, Common
	  //0xA840, 0xA87F, //Phags-pa, 64, 56, Phags Pa
	  //0xA880, 0xA8DF, //Saurashtra, 96, 82, Saurashtra
	  //0xA8E0, 0xA8FF, //Devanagari Extended, 32, 30, Devanagari
	  //0xA900, 0xA92F, //Kayah Li, 48, 48, Kayah Li (47 characters), Common (1 character)
	  //0xA930, 0xA95F, //Rejang, 48, 37, Rejang
	  //0xA960, 0xA97F, //Hangul Jamo Extended-A, 32, 29, Hangul
	  //0xA980, 0xA9DF, //Javanese, 96, 91, Javanese (90 characters), Common (1 character)
	  //0xA9E0, 0xA9FF, //Myanmar Extended-B, 32, 31, Myanmar
	  //0xAA00, 0xAA5F, //Cham, 96, 83, Cham
	  //0xAA60, 0xAA7F, //Myanmar Extended-A, 32, 32, Myanmar
	  //0xAA80, 0xAADF, //Tai Viet, 96, 72, Tai Viet
	  //0xAAE0, 0xAAFF, //Meetei Mayek Extensions, 32, 23, Meetei Mayek
	  //0xAB00, 0xAB2F, //Ethiopic Extended-A, 48, 32, Ethiopic
	  //0xAB30, 0xAB6F, //Latin Extended-E, 64, 54, Latin (52 characters), Greek (1 character), Common (1 character)
	  //0xAB70, 0xABBF, //Cherokee Supplement, 80, 80, Cherokee
	  //0xABC0, 0xABFF, //Meetei Mayek, 64, 56, Meetei Mayek
	  //0xAC00, 0xD7AF, //Hangul Syllables, 11,184, 11,172, Hangul
	  //0xD7B0, 0xD7FF, //Hangul Jamo Extended-B, 80, 72, Hangul
	  //0xD800, 0xDB7F, //High Surrogates, 896, 0, Unknown
	  //0xDB80, 0xDBFF, //High Private Use Surrogates, 128, 0, Unknown
	  //0xDC00, 0xDFFF, //Low Surrogates, 1,024, 0, Unknown
	  //0xE000, 0xF8FF, //Private Use Area, 6,400, 6,400, Unknown
	  //0xF900, 0xFAFF, //CJK Compatibility Ideographs, 512, 472, Han
	  //0xFB00, 0xFB4F, //Alphabetic Presentation Forms, 80, 58, Armenian (5 characters), Hebrew (46 characters), Latin (7 characters)
	  //0xFB50, 0xFDFF, //Arabic Presentation Forms-A, 688, 611, Arabic (609 characters), Common (2 characters)
	  //0xFE00, 0xFE0F, //Variation Selectors, 16, 16, Inherited
	  //0xFE10, 0xFE1F, //Vertical Forms, 16, 10, Common
	  //0xFE20, 0xFE2F, //Combining Half Marks, 16, 16, Cyrillic (2 characters), Inherited (14 characters)
	  //0xFE30, 0xFE4F, //CJK Compatibility Forms, 32, 32, Common
	  //0xFE50, 0xFE6F, //Small Form Variants, 32, 26, Common
	  //0xFE70, 0xFEFF, //Arabic Presentation Forms-B, 144, 141, Arabic (140 characters), Common (1 character)
	  //0xFF00, 0xFFEF, //Halfwidth and Fullwidth Forms, 240, 225, Hangul (52 characters), Katakana (55 characters), Latin (52 characters), Common (66 characters)
	  //0xFFF0, 0xFFFF, //Specials, 16, 5, Common
	};

	/*
	class GFXglyph
	{
	  byte code;          // char code (last utf-8 char)
	  byte mode0;
	  byte mode1;
	  byte width;         // Bitmap dimensions in pixels
	  byte height;        // Bitmap dimensions in pixels
	  byte xAdvance;      // Distance to advance cursor (x axis)
	  byte xOffset;        // X dist from cursor pos to UL corner
	  byte yOffset;        // Y dist from cursor pos to UL corner
	  byte len0, len1;    // bitmap length
	}
	*/

	static void run(final String fontPath, final String fontName, final String fontType, final char[] charset, final int fontSize, final int fontSize2, final int fontHeight)
	{
		PApplet applet = new PApplet();
		applet.sketchPath();	// set default sketchPath, otherwise you are out of processing 
		PDE pde = new PDE();
		pde.setParent(applet);
	
		boolean smooth = false;
		PFont font = pde.getFont(fontPath+fontName+fontType, fontSize2, smooth, charset); //<>//
	
		String fontNameSizePt = fontName + "_" + fontSize + "pt";
		ToFile toFile = new ToFile(fontNameSizePt, fontHeight); 
		try {
			toFile.run(font);
			File file = new File("FontFiles/TSD_" + fontNameSizePt + ".h");
			FileWriter w = new FileWriter(file);
			w.write(toFile.writer.sb.toString());
			w.write(toFile.footer.sb.toString());
			w.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	static public void main(String[] args)
	{
//	    String[] fontList = PFont.list();

		char[]   charset;
		int count = 0;
		
		int blockCount = unicodeBlocks.length; //<>//
		
		for (int i = 0; i < blockCount; i+=2) {
			int firstUnicode = unicodeBlocks[i];
		    int lastUnicode  = unicodeBlocks[i+1];
		    count += (lastUnicode - firstUnicode + 1);
		}
		
		charset = new char[count];
		
		int index = 0;
		for (int i = 0; i < blockCount; i+=2) {
			int firstUnicode = unicodeBlocks[i];
		    int lastUnicode  =  unicodeBlocks[i+1];
		    for (int code = firstUnicode; code <= lastUnicode; code++) {
		    	charset[index] = Character.toChars(code)[0];
		    	index++;
		    }
		}
		
		run("/usr/share/fonts/google-noto/", "NotoSansMono-Regular", ".ttf", charset,  9, 20, 19);
		run("/usr/share/fonts/google-noto/", "NotoSansMono-Regular", ".ttf", charset, 12, 24, 24);
		run("/usr/share/fonts/google-noto/", "NotoSansMono-Regular", ".ttf", charset, 18, 36, 35);
		run("/usr/share/fonts/google-noto/", "NotoSansMono-Regular", ".ttf", charset, 24, 48, 47);

		run("/usr/share/fonts/google-noto/", "NotoSans-Regular", ".ttf", charset,  9, 20, 22);
		run("/usr/share/fonts/google-noto/", "NotoSans-Regular", ".ttf", charset, 12, 24, 29);
		run("/usr/share/fonts/google-noto/", "NotoSans-Regular", ".ttf", charset, 18, 36, 44);	// 42
		run("/usr/share/fonts/google-noto/", "NotoSans-Regular", ".ttf", charset, 24, 48, 58);	// 56

		run("/usr/share/fonts/google-noto/", "NotoSerif-Regular", ".ttf", charset,  9, 20, 22);
		run("/usr/share/fonts/google-noto/", "NotoSerif-Regular", ".ttf", charset, 12, 24, 29);
		run("/usr/share/fonts/google-noto/", "NotoSerif-Regular", ".ttf", charset, 18, 36, 44);	// 42
		run("/usr/share/fonts/google-noto/", "NotoSerif-Regular", ".ttf", charset, 24, 48, 58);	// 56
		
		System.out.println("OK");
	
	}
}
