
function toHex00(v) {
  if (v < 0) {
    v = 256 + v;
  }
  return ('00' + v.toString(16).toUpperCase()).slice(-2)
}

function toHex(v) {
  return '0x' + toHex00(v)
}

function hexchar(c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')
}

function pfxhex(pfx) {
   switch (pfx.length) {
     case 0: return [];
     case 3:
     case 1: return '';
     case 2:
       if (hexchar(pfx[0]) && hexchar(pfx[1])) {
         let c1 = parseInt(pfx, 16)
         return c1 >= 0xc0 && c1 <= 0xdf ? [c1] : ''
       }
       else {
         return '';
       }
     case 4:
       if (hexchar(pfx[0]) && hexchar(pfx[1]) && hexchar(pfx[2]) && hexchar(pfx[3])) {
         let c1 = parseInt(pfx.substring(0,2), 16)
         let c2 = parseInt(pfx.substring(2,4), 16)
         return c1 >= 0xe0 && c2 >= 0x80 && c2 <= 0xbf ? [c1, c2] : ''
       }
       else {
         return '';
       }
     case 5:
       if (hexchar(pfx[0]) && hexchar(pfx[1]) && pfx[2] == ' ' && hexchar(pfx[3]) && hexchar(pfx[4])) {
         let c1 = parseInt(pfx.substring(0,2), 16)
         let c2 = parseInt(pfx.substring(3,5), 16)
         return c1 >= 0xe0 && c2 >= 0x80 && c2 <= 0xbf ? [c1, c2] : ''
       }
       else {
         return '';
       }
     default:
       return '';
   }
}

function utf8table(pfx, code) {
  let tt = pfxhex(pfx)
  if (!Array.isArray(tt) || code.length != 2 || !hexchar(code[0]) || !hexchar(code[1])) {
    return '';
  }
  else {
    let v = parseInt(code, 16)
    if (tt.length > 0) {
      if (v >= 0x80 && v <= 0xbf) {
        tt.push(v)
      }
      else {
        return ''
      }
    }
    else {
      tt.push(v)
    }
    return tt
  }
}

function utf8value(pfx, code) {
  let tt = utf8table(pfx, code)
  if (Array.isArray(tt)) {
    let s = ''
    for (i = 0; i < tt.length; ++i) {
      let t = tt[i]
      s += ' ' + toHex(t) + ','
    }
    return s
  }
  return ''
}

