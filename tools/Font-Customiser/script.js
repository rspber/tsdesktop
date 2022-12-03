
function say(msg) {
  $('.ui.modal.message p').text(msg)
  $('.ui.modal.message').modal('show')
}

function makeUpDownButton (name, func, color, width) {
  const button = $('<div class="ui mini compact buttons" style="margin:1px"></div>')
  const style = 'ui compact button ' + func + '-handler ' + color
  button.append('<button id="' + func + '-remove" class="' + style + '">-</button>')
  button.append('<button id="' + func + '-value" class="ui compact disabled button ' + color + '" style="width:' + width + '">' + name + '</button>')
  button.append('<button id="' + func + '-add" class="' + style + '">+</button>')
  return button
}

function makeCheckButton (name, func, color, width, disabled) {
  const button = $('<span class="ui mini compact" style="margin:1px"></span>')
  button.append('<label class="ui '+color+' label" disabled style="width:' + width + '""><input type="checkbox" class="'+func+'-handler" id="'+func+'"' + (disabled ? ' checked="checked"' : '') + '> '+name+'</label>')
  return button
}

function makeGlyphItem (n, w, h, hexChar, adv, ow, oh, disabled) {
  const table = $('<div class="table"></div>')
    .addClass('glyph')
    .attr('data-pixels', n)
    .attr('data-w', w)
    .attr('data-h', h)
    .attr('data-hexchar', hexChar)
    .attr('data-adv', adv)
    .attr('data-ow', ow)
    .attr('data-oh', oh)
    .attr('data-dis', disabled ? 1 : 0)
    .css('opacity', disabled ? 0.1 : 1)

  const grid = $(`<div style="width: ${window['glyph_table_width']}px"></div>`)
  const div = $('<div class="ui attached segment inner"></div>')

  div.append(table)

  grid.append('<h2 class="ui top attached segment inner">' + ucharvalue(utf8_pfx, hexChar) + '</h2>')

  grid.append('<div class="ui attached segment inner secondary centered">' + unicodevalue(utf8_pfx, hexChar) + ' / ' + utf8value(utf8_pfx, hexChar) + '</div>')
  grid.append(div)

  const buttonBar = $('<div class="ui bottom attached warning message inner centered"></div>')
  buttonBar.append(makeUpDownButton('Rows', 'row', 'purple', 50))
  buttonBar.append(makeUpDownButton('Cols', 'col', 'violet', 50))
  buttonBar.append(makeUpDownButton('Base', 'base', 'green', 50))
  buttonBar.append(makeUpDownButton('XOff', 'xoff', 'blue', 50))
  buttonBar.append(makeUpDownButton('XAdv', 'xadv', 'teal', 50))
  buttonBar.append(makeCheckButton('Disable', 'dis', 'yellow', 110, disabled))

  grid.append(buttonBar)

  return grid
}

function setGlyphTable (table) {
  const xadvance = parseInt(table.attr('data-adv'))

  const xoffset = parseInt(table.attr('data-ow'))
  const yoffset = parseInt(table.attr('data-oh'))

  const width = parseInt(table.attr('data-w'))
  const height = parseInt(table.attr('data-h'))

  const pixels = table.attr('data-pixels')

  const left = xoffset
  const right = width + xoffset
  const top = maxBaseline + yoffset
  const bottom = top + height

  // Set the table
  table.empty()
  const maxH = maxBaseline + 1 - minUnderBaseline
  for (let i = 0; i < maxH; i++) {
    const row = $('<div></div>').addClass('row')
    for (let j = Math.min(0, xoffset); j <= Math.max(xadvance, right); j++) {
      const cell = $('<div></div>').addClass('cell').attr('data-x', j).attr('data-y', i)
      // These classes are used to position the "limits" vertical lines
      if (i === 0 && j === 0) {
        cell.addClass('before_xoffset')
      }
      if (i === 0 && j === xadvance) {
        cell.addClass('before_xadvance')
      }
      // Now draw the actual pixels
      if (i < top || i >= bottom || j < left || j >= right) {
        cell.addClass('dead')
      } else if (pixels.charAt((i - top) * width + (j - left)) == '1') {
        cell.addClass('fill')
      }
      // If the pixel is after xadvance, or before xoffset
      if (j >= xadvance || j < Math.max(0, xoffset)) {
        cell.addClass('over')
      }
      row.append(cell)
    }
    table.append(row)
  }

  // Draw the xadvance/xoffset limits
  // (We need to get the actual cell position before hand)
  const bef_xoffset = table.find('div.before_xoffset')
  const left_xoffset = bef_xoffset[0].offsetLeft
  xoffset_limit = $('<div class="xoffset_limit limit"></div>').css('left', (left_xoffset - 1) + 'px') // -1 accounts for first border of table
  table.append(xoffset_limit)

  const bef_xadvance = table.find('div.before_xadvance')
  const left_xadvance = bef_xadvance[0].offsetLeft
  xadvance_limit = $('<div class="xadvance_limit limit"></div>').css('left', (left_xadvance - 1) + 'px')
  table.append(xadvance_limit)

  // Set the baseline
  baseline = $('<div class="baseline"></div>').css('top', ((maxBaseline + 1) * 10 - 1) + 'px')
  table.append(baseline)
}

