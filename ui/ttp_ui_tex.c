/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_ui_tex.c
 * PURPOSE     : Tough Third Planet project.
 *               Draw to texture UI module.
 * PROGRAMMER  : CGSG'2022.
 *               Nikita Koptelov (NK1).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"

/* Pointer to current 2D drawing structure */
static ttpDRAW2D *TTP_UIDraw2D;

/* UBO for 2D drawing output (vec4(x, y, tex_w, tex_h)) */
static ttpUBO *TTP_UIDraw2DUBO;
/* Primitive for 2D drawing output */
static ttpPRIM *TTP_UIDraw2DPrim;

/* Helpful font set function in current 2D drawing structure.
 * ARGUMENTS:
 *   - 2D drawing structure to draw:
 *       ttpDRAW2D *Dr;
 * RETURNS: None.
 */
static VOID TTP_UIFontSet( ttpDRAW2D *Dr )
{
  INT Weight = FW_NORMAL;
  BOOL
    IsItalic = FALSE,
    IsUnderline = FALSE,
    IsStrike = FALSE;

  if (Dr == NULL)
    return;

  if (Dr->Params & 0x0001)
    Weight = FW_LIGHT;
  else if (Dr->Params & 0x0002)
    Weight = FW_NORMAL;
  else if (Dr->Params & 0x0004)
    Weight = FW_BOLD;

  if (Dr->Params & 0x0008)
    IsItalic = FALSE;
  else if (Dr->Params & 0x0010)
    IsItalic = TRUE;

  if (Dr->Params & 0x0020)
    IsUnderline = FALSE;
  else if (Dr->Params & 0x0040)
    IsUnderline = TRUE;

  if (Dr->Params & 0x0080)
    IsStrike = FALSE;
  else if (Dr->Params & 0x0100)
    IsStrike = TRUE;

  if (Dr->hFnt != NULL)
    DeleteObject(Dr->hFnt);
  Dr->hFnt = CreateFont(Dr->Size,
    0, 0, 0, Weight,
    IsItalic, IsUnderline, IsStrike, RUSSIAN_CHARSET,
    OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, /* ANTIALIASED_QUALITY */
    VARIABLE_PITCH | FF_DONTCARE,
    Dr->FontName);
} /* End of 'TTP_UIFontSet' function */

/* Create struct for 2D drawing function.
 * ARGUMENTS:
 *   - width and height of image:
 *       INT W, H;
 * RETURNS:
 *   (ttpDRAW2D *) pointer to created 2D drawing structure.
 */
static ttpDRAW2D * TTP_UIDraw2DCreate( INT W, INT H )
{
  ttpDRAW2D *Dr;

  /* Create 2D drawing structure */
  if ((Dr = malloc(sizeof(ttpDRAW2D))) == NULL)
    return NULL;

  Ttp->ImgCreate(&Dr->Img, W, H, 4, "UI image sample");
  Dr->Tex = Ttp->TexAddImg(&Dr->Img);
  Dr->Size = 18;
  strncpy(Dr->FontName, "Calibri Light", TTP_MAX_NAME);
  Dr->Params = TTP_FONT_MEDIUM | TTP_FONT_ITALIC_OFF |
    TTP_FONT_UNDERLINE_OFF | TTP_FONT_STRIKE_OFF;
  TTP_UIFontSet(Dr);
  Dr->ToReverseText = FALSE;
  Dr->LimitX1 = Dr->LimitY1 = Dr->LimitX2 = Dr->LimitY2 = 0;
  Dr->IsLimitsActive = FALSE;
  return Dr;
} /* End of 'TTP_UIDraw2DCreate' function */

/* Free struct for 2D drawing function.
 * ARGUMENTS:
 *   - pointer to structure:
 *       ttpDRAW2D *Dr;
 * RETURNS: None.
 */
static VOID TTP_UIDraw2DFree( ttpDRAW2D *Dr )
{
  if (Dr == NULL)
    return;

  Ttp->ImgFree(&Dr->Img);
  Ttp->TexFree(Dr->Tex);
  if (Dr->hFnt != NULL)
    DeleteObject(Dr->hFnt);
  free(Dr);
} /* End of 'TTP_UIDraw2DFree' function */