function unicode(pfx, code) {
  let tt = utf8table(pfx, code)
  if (Array.isArray(tt)) {
    let v = 0
    switch (tt.length) {
      case 3:
      {
        let c1 = tt[0]
        let c2 = tt[1]
        let c3 = tt[2]
        if (c1 < 0xe1 && c2 < 0xa0) {
          return NaN;
        }
        v = ((c1 & 0x0f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f)
        break;
      }
      case 2:
      {
        let c1 = tt[0]
        let c2 = tt[1]
        if (c1 < 0xc4) {
          return NaN;
        }
        v = ((c1 & 0x1f) << 6) | (c2 & 0x3f)
        break;
      }
      default:
        v = tt[0]
    }
    return v
  }
  return -1
}

function unicodevalue(pfx, code) {
  let u = unicode(pfx, code)
  return isNaN(u) ? 'ureachable' : u < 0 ? '' : '0x' + ('0000' + u.toString(16)).slice(-4)
}

function ucharvalue(pfx, code) {
  let u = unicode(pfx, code)
  return isNaN(u) ? 'unreachable' : u < 0 ? '' : String.fromCharCode(u)
}

function fillUCodeAndUtf8(pfx, charInput){
  const char = charInput.nextSibling;
  const ucode = char.nextSibling;
  const utf8 = ucode.nextSibling;
  const code = charInput.value
  char.value = ucharvalue(pfx.value, code)
  ucode.value = unicodevalue(pfx.value, code)
  utf8.value = utf8value(pfx.value, code).toLowerCase()
}

function newCharsChanged(pfx, firstInput, lastInput) {
  fillUCodeAndUtf8(pfx, firstInput)
  fillUCodeAndUtf8(pfx, lastInput)
}


function load_err(msg) {
  alert('No correct font file found (' + msg + '), please paste the suitable fragment of TSD_GFX font file first.')
  $("#loader").hide()
}

function signedByte(v) {
  return v < 128 ? v : v - 256
}

function extractFont () {
  let data = $('#source').val()

  const re1 = /static\s+const\s+uint8\_t\s+(\w+)\_(\d+)pt\_Glyphs\_(\w+)\[\]\s*\{/
  const found1 = data.match(re1)

  if (found1 != null && found1.length > 2) {
    font_name = found1[1] + '_' + found1[2] + 'pt'
    font_size = found1[2]
    font_fract = found1[3]
  } else {
      load_err('No starting section: \'static const uint8_t <FontName>_<size>pt_Glyphs_<CC>[]\' found')
    return
  }

  $('#glyphs').empty()

  var part2
  {
    const re2 = /static\s+const\s+GFXfont\s+(\w+)\_(\d+)pt\_(\w+)\s*\{/
    const found2 = data.match(re2)
    if (found2 != null && found2.length > 2) {
      name2 = found2[1] + '_' + found2[2] + 'pt'
      if (font_name != name2) {
        load_err('Font names not match: ' + font_name + ' <> ' + name2)
        return;
      }
      fract2 = found2[3]
      if (font_fract != fract2) {
        load_err('Font subranges not match: ' + font_fract + ' <> ' + fract2)
        return;
      }
      const i = data.indexOf(found2[0])
      part2 = data.substring(i + found2[0].length)
      data = data.substring(0, i)
    }
    else {
      load_err('No ending section: \'static const GFXfont <FontName>_<size>pt_<CC>\' found')
      return;
    }
  }

  data = 'FirstFontSection = [' + data.substring(found1[0].length)
  data = data.replace(/\n  0x/gi, '\n[0x')
  data = data.replace(/ \'\'\'\n/gi, ' \'\'\n')
  data = data.replace(/ \/\/ \'(.*)\'(.*)\n/gi, '\'$1 $2\'],\n')
  data = data.replace('0}', '0]')

  eval(data)

  /*
      // Tada 🎉
  */

  {
    const re3 = /(\w+)\_(\d+)pt\_Glyphs\_(\w+)\s*\,/
    const found3 = part2.match(re3)
    if (found3 != null && found3.length > 2) {
      let i = part2.indexOf(found3[0])
      part2 = part2.substring(i + found3[0].length)
    }
    else {
      load_err('No item in ending section: \'<FontName>_<size>pt_Glyphs_<CC>,\' found')
      return;
    }
  }
  part2 = 'LastFontSection = [' + part2.replace('}', ']')

  eval(part2)

  /*
      // Tada 🎉
  */

  const tt = LastFontSection
  const c1 = tt[0]
  const c2 = tt[1]
  utf8_pfx = (c1 != 0 ? toHex00(c1) + (c2 != 0 ? toHex00(c2) : '') : '').toUpperCase();
  const first = tt[3]
  const last = tt[4]
  let fract3 = utf8_pfx != '' ? utf8_pfx : toHex00(first)
  if (fract3 != font_fract) {
    load_err('Font subranges in footer not match: ' + font_fract + ' <> ' + fract3)
    return;
  }
  maxH = tt[5]

  $('.fontname').text(font_name).parent().show()
  $('#firstglyph').val(toHex00(first).toUpperCase())
  $('#lastglyph').val(toHex00(last).toUpperCase())

  const glyphsArray = FirstFontSection

  // Run pre-calculations for correct display
  let maxW = 0
  maxBaseline = 0
  minUnderBaseline = 0
  let tablen = 0;
  for (ind in glyphsArray) {
    const glyph = glyphsArray[ind]
    if (Array.isArray(glyph)) {
      const inv_oh = signedByte(glyph[7])
      maxW = Math.max(maxW, glyph[3], glyph[5])
      maxBaseline = Math.max(maxBaseline, inv_oh)
      minUnderBaseline = Math.min(minUnderBaseline, inv_oh + 1 - glyph[4])
      tablen++;
    }
  }

  // Calculate the ideal glyph width. 160px is the minimum (for the buttons)
  glyph_table_width = Math.max(160, maxW * 11 + 30 /* some margin */)

  // sort glyphsArray

  let charIndex = 0;
  let lastChar = glyphsArray[0][0]
  for (ind in glyphsArray) {
    const glyph = glyphsArray[ind]
    if (Array.isArray(glyph)) {
      ++lastChar

      function glyphAppendClosure(el, adv) {
        setTimeout(function () {
          advanceLoading(0.2 * adv)
          $('#glyphs').append(el)
          if (adv === 1) {
            // Run the setGlyphTable function now
            displayGlyphTable()
          }
        }, 1)
      }

      while (glyph[0] > lastChar) {
        const grid = makeGlyphItem (' ' , 1, 1, toHex00(lastChar), 4, 0, -fontHeight, true)
        ++lastChar;
      }
      const hexChar = toHex00(glyph[0])
      const w = glyph[3]
      const h = glyph[4]
      const adv = glyph[5]
      const ow = signedByte(glyph[6])
      const oh = -signedByte(glyph[7])
      let n = ''

      const nof = parseInt(glyph[8]) | (parseInt(glyph[9]) << 8);
      for (let k = 0; k < nof; k++) {
        n += ('000000000' + glyph[10 + k].toString(2)).substr(-8)
      }

      const grid = makeGlyphItem (n, w, h, hexChar, adv, ow, oh, false)

      charIndex ++;
      glyphAppendClosure(grid, charIndex / tablen)
    }
  }

  $('#export').prop( "disabled", false )
  $('#reset').prop("disabled", false)
  $('#create').prop( "disabled", true )
  $('#extract').prop( "disabled", true )
}

function bits8(bits) {
  return (bits + '00000000').slice(0, 8)
}

function exportFont() {
  const glyphs = []

  $('.table.glyph').each(function () {
    const t = $(this)
    const hexchar = t.attr('data-hexchar')

    const data_w = parseInt(t.attr('data-w'))
    const data_h = parseInt(t.attr('data-h'))

    const data_adv = parseInt(t.attr('data-adv'))
    const data_ow = parseInt(t.attr('data-ow'))
    const data_oh = parseInt(t.attr('data-oh'))

    var data_pixels = t.attr('data-pixels')
//    const data_dis = parseInt(t.attr('data-dis'))
    let bits = ''

    let bitmap = []
    let j = 8
    for (let i = 0; i < data_pixels.length; i++) {
      bits += data_pixels.charAt(i)
      // Each 8 bits, we form the HEX value
      if (bits.length == 8) {
        let out = ' ' + toHex(parseInt(bits8(bits), 2))
        if (++j > 16) {
           out = '\n   ' + out
           j = 1
        }
        bitmap.push(out)
        bits = ''
      }
    }

    // Remaining bits with padding then, if necessary
    if (bits != '') {
      let out = ' ' + toHex(parseInt(bits8(bits), 2))
      if (++j > 16) {
         out = '\n   ' + out
         j = 1
      }
      bitmap.push(out)
    }

    const len = bitmap.length
    //    if (data_dis == 0)
    let output = '  0x' + hexchar + ', ' + '0x00, 0x00, '
      + data_w + ', ' + data_h + ', ' + data_adv + ', '
      + toHex(data_ow).toLowerCase() + ', ' + toHex(-data_oh).toLowerCase() + ', '
      + (len & 0xff) + ', ' + (len >> 8) + ',' + bitmap.join(',') + ','

    let comment = ' // ' + '\'' + ucharvalue(utf8_pfx, hexchar) + '\''
    if (utf8_pfx != '') {
      j += (utf8_pfx.length >> 1)
      comment += utf8value(utf8_pfx, hexchar)
    }
    if (j > 14) {
        output += '\n   '
    }
    output += comment
    glyphs.push(output)
  })

  let c1 = '0x00'
  let c2 = '0x00'
  if (utf8_pfx.length >= 2) {
    c1 = '0x' + utf8_pfx.substring(0, 2)
    if (utf8_pfx.length >= 4) {
      const i = 2 + (utf8_pfx.length > 4 ? 1 : 0)
      c2 = '0x' + utf8_pfx.substring(i, i + 2)
    }
  }
  let output =
  'static const uint8_t ' + font_name + '_Glyphs_' + font_fract + '[] {\n' +
    glyphs.join('\n') + '\n' +
  '0};\n\n' +
  'static const GFXfont ' + font_name + '_' + font_fract + ' {\n' +
    '  ' + font_name + '_Glyphs_' + font_fract + ',\n' +
    '  ' + c1 + ', ' + c2 + ', ' + '0' + ', ' + '0x' + $('#firstglyph').val() + ', ' + '0x' + $('#lastglyph').val() + ', ' + maxH + '\n' +
  '};\n'

  $('#result').val(output)
}
