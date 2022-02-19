#include "dmtx_lcd.h"
#include "dmtx.h"
#include <stdlib.h>
#include <string.h>

#define RGB2COLOR(r, g, b) ((((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)))

char *generate_datamatrix(const char *data, int border, int scale, int *width, int *height)
{
  char *symbol_data = NULL;
  int bytesPerPixel;

  // int ret = dotcode_dmtx_encode(data, strlen(data), 0, 6, &symbol_mat);

  DmtxEncode *enc;

  enc = dmtxEncodeCreate();

  dmtxEncodeSetProp(enc, DmtxPropPixelPacking, DmtxPack8bppK);
  // dmtxEncodeSetProp( enc, DmtxPropPixelPacking, DmtxPack32bppRGB );
  // dmtxEncodeSetProp( enc, DmtxPropWidth, 160 );
  // dmtxEncodeSetProp( enc, DmtxPropHeight, 160 );
  dmtxEncodeSetProp(enc, DmtxPropMarginSize, border);
  dmtxEncodeSetProp(enc, DmtxPropModuleSize, scale);

  dmtxEncodeDataMatrix(enc, strlen(data), (unsigned char *)data);

  *width = dmtxImageGetProp(enc->image, DmtxPropWidth);
  *height = dmtxImageGetProp(enc->image, DmtxPropHeight);
  bytesPerPixel = dmtxImageGetProp(enc->image, DmtxPropBytesPerPixel);

  symbol_data = (char *)malloc(sizeof(char) * (*width) * (*height) * bytesPerPixel);

  memcpy(symbol_data, enc->image->pxl, sizeof(char) * (*width) * (*height) * bytesPerPixel);

  dmtxEncodeDestroy(&enc);

  return symbol_data;
}

void dmtx_update_buffer(const char *dm_data, uint8_t *data, int width, int height, int depth)
{
  int i, j;
  int k = 0;
  int ox, oy;
  int p;

  char *output_data = NULL;
  int dm_width, dm_height;

  if (dm_data == NULL)
  {
    return;
  }
  output_data = generate_datamatrix(dm_data, 0, 12, &dm_width, &dm_height);

  ox = (width - dm_width) / 2;
  oy = (height - dm_height) / 2;
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width * depth; j += depth)
    {
      uint8_t pixel_r = 255; // random(255);
      uint8_t pixel_g = 255; // random(255);
      uint8_t pixel_b = 255; // random(255);
      if (j / 2 >= ox && j / 2 < (ox + dm_width) && i >= oy && i < (oy + dm_height))
      {
        p = j / 2 - ox + (i - oy) * dm_width;

        if ((int)(output_data[p]) == 0)
        {
          pixel_r = 0;
          pixel_g = 0;
          pixel_b = 0;
        }
        else
        {
          pixel_r = 255;
          pixel_g = 255;
          pixel_b = 255;
        }
      }
      unsigned int pixel = (pixel_r & 0xFF << 16) | (pixel_g & 0xFF << 8) | pixel_b & 0xFF;
      unsigned short rgb565 = RGB2COLOR(pixel_r, pixel_g, pixel_b); // RGB888ToRGB565(pixel);
      *(data + i * width * depth + j) = (rgb565 & 0xFF00) >> 8;
      *(data + i * width * depth + j + 1) = rgb565 & 0xFF;
    }
  }
  free(output_data);
}