/* Clear image in 2D drawing structure and recreate reversed hbitmap function.
 * ARGUMENTS:
 *   - 2D drawing structure pointer:
 *       ttpDRAW2D *Dr;
 * RETURNS: None.
 */
static VOID TTP_UIDraw2DReverse( ttpDRAW2D *Dr )
{
  BITMAPINFOHEADER bmi = {0};

  /* NOTE: use after creating a 2d drawing struct */

  if (Dr == NULL)
    return;

  if (Dr->Img.hBm != NULL)
    DeleteObject(Dr->Img.hBm);

  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biWidth = Dr->Img.W;
  bmi.biHeight = Dr->Img.H;
  bmi.biBitCount = 32;
  bmi.biPlanes = 1;
  bmi.biCompression = 0;
  bmi.biSizeImage = Dr->Img.W * Dr->Img.H * 4;
  bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 1;
  bmi.biClrUsed = 0;
  bmi.biClrImportant = 0;

  Dr->Img.hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, (VOID **)&Dr->Img.Bits, NULL, 0);
} /* End of 'TTP_UIDraw2DReverse' function */

/* Display 2D drawing to screen function.
 * ARGUMENTS:
 *   - pointer to structure:
 *       ttpDRAW2D *Dr;
 *   - screen position:
 *       INT X, Y;
 * RETURNS: None.
 */
static VOID TTP_UIDraw2DToScreen( ttpDRAW2D *Dr, INT X, INT Y )
{
  VEC4 v;

  if (Dr == NULL)
    Dr = TTP_UIRenderDraw2D;
  v = Vec4Set(X, Y, Dr->Img.W, Dr->Img.H);
  Ttp->UBOUpdate(TTP_UIDraw2DUBO, 0, 0, &v);
  TTP_UIDraw2DPrim->Mtl->Tex[0] = Dr->Tex;
  TTP_UIDraw2DPrim->Mtl->NumOfTextures = 1;
  Ttp->PrimDraw(TTP_UIDraw2DPrim, MatrIdentity());
} /* End of 'TTP_UIDraw2DToScreen' function */

/* Start draw to texture function.
 * ARGUMENTS:
 *   - 2D drawing structure to draw:
 *       ttpDRAW2D *Dr;
 * RETURNS: None.
 */
static VOID TTP_UIDrawStart( ttpDRAW2D *Dr )
{
  TTP_UIDraw2D = Dr;
} /* End of 'TTP_UIDrawStart' function */

/* Put limits in 2D drawing struct function.
 * ARGUMENTS:
 *   - left top point (left top corner is begin of coord):
 *       INT X1, Y1;
 *   - right bottom point (left top corner is begin of coord):
 *       INT X2, Y2;
 * RETURNS: None.
 */
static VOID TTP_UIPutLimitsSet( INT X1, INT Y1, INT X2, INT Y2 )
{
  INT W, H;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  W = TTP_UIDraw2D->Img.W;
  H = TTP_UIDraw2D->Img.H;

  if (X1 < 0)
    X1 = 0;
  else if (X1 >= W)
    X1 = W - 1;

  if (Y1 < 0)
    Y1 = 0;
  else if (Y1 >= H)
    Y1 = H - 1;

  if (X2 < 0)
    X2 = 0;
  else if (X2 >= W)
    X2 = W - 1;

  if (Y2 < 0)
    Y2 = 0;
  else if (Y2 >= H)
    Y2 = H - 1;

  TTP_UIDraw2D->LimitX1 = X1;
  TTP_UIDraw2D->LimitY1 = Y1;
  TTP_UIDraw2D->LimitX2 = X2;
  TTP_UIDraw2D->LimitY2 = Y2;
} /* End of 'TTP_UIPutLimits' function */

/* Set limits to default in 2D drawing struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIPutLimitsDef( VOID )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  TTP_UIDraw2D->LimitX1 = TTP_UIDraw2D->LimitY1 = 0;
  TTP_UIDraw2D->LimitX2 = TTP_UIDraw2D->Img.W - 1;
  TTP_UIDraw2D->LimitY2 = TTP_UIDraw2D->Img.H - 1;
} /* End of 'TTP_UIPutLimits' function */

