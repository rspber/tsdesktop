#!/usr/bin/python3

#  Copyright (c) 2022, rspber (https://github.com/rspber)
#  All rights reserved
#
# Utility to convert Adafruit's GFX fonts to TSD font format.
#
# Usage:
#  - place Adafruit's fonts in subdirectory Fonts
#  - create empty subdirectory TSDFonts
#  - run tsdfonconvert.py
#  - then place the new fonts in tsdesktop/TSD_GFX subdirectory

import sys, re
import os, sys

bytes = []

def do_glyph(f, code, res, upto):
  offs = int(res.group(1))
  w = int(res.group(2))
  h = int(res.group(3))
  xadv = int(res.group(4))
  xo = int(res.group(5)) & 0xff
  yo = (1 - int(res.group(6))) & 0xff		# convert to positive
  s = ''
  j = 9
  for i in range (offs, upto):
    s = s + bytes[i] + ', '
    j = j + 1
    if j > 14:
      s = s + '\n    '
      j = 0
  if j == 14:
    s = s + '\n    '
    j = 0
  le = upto - offs;
  f.write('  0x%02X, 0x00, 0x00, %d, %d, %d, 0x%02x, 0x%02x, %d, %d, %s// \'%c\'\n' % (code, w, h, xadv, xo, yo, le & 0xff, le >> 8, s, code))

def proc_file(f, lines, name, yadv):
  global bytes

  code = 0x20
  code0 = code
  tt = [code0]
  f.write('static const uint8_t %s_Glyphs_%02X[] {\n' % (name, code0))
  mode = 0
  res0 = None
  bytes = []
  for s in lines:
    if s.startswith('#'):
      continue
    if s.startswith('//'):
      continue
    if s.startswith('const GFXglyph'):
      mode = 1
    if mode == 0:
      i = 0
      j = s.find('//')
      if j >= 0:
        s = s[:j]
      j = s.find('/*')
      while True:
        i = s.find('0x', i)
        if i < 0:
          break
        while j >= 0 and j < i:
          j = s.find('*/', j)
          if j >= 0:
            if j > i:
              i = s.find('0x', j + 2)
            j = s.find('/*', j + 2)
          else:
            i = len(s)
            break
        if i >= 0:
          bytes.append(s[i:i+4])
          i = i + 4
    else:
      result = re.search(r"\{\s*(\d+)\s*\,\s*(\d+)\s*\,\s*(\d+)\s*\,\s*(\d+)\s*\,\s*([-]*\s*\d+)\s*\,\s*([-]*\s*\d+)\s*\}", s)
      if result:
        if res0:
          offs = int(result.group(1))
          do_glyph(f, code, res0, offs)
          code = code + 1
          if (code % 0x20) == 0:
            f.write('0};\n\n')
            f.write('static const GFXfont %s_%02X {\n  %s_Glyphs_%02X,\n  0x00, 0x00, 0, 0x%02X, 0x%02X, %d\n};\n\n' % (name, code0, name, code0, code0, code-1, yadv))
            code0 = code
            tt.append(code0)
            f.write('static const uint8_t %s_Glyphs_%02X[] {\n' % (name, code0))
        res0 = result
      else:
        if res0:
          do_glyph(f, code, res0, len(bytes))
          code = code + 1
          res0 = None
  f.write('0};\n\n')
  f.write('static const GFXfont %s_%02X {\n  %s_Glyphs_%02X,\n  0x00, 0x00, 0, 0x%02X, 0x%02X, %d\n};\n\n' % (name, code0, name, code0, code0, code-1, yadv))

  f.write('static const GFXfont* %s[] {\n' % name)
  for c0 in tt:
    f.write('  &%s_%02X,\n' % (name, c0))
  f.write('  0\n};\n\n')
  s = lines[len(lines)-1]
  if s.startswith('//'):
    f.write(s)
  bytes = []

def do_file(olddir, newdir, fname):
  print(fname)

  s = fname
  if s.endswith('.h'):
    s = s[:-2]

  if s.endswith('7b'):
    s = s[:-2]

  f = open(olddir + '/' + fname, 'r')
  lines = f.readlines();
  f.close()

  for line in lines:
    z = re.search('0x7E\,\s*(\d+)\s*\}', line)
    if z:
      yadv = int(z.group(1))

  r = re.match('([A-Z_a-z]+)(\d+)pt', s)
  if r:
    s = r.group(1) + '_' + r.group(2) + 'pt'
  f = open(newdir + '/TSD_' + s +'.h', 'w')
  proc_file(f, lines, s, yadv)
  f.close()

def main():

  oldfonts = "Fonts"
  newfonts = "TSDFonts"
  dirs = os.listdir( oldfonts )

  for fname in dirs:
    if fname.endswith('.h'):
      do_file(oldfonts, newfonts, fname)

main()
