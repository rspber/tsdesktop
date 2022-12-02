
function updatePixels (table, newWidth, newHeight, x, y, fill) {
  const dataPixels = table.attr('data-pixels')
  const width = parseInt(table.attr('data-w'))
  const height = parseInt(table.attr('data-h'))
  const xoff = parseInt(table.attr('data-ow'))
  const base = parseInt(table.attr('data-oh'))
  const xAdjust = xoff * -1
  const yAdjust = (window['maxBaseline'] + base) * -1

  if (newWidth < 0) {
    newWidth = width
  }

  if (newHeight < 0) {
    newHeight = height
  }

  if (x >= 0 && y >= 0) {
    x += xAdjust
    y += yAdjust
  }

  // Store the pixels into a 2D bool array
  const array = []
  let pixelIndex = 0
  for (let i = 0; i < height; ++i) {
    const rowArray = []
    for (let j = 0; j < width; ++j) {
      if(x == j && y == i) {
        rowArray.push(fill)
      } else {
        rowArray.push(dataPixels.charAt(pixelIndex))
      }
      ++pixelIndex
    }
    array.push(rowArray)
  }

  // Write out to a string newWidth x newHeight
  let newPixels = ''
  for (let i = 0; i < newHeight; ++i) {
    for (let j = 0; j < newWidth; ++j) {
      if (i >= height || j >= width) {
        newPixels += '0'
      } else {
        newPixels += array[i][j]
      }
    }
  }

  // Pad out to a multiple of 8
  const pad = newPixels.length % 8
  for (let i = 0; i < pad; ++i) {
    newPixels += '0'
  }

  table.attr('data-pixels', newPixels)
}