/* Get inactive limits in 2D drawing structure function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIPutLimitsOff( VOID )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  TTP_UIDraw2D->IsLimitsActive = FALSE;
} /* End of 'TTP_UIPutLimitsOff' function */

/* Get active limits in 2D drawing structure function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIPutLimitsOn( VOID )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  TTP_UIDraw2D->IsLimitsActive = TRUE;
} /* End of 'TTP_UIPutLimitsOn' function */

/* Get pixel from texture function.
 * ARGUMENTS:
 *   - position of pixel:
 *       INT X, Y;
 * RETURNS:
 *   (VEC4) returned color of pixel.
 */
static VEC4 TTP_UIGetPixel( INT X, INT Y )
{
  DWORD Col;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;
    if (TTP_UIDraw2D->IsLimitsActive)
  {
    if (X < TTP_UIDraw2D->LimitX1 || X > TTP_UIDraw2D->LimitX2 ||
        Y < TTP_UIDraw2D->LimitY1 || Y > TTP_UIDraw2D->LimitY2)
      return Vec4Set1(0);
  }
  else
    if (X < 0 || X > TTP_UIDraw2D->Img.W - 1 ||
        Y < 0 || Y > TTP_UIDraw2D->Img.H - 1)
      return Vec4Set1(0);

  Col = TTP_UIDraw2D->Img.Bits[(TTP_UIDraw2D->Img.H - Y - 1) * TTP_UIDraw2D->Img.W + X];
  return Vec4Set((Col >> 16) & 0xFF, (Col >> 8) & 0xFF, Col & 0xFF, (Col >> 24) & 0xFF);
} /* End of 'TTP_UIPutPixel' function */

/* Put pixel in texture function.
 * ARGUMENTS:
 *   - position of pixel:
 *       INT X, Y;
 *   - color to draw:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutPixel( INT X, INT Y, VEC4 Col )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  if (TTP_UIDraw2D->IsLimitsActive)
  {
    if (X < TTP_UIDraw2D->LimitX1 || X > TTP_UIDraw2D->LimitX2 ||
        Y < TTP_UIDraw2D->LimitY1 || Y > TTP_UIDraw2D->LimitY2)
      return;
  }
  else
    if (X < 0 || X > TTP_UIDraw2D->Img.W - 1 ||
        Y < 0 || Y > TTP_UIDraw2D->Img.H - 1)
      return;

  TTP_UIDraw2D->Img.Bits[(TTP_UIDraw2D->Img.H - Y - 1) * TTP_UIDraw2D->Img.W + X] =
    ((DWORD)Col.W << 24) | ((DWORD)Col.X << 16) | ((DWORD)Col.Y << 8) | (DWORD)Col.Z;
} /* End of 'TTP_UIPutPixel' function */

/* Put line in texture function.
 * ARGUMENTS:
 *   - begin of line:
 *       INT X1, Y1;
 *   - end of line:
 *       INT X2, Y2;
 *   - color to draw:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutLine( INT X1, INT Y1, INT X2, INT Y2, VEC4 Col )
{
  INT X, Y, IncrE, IncrNE, F, dx, dy, sx, count, tmp;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  if (Y2 < Y1)
    COM_SWAP(X1, X2, tmp), COM_SWAP(Y1, Y2, tmp);
  dy = Y2 - Y1;
  dx = X2 - X1;
  if (dx < 0)
    dx = -dx, sx = -1;
  else
    sx = 1;

  X = X1;
  Y = Y1;
  TTP_UIPutPixel(X, Y, Col);

  if (dx >= dy)
  {
    IncrE = 2 * dy;
    IncrNE = 2 * dy - 2 * dx;
    F = 2 * dy - dx;
    count = dx;
    while (count-- > 0)
    {
      if (F > 0)
        Y++, F += IncrNE;
      else
        F += IncrE;
      X += sx;
      TTP_UIPutPixel(X, Y, Col);
    }
  }
  else
  {
    IncrE = 2 * dx;
    IncrNE = 2 * dx - 2 * dy;
    F = 2 * dx - dy;
    count = dy;
    while (count-- > 0)
    {
      if (F > 0)
        X += sx, F += IncrNE;
      else
        F += IncrE;
      Y++;
      TTP_UIPutPixel(X, Y, Col);
    }
  }
} /* End of 'TTP_UIPutLine' function */