function advanceLoading (percent) {
  const element = document.querySelector('#loader .advance')
  element.style.width = Math.floor(294 * percent)
}

function displayGlyphTable () {
  const l = $('#glyphs').children().length
  $('#glyphs').children().each(function (i) {

    function setGlyphTableClosure(el, adv) {
      setTimeout(function () {
        advanceLoading(0.2 + 0.8 * adv)
        setGlyphTable(el.children().find('.glyph'))
        if (adv === 1) {
          $('#loader').hide()
        }
      }, 1)
    }

    setGlyphTableClosure($(this), (i + 1) / l)
  })
}

$(document).ready(function () {

  $('#export').attr('disabled', 'disabled')
  $('#reset').prop("disabled", 'disabled')
  $('.ui.checkbox.use-charcode').checkbox()

  $(document).on('change', '.ui.checkbox.use-charcode input', function (e) {
    $('#new-character').parent().toggle()
    $('#new-character-code').parent().toggle()

    if ($('input[name=charcode]').is(':checked')) {
      if ($('#new-character-code').val() == '') {
        $('.character').text('N/A')
      } else {
        $('.character').text(String.fromCharCode(parseInt($('#new-character-code').val(), 16)))
      }
    } else {
      if ($('#new-character').val() == '') {
        $('.charcode').text('N/A')
      } else {
        $('.charcode').text('0x' + $('#new-character').val().charCodeAt(0).toString(16).toUpperCase())
      }
    }
  })

  $('#loader').hide()
  font_name = null
  FirstFontSection = null
  LastFontSection = null

  $('#import').click(function () {
    advanceLoading(0)
    $('#loader').fadeIn(function() {
      extractFont()
    })
  })

  $('#reset').click(function () {

    $('.ui.modal.confirmation p').text('The reset will close the font and discard any changes you made.')
    $('.ui.modal.confirmation').modal({
      closable: false,
      onApprove: function() {
        $('#glyphs').empty()
        $('#source').val('')
        $('#create').prop("disabled", false)
        $('#import').prop("disabled", false)
        $('#export').prop("disabled", "disabled")
        $('#reset').prop("disabled", "disabled")
        $('.fontname').text('unknown').parent().hide()
      }
    }).modal('show')
  })

  const fillPixel = (target, fill) => {
    fill === '1' ? target.addClass('fill') : target.removeClass('fill')
    const table = target.parent().parent().parent().find('.table.glyph')
    updatePixels(table, -1, -1, parseInt(target.attr('data-x')), parseInt(target.attr('data-y')), fill)
  }

  let isFilling = false;
  let fillingMode = '1';
  $(document).on('mousedown', '.cell:not(.dead)', function (e) {
    isFilling = true;
    fillingMode = $(e.target).hasClass('fill') ? '0' : '1'
    fillPixel($(e.target), fillingMode)
  })
  $(document).on('mouseup', '.cell:not(.dead)', function (e) {
    isFilling = false;
  })
  $(document).on('mouseenter', '.cell:not(.dead)', function (e) {
    if (isFilling) {
      fillPixel($(e.target), fillingMode)
    }
  })

  $(document).on('click', '.row-handler', function (e) {
    const targetID = $(e.target).attr('id')
    const table = $(e.target).parent().parent().parent().find('.table.glyph')
    let height = parseInt(table.attr('data-h'))

    if (targetID === 'row-add') {
      height++
    } else if (targetID === 'row-remove') {
      height--
    }

    updatePixels(table, -1, height, -1, -1, false)
    table.attr('data-h', height)
    setGlyphTable(table)
    return false
  })

  $(document).on('click', '.col-handler', function (e) {
    const targetID = $(e.target).attr('id')
    const table = $(e.target).parent().parent().parent().find('.table.glyph')
    let width = parseInt(table.attr('data-w'))

    if (targetID === 'col-add') {
      width++
    } else if (targetID === 'col-remove') {
      width--
    }

    updatePixels(table, width, -1, -1, -1, false)
    table.attr('data-w', width)
    setGlyphTable(table)
    return false
  })

  $(document).on('click', '.base-handler', function (e) {
    const targetID = $(e.target).attr('id')
    const table = $(e.target).parent().parent().parent().find('.table.glyph')
    if (targetID === 'base-add') {
      table.attr('data-oh', parseInt(table.attr('data-oh')) + 1)
    } else if (targetID === 'base-remove') {
      table.attr('data-oh', parseInt(table.attr('data-oh')) - 1)
    }

    setGlyphTable(table)
    return false
  })

  $(document).on('click', '.xadv-handler', function (e) {
    const targetID = $(e.target).attr('id')
    const table = $(e.target).parent().parent().parent().find('.table.glyph')
    if (targetID === 'xadv-add') {
      table.attr('data-adv', parseInt(table.attr('data-adv')) + 1)
    } else if (targetID === 'xadv-remove') {
      table.attr('data-adv', parseInt(table.attr('data-adv')) - 1)
    }

    setGlyphTable(table)
    return false
  })

  $(document).on('click', '.xoff-handler', function (e) {
    const targetID = $(e.target).attr('id')
    const table = $(e.target).parent().parent().parent().find('.table.glyph')
    if (targetID === 'xoff-add') {
      table.attr('data-ow', parseInt(table.attr('data-ow')) + 1)
    } else if (targetID === 'xoff-remove') {
      table.attr('data-ow', parseInt(table.attr('data-ow')) - 1)
    }

    setGlyphTable(table)
    return false
  })

  $(document).on('change', '.dis-handler', function (e) {
    const table = $(e.target).parent().parent().parent().parent().find('.table.glyph')
    table.attr('data-dis', 1 - parseInt(table.attr('data-dis')))
    table.fadeTo('fast', 1 - 0.9 * table.attr('data-dis'))
    return false
  })

  $(document).on('keyup', '#newfont-size', function (e) {
    $('#newfont-height').val(Math.round(parseInt(this.value)/0.41))
  })

  $(document).on('change', '#newfont-size', function (e) {
    $('#newfont-height').val(Math.round(parseInt(this.value)/0.41))
  })

  $(document).on('keyup', '#newfont-utf8-pfx', function (e) {
    newCharsChanged(this, document.getElementById('newfont-first-hex'), document.getElementById('newfont-last-hex'))
  })

  $(document).on('change', '#newfont-utf8-pfx', function (e) {
    newCharsChanged(this, document.getElementById('newfont-first-hex'), document.getElementById('newfont-last-hex'))
  })

  $(document).on('keyup', '#newfont-first-hex', function (e) {
    newCharsChanged(document.getElementById('newfont-utf8-pfx'), this, document.getElementById('newfont-last-hex'))
  })

  $(document).on('change', '#newfont-first-hex', function (e) {
    newCharsChanged(document.getElementById('newfont-utf8-pfx'), this, document.getElementById('newfont-last-hex'))
  })

  $(document).on('keyup', '#newfont-last-hex', function (e) {
    newCharsChanged(document.getElementById('newfont-utf8-pfx'), document.getElementById('newfont-first-hex'), this)
  })

  $(document).on('change', '#newfont-last-hex', function (e) {
    newCharsChanged(document.getElementById('newfont-utf8-pfx'), document.getElementById('newfont-first-hex'), this)
  })

  $('#create').click(function () {
    $('.ui.modal.newfont').modal({
      closable: false,

      onApprove: function() {
        const name = $('#newfont-name').val()
        if (name.length < 1) {
          say('The font must have a name.')
          return
        }

        const fontSize = parseInt($('#newfont-size').val())
        if (fontSize < 1) {
          say('The font size must be greater than 0.')
          return
        }

        const fontHeight = parseInt($('#newfont-height').val())
        if (fontHeight < 1) {
          say('The font height must be greater than 0.')
          return
        }

        const l_utf8_pfx = $('#newfont-utf8-pfx').val().replace(/\s/g, '').toUpperCase()
        const first = $('#newfont-first-hex').val()
        const last = $('#newfont-last-hex').val()

        const fst_utf8_tt = utf8table(l_utf8_pfx, first)

        if (fst_utf8_tt == '' || fst_utf8_tt.length == 0) {
          say('First character is incorrect.')
          return
        }
        const l_idx = fst_utf8_tt.length - 1;

        const lst_utf8_tt = utf8table(l_utf8_pfx, last)

        if (lst_utf8_tt == '' || lst_utf8_tt.length != l_idx + 1) {
          say('Last character is incorrect.')
          return
        }

        for (i = 0; i <= l_idx; ++i) {
          if (parseInt(fst_utf8_tt[i], 16) > parseInt(lst_utf8_tt[i], 16)) {
            say('Last char can\'t be less then first one')
            return;
          }
        }

        font_name = name + '_' + fontSize + 'pt'
        font_size = fontSize
        utf8_pfx = l_utf8_pfx
        font_fract = (utf8_pfx.length > 0 ? utf8_pfx : first).toUpperCase()
        font_height = fontHeight
        minUnderBaseline = -Math.round(fontHeight/5)
        maxBaseline = fontHeight + minUnderBaseline;

        // Calculate the ideal glyph width. 160px is the minimum (for the buttons)
        glyph_table_width = Math.max(160, fontHeight * 11 + 30 /* some margin */)

        $('.fontname').text(font_name).parent().show()
        $('#firstglyph').val(first.toUpperCase())
        $('#lastglyph').val(last.toUpperCase())
        $('#glyphs').empty()

        // Change button states
        $('#export').prop( "disabled", false )
        $('#create').prop( "disabled", true )
        $('#import').prop( "disabled", true )
        $('#reset').prop("disabled", false)

        for (j = parseInt(first, 16); j <= parseInt(last, 16); ++j) {
          const hexChar = toHex00(j)
          const grid = makeGlyphItem (' ' , 1, 1, hexChar, 4, 0, -maxBaseline, false)
          $('#glyphs').append(grid)
          setGlyphTable(grid.find('.glyph'))
        }
      }
    })
    .modal('show')
  })

  $('#export').click(function () {
    exportFont()
  })
})