/* Put rectangle in texture function.
 * ARGUMENTS:
 *   - left top corner position:
 *       INT X1, Y1;
 *   - right bottom corner position:
 *       INT X2, Y2;
 *   - color to draw border:
 *       VEC4 ColBorder;
 *   - color to fill rectangle:
 *       VEC4 ColFill;
 * RETURNS: None.
 */
static VOID TTP_UIPutRect( INT X1, INT Y1, INT X2, INT Y2,
                           VEC4 ColBorder, VEC4 ColFill )
{
  INT x, y;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  /* Fill rectangle */
  for (y = Y1 + 1; y < Y2; y++)
    for (x = X1 + 1; x < X2; x++)
      TTP_UIPutPixel(x, y, ColFill);

  /* Draw border */
  TTP_UIPutLine(X1, Y1, X2, Y1, ColBorder);
  TTP_UIPutLine(X2, Y1, X2, Y2, ColBorder);
  TTP_UIPutLine(X2, Y2, X1, Y2, ColBorder);
  TTP_UIPutLine(X1, Y2, X1, Y1, ColBorder);
} /* End of 'TTP_UIPutRect' function */

/* Put rect which clear BG in texture function.
 * ARGUMENTS:
 *   - color to fill rectangle:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutClearBG( VEC4 Col )
{
  TTP_UIPutRect(0, 0, TTP_UIDraw2D->Img.W, TTP_UIDraw2D->Img.H, Col, Col);
} /* End of 'TTP_UIPutClearBG' function */

/* Put 4 points depended on center symmetry function.
 * ARGUMENTS:
 *   - center of circle position:
 *       INT X0, Y0;
 *   - point position:
 *       INT X, Y;
 *   - color of circle:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPut4Points( INT X0, INT Y0, INT X, INT Y, VEC4 Col )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  TTP_UIPutPixel(X + X0, Y + Y0, Col);
  TTP_UIPutPixel(-X + X0, -Y + Y0, Col);
  TTP_UIPutPixel(Y + X0, X + Y0, Col);
  TTP_UIPutPixel(-Y + X0, -X + Y0, Col);
} /* End of 'TTP_UIPut4Points' function */

/* Put 8 points depended on center symmetry function.
 * ARGUMENTS:
 *   - center of circle position:
 *       INT X0, Y0;
 *   - point position:
 *       INT X, Y;
 *   - color of circle:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPut8Points( INT X0, INT Y0, INT X, INT Y, VEC4 Col )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  TTP_UIPutPixel(X + X0, Y + Y0, Col);
  TTP_UIPutPixel(X + X0, -Y + Y0, Col);
  TTP_UIPutPixel(-X + X0, Y + Y0, Col);
  TTP_UIPutPixel(-X + X0, -Y + Y0, Col);

  TTP_UIPutPixel(Y + X0, X + Y0, Col);
  TTP_UIPutPixel(-Y + X0, X + Y0, Col);
  TTP_UIPutPixel(Y + X0, -X + Y0, Col);
  TTP_UIPutPixel(-Y + X0, -X + Y0, Col);
} /* End of 'TTP_UIPut8Points' function */

/* Put circle in texture function.
 * ARGUMENTS:
 *   - center of circle position:
 *       INT X0, Y0;
 *   - radius of circle:
 *       INT R;
 *   - color of circle:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutCircle( INT X0, INT Y0, INT R, VEC4 Col )
{
  INT F, IncrE, IncrSE, X, Y;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  F = 1 - R;
  IncrE = 3;
  IncrSE = 5 - 2 * R;
  X = 0, Y = R;

  TTP_UIPut4Points(X0, Y0, X, Y, Col);
  while (TRUE)
  {
    if (F >= 0)
      Y--, F += IncrSE, IncrSE += 4;
    else
      F += IncrE, IncrSE += 2;
    IncrE += 2;
    X++;
    if (X > Y)
      break;
    if (X != Y)
      TTP_UIPut8Points(X0, Y0, X, Y, Col);
  }
} /* End of 'TTP_UIPutCircle' function */

/* Put filled circle in texture function.
 * ARGUMENTS:
 *   - center of circle position:
 *       INT X0, Y0;
 *   - radius of circle:
 *       INT R;
 *   - color of circle:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutFilledCircle( INT X0, INT Y0, INT R, VEC4 Col )
{
  INT i, j;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  for (i = X0 - R; i <= X0 + R; i++)
    for (j = Y0 - R; j <= Y0 + R; j++)
      if ((i - X0) * (i - X0) + (j - Y0) * (j - Y0) <= R * R)
        TTP_UIPutPixel(i, j, Col);
} /* End of 'TTP_UIPutFilledCircle' function */

/* Put torus in texture function.
 * ARGUMENTS:
 *   - center of torus position:
 *       INT X0, Y0;
 *   - radius of torus:
 *       INT R;
 *   - length of the middle part:
 *       INT Len;
 *   - color of torus:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutTorus( INT X0, INT Y0, INT R, INT Len, VEC4 Col )
{
  INT i, j;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  for (i = 0; i < R * 2 + Len; i++)
    for (j = 0; j < R * 2; j++)
      if (i <= R)
      {
        if ((i - R) * (i - R) + (j - R) * (j - R) - R * R < 0.1)
          TTP_UIPutPixel(X0 + i, Y0 + j, Col);
      }
      else if (i >= R + Len)
      {
        if ((i - R - Len) * (i - R - Len) + (j - R) * (j - R) - R * R < 0.1)
          TTP_UIPutPixel(X0 + i, Y0 + j, Col);
      }
      else
        TTP_UIPutPixel(X0 + i, Y0 + j, Col);
} /* End of 'TTP_UIPutTorus' function */

/* Put rectangle with round corners (smooth) in texture function.
 * ARGUMENTS:
 *   - left top corner position:
 *       INT X1, Y1;
 *   - right bottom corner position:
 *       INT X2, Y2;
 *   - radius of smoothness:
 *       INT R;
 *   - color to draw border:
 *       VEC4 ColBorder;
 *   - color to fill rectangle:
 *       VEC4 ColFill;
 * RETURNS: None.
 */
static VOID TTP_UIPutSmoothRect( INT X1, INT Y1, INT X2, INT Y2, INT R,
                                 VEC4 ColBorder, VEC4 ColFill )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  /* Corners (circles) */
  TTP_UIPutFilledCircle(X1 + R, Y1 + R, R, ColFill);
  TTP_UIPutCircle(X1 + R, Y1 + R, R, ColBorder);

  TTP_UIPutFilledCircle(X2 - R, Y1 + R, R, ColFill);
  TTP_UIPutCircle(X2 - R, Y1 + R, R, ColBorder);

  TTP_UIPutFilledCircle(X2 - R, Y2 - R, R, ColFill);
  TTP_UIPutCircle(X2 - R, Y2 - R, R, ColBorder);

  TTP_UIPutFilledCircle(X1 + R, Y2 - R, R, ColFill);
  TTP_UIPutCircle(X1 + R, Y2 - R, R, ColBorder);

  /* Center of smooth rectangle */
  TTP_UIPutRect(X1 + R, Y1, X2 - R, Y2, ColFill, ColFill);
  TTP_UIPutRect(X1, Y1 + R, X2, Y2 - R, ColFill, ColFill);

  /* Additional borders */
  TTP_UIPutLine(X1, Y1 + R, X1, Y2 - R, ColBorder);
  TTP_UIPutLine(X2, Y1 + R, X2, Y2 - R, ColBorder);
  TTP_UIPutLine(X1 + R, Y1, X2 - R, Y1, ColBorder);
  TTP_UIPutLine(X1 + R, Y2, X2 - R, Y2, ColBorder);
} /* End of 'TTP_UIPutSmoothRect' function */

/* Put rectangle with 2 round corners (pill-form) in texture function.
 * ARGUMENTS:
 *   - left top corner position:
 *       INT X1, Y1;
 *   - right bottom corner position:
 *       INT X2, Y2;
 *   - color to draw border:
 *       VEC4 ColBorder;
 *   - color to fill rectangle:
 *       VEC4 ColFill;
 * RETURNS: None.
 */
static VOID TTP_UIPutPill( INT X1, INT Y1, INT X2, INT Y2,
                           VEC4 ColBorder, VEC4 ColFill )
{
  INT R = (Y2 - Y1) / 2;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  /* Put 2 circles */
  TTP_UIPutFilledCircle(X1 + R, Y1 + R, R, ColFill);
  TTP_UIPutCircle(X1 + R, Y1 + R, R, ColBorder);

  TTP_UIPutFilledCircle(X2 - R, Y2 - R, R, ColFill);
  TTP_UIPutCircle(X2 - R, Y2 - R, R, ColBorder);

  /* Center of pill */
  TTP_UIPutRect(X1 + R, Y1, X2 - R, Y2, ColFill, ColFill);

  /* Additional borders of pill */
  TTP_UIPutLine(X1 + R, Y1, X2 - R, Y1, ColBorder);
  TTP_UIPutLine(X1 + R, Y2, X2 - R, Y2, ColBorder);
} /* End of 'TTP_UIPutPill' function */

/* Set font size in current 2D drawing structure function.
 * ARGUMENTS:
 *   - size of font:
 *       INT Size;
 * RETURNS: None.
 */
static VOID TTP_UIPutTextSetSize( INT Size )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  TTP_UIDraw2D->Size = Size;
  TTP_UIFontSet(TTP_UIDraw2D);
} /* End of 'TTP_UIPutTextSize' function */

/* Set font size in current 2D drawing structure function.
 * ARGUMENTS:
 *   - font name (can be taken from MS Word):
 *       CHAR *FontName;
 *   - bit flags (see TTP_FONT params):
 *       DWORD BitFlags;
 * RETURNS: None.
 */
static VOID TTP_UIPutTextSetFont( CHAR *FontName, DWORD BitFlags )
{
  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  strncpy(TTP_UIDraw2D->FontName, FontName, TTP_MAX_NAME);
  TTP_UIDraw2D->Params = BitFlags;
  TTP_UIFontSet(TTP_UIDraw2D);
} /* End of 'TTP_UIPutTextSetFont' function */

/* Put text by DrawText WinAPI function.
 * ARGUMENTS:
 *   - text to put in window:
 *       CHAR *Str;
 *   - position of text:
 *       INT X, Y;
 *   - color to draw:
 *       VEC4 Col;
 * RETURNS: None.
 */
static VOID TTP_UIPutText( CHAR *Str, INT X, INT Y, VEC4 Col )
{
  HDC hDC, hDCDest;
  HBITMAP hBm;
  HFONT hFntOld;
  RECT rc;
  INT i;
  BITMAPINFOHEADER bmi = {0};
  DWORD *Bits;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  hDC = CreateCompatibleDC(Ttp->hDC);
  GetWindowRect(Ttp->hWnd, &rc);
  rc.left = rc.top = 0;
  hFntOld = SelectObject(hDC, TTP_UIDraw2D->hFnt);

  DrawText(hDC, Str, -1, &rc, DT_CALCRECT);

  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biWidth = rc.right;
  bmi.biHeight = rc.bottom * (TTP_UIDraw2D->ToReverseText ? 1 : -1);
  bmi.biBitCount = 32;
  bmi.biPlanes = 1;

  hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, (VOID **)&Bits, NULL, 0); 

  SelectObject(hDC, hBm);
  hDCDest = CreateCompatibleDC(Ttp->hDC);
  SelectObject(hDCDest, TTP_UIDraw2D->Img.hBm);

  BitBlt(hDC, 0, 0, rc.right, rc.bottom, hDCDest, X, Y, SRCCOPY);

  SetTextColor(hDC, RGB(Col.X, Col.Y, Col.Z));
  SetBkMode(hDC, TRANSPARENT);
  DrawText(hDC, Str, -1, &rc, DT_NOCLIP);
  SelectObject(hDC, hFntOld);

  for (i = 0; i < rc.right * rc.bottom; i++)
    if (Bits[i] != 0)
      Bits[i] |= (DWORD)Col.W << 24;

  BitBlt(hDCDest, X, Y, rc.right, rc.bottom, hDC, 0, 0, SRCCOPY);

  DeleteDC(hDC);
  DeleteDC(hDCDest);
  DeleteObject(hBm);
} /* End of 'TTP_UIPutText' function */

/* Put windows control by WinAPI function.
 * ARGUMENTS:
 *   - left and top coordinates:
 *       INT X1, Y1;
 *   - width and height of element:
 *       INT W, H;
 *   - control type (see TTP_UI_WIN defines):
 *       DWORD ControlType;
 *   - control state (see TTP_UI_WIN defines):
 *       DWORD ControlState;
 * RETURNS: None.
 */
static VOID TTP_UIPutWinControl( INT X1, INT Y1, INT W, INT H, DWORD ControlType, DWORD ControlState )
{
  HDC hDC = CreateCompatibleDC(Ttp->hDC);
  RECT rc;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  rc.left = X1;
  rc.top = Y1;
  rc.right = X1 + W;
  rc.bottom = Y1 + H;

  SelectObject(hDC, TTP_UIDraw2D->Img.hBm);
  DrawFrameControl(hDC, &rc, ControlType, ControlState);
  DeleteDC(hDC);
} /* End of 'TTP_UIPutWinControl' function */

/* Image get pixel color vector function.
 * ARGUMENTS:
 *   - image:
 *       ttpIMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (VEC4) color vector of pixel.
 */
static VEC4 TTP_UIImgGetPixel( ttpIMAGE *Img, INT X, INT Y )
{
  DWORD col;

  if (Img == NULL ||
      X < 0 || X >= Img->W ||
      Y < 0 || Y >= Img->H)
    return Vec4Set1(0);

  col = Img->Bits[Y * Img->W + X];

  switch (Img->BitCount)
  {
  case 32:
    return Vec4Set((col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF, (col >> 24) & 0xFF);
  case 24:
    return Vec4Set((col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF, 255);
  default:
    return Vec4Set((col >> 16) & 0xFF, (col >> 16) & 0xFF, (col >> 16) & 0xFF, 255);
  }
} /* End of 'TTP_UIImgGetPixel' function */

/* Put image in texture function.
 * ARGUMENTS:
 *   - image to draw:
 *       ttpIMAGE *Img;
 *   - left top corner position:
 *       INT X, Y;
 *   - sizes of image (0 - image source sizes, otherwise it will be scaled):
 *       INT W, H;
 * RETURNS: None.
 */
static VOID TTP_UIPutImg( ttpIMAGE *Img, INT X, INT Y, INT W, INT H )
{
  INT
    x, y,
    nh = H == 0 ? Img->H : H,
    nw = W == 0 ? Img->W : W;
  DBL
    kh = H == 0 ? 1 : (DBL)Img->H / H,
    kw = W == 0 ? 1 : (DBL)Img->W / W;
  VEC4 c;

  if (TTP_UIDraw2D == NULL)
    TTP_UIDraw2D = TTP_UIRenderDraw2D;

  if (Img == NULL)
    return;
  for (y = 0; y < nh; y++)
    for (x = 0; x < nw; x++)
    {
      c = TTP_UIImgGetPixel(Img, x * kw, y * kh);
      if (c.W != 0)
        TTP_UIPutPixel(X + x, Y + y, c);
    }
} /* End of 'TTP_UIPutImg' function */

/* End draw to texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIDrawEnd( VOID )
{
  /*
  if (TTP_UIDraw2D->Tex != NULL)
    Ttp->TexFree(TTP_UIDraw2D->Tex);
  TTP_UIDraw2D->Tex = Ttp->TexAddImg(&TTP_UIDraw2D->Img);
  TTP_UIDraw2D = NULL;
  */
  Ttp->TexUpdate(TTP_UIDraw2D->Tex, &TTP_UIDraw2D->Img);
  TTP_UIDraw2D = NULL;
} /* End of 'TTP_UIDrawEnd' function */

/* UI draw to texture pointers initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIDrawInit( VOID )
{
  ttpMATERIAL_PATTERN *MtlPat;
  ttpMATERIAL *Mtl;

  TTP_UIDraw2D = NULL;

  Ttp->Draw2DCreate = TTP_UIDraw2DCreate;
  Ttp->Draw2DFree = TTP_UIDraw2DFree;
  Ttp->Draw2DReverse = TTP_UIDraw2DReverse;
  Ttp->Draw2DToScreen =  TTP_UIDraw2DToScreen;
  Ttp->DrawStart = TTP_UIDrawStart;
  Ttp->PutLimitsSet = TTP_UIPutLimitsSet;
  Ttp->PutLimitsDef = TTP_UIPutLimitsDef;
  Ttp->PutLimitsOff = TTP_UIPutLimitsOff;
  Ttp->PutLimitsOn = TTP_UIPutLimitsOn;
  Ttp->PutPixel = TTP_UIPutPixel;
  Ttp->PutLine = TTP_UIPutLine;
  Ttp->PutRect = TTP_UIPutRect;
  Ttp->PutClearBG = TTP_UIPutClearBG;
  Ttp->PutSmoothRect = TTP_UIPutSmoothRect;
  Ttp->PutPill = TTP_UIPutPill;
  Ttp->PutCircle = TTP_UIPutCircle;
  Ttp->PutFilledCircle = TTP_UIPutFilledCircle;
  Ttp->PutTorus = TTP_UIPutTorus;
  Ttp->PutTextSetSize = TTP_UIPutTextSetSize;
  Ttp->PutTextSetFont = TTP_UIPutTextSetFont;
  Ttp->PutText = TTP_UIPutText;
  Ttp->PutWinControl = TTP_UIPutWinControl;
  Ttp->DrawEnd = TTP_UIDrawEnd;
  Ttp->PutImg = TTP_UIPutImg;
  TTP_UIDraw2DUBO = Ttp->UBOCreate("UBO Draw2D", TTP_RND_RES_UBO_BIND_DRAW2D, sizeof(VEC4), NULL);
  Ttp->UBOApply(TTP_UIDraw2DUBO);

  MtlPat = Ttp->MtlPatCreate("Draw2D material pattern", "", "", "draw2d", 1);
  Mtl = Ttp->MtlCreateTex("Draw2D material", MtlPat, "", NULL, 0, TRUE);
  TTP_UIDraw2DPrim = Ttp->PrimCreate(Mtl, TTP_RND_PRIM_POINTS, NULL, 1, NULL, 0);
} /* End of 'TTP_UIDrawInit' function */

/* UI draw to texture pointers deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIDrawClose( VOID )
{
  TTP_UIDraw2D = NULL;

  Ttp->UBOFree(TTP_UIDraw2DUBO);

  Ttp->Draw2DCreate =  NULL;
  Ttp->Draw2DFree = NULL;
  Ttp->Draw2DReverse = NULL;
  Ttp->Draw2DToScreen =  NULL;
  Ttp->DrawStart = NULL;
  Ttp->PutLimitsSet = NULL;
  Ttp->PutLimitsDef = NULL;
  Ttp->PutLimitsOff = NULL;
  Ttp->PutLimitsOn = NULL;
  Ttp->PutPixel = NULL;
  Ttp->PutLine = NULL;
  Ttp->PutRect = NULL;
  Ttp->PutClearBG = NULL;
  Ttp->PutSmoothRect = NULL;
  Ttp->PutPill = NULL;
  Ttp->PutCircle = NULL;
  Ttp->PutFilledCircle = NULL;
  Ttp->PutTorus = NULL;
  Ttp->PutTextSetSize = NULL;
  Ttp->PutTextSetFont = NULL;
  Ttp->PutText = NULL;
  Ttp->PutWinControl = NULL;
  Ttp->DrawEnd = NULL;
  Ttp->PutImg = NULL;
} /* End of 'TTP_UIDrawClose' function */

/* END OF 'ttp_ui_tex.c' FILE */
