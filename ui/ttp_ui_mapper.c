/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_ui_mapper.c
 * PURPOSE     : Tough Third Planet project.
 *               User interface mapper.
 * PROGRAMMER  : CGSG'2022.
 *               Alice Perfileva (AP4),
 *               Daniil Shapovalov (DS5).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"

/* Point in rectangle check */
#define POINT_IN_RECT(Px, Py, Rx, Ry, W, H) \
                 (((Px) >= (Rx)) && ((Px) <= ((Rx) + W)) && \
                 (((Py) >= (Ry)) && ((Py) <= ((Ry) + H))))
/* Point in circle check */
#define TTP_UI_POINT_IN_CIRCLE(Px, Py, Cx, Cy, R) (((Px) - (Cx)) * ((Px) - (Cx)) + ((Py) - (Cy)) * ((Py) - (Cy)) <= (R) * (R))
#define SLIDER_HEIGHT 2
#define SLIDER_CIRCLE (SLIDER_HEIGHT * 5)

/* Round number to nearest integer function.
 * ARGUMENTS:
 *   - number to round:
 *       DBL X;
 * RETURNS:
 *   (INT) nearest integer.
 */
static INT TTP_UIControlsRound( DBL X )
{
  INT fx = floor(X), cx = ceil(X);

  return (X - fx) < (cx - X) ? fx : cx;
} /* End of 'TTP_UIControlsRound' function */

/* UI control response object function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
VOID TTP_UIControlResponseObject( ttpUI_CONTROL *B );

/* UI control response button function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIControlResponseButton( ttpUI_CONTROL *B )
{
  INT left = B->Left, top = B->Top, leftTab = TTP_UIMainTab.Left, topTab = TTP_UIMainTab.Top, xslider;
  DBL r, t;

  if (leftTab < 0)
    leftTab = 0;
  else if (leftTab + TTP_UIMainTab.W >= Ttp->W)
    leftTab = Ttp->W - TTP_UIMainTab.W;
  if (topTab < 0)
    topTab = 0;
  else if (topTab + TTP_UIMainTab.H >= Ttp->H)
    topTab = Ttp->H - TTP_UIMainTab.H;

  if (!B->IsInTab)
  {
    if (left < 0)
      left = 0;
    else if (left + B->W >= Ttp->W)
      left = Ttp->W - B->W;
    if (top < 0)
      top = 0;
    else if (top + B->H >= Ttp->H)
      top = Ttp->H - B->H;
  }

  switch (B->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    /* Pressed */
    if (B->Button.ActiveTime > 0)
      B->Button.ActiveTime -= Ttp->GlobalDeltaTime;
    B->Button.IsPressed = FALSE;

    if (!TTP_UIControlMousePrev && Ttp->Anim.Input.Keys[VK_LBUTTON])
      if (POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                        left, top, B->W, B->H))
        B->Button.IsActive = TRUE;
      else
        B->Button.IsActive = FALSE;

    if (B->Button.IsActive && !Ttp->Anim.Input.Keys[VK_LBUTTON] &&
        POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                      left, top, B->W, B->H))
    {
      INT i;

      B->Button.IsActive = FALSE;
      B->Button.ActiveTime = 0.1;
      B->Button.IsPressed = TRUE;
      for (i = 0; i < B->Button.NumOfElements; i++)
        *(B->Button.Values[i]) = B->Button.IsPressed;
    }
    else
    {
      INT i;

      for (i = 0; i < B->Button.NumOfElements; i++)
        *(B->Button.Values[i]) = B->Button.IsPressed;
    }
    if (B->Button.IsActive && !Ttp->Anim.Input.Keys[VK_LBUTTON])
      B->Button.IsActive = FALSE;
    break;
  case TTP_UI_CONTROL_TYPE_SLIDER:
    r = B->H / 2.0;
    t = ((DBL)B->Slider.Pos - B->Slider.From) / (B->Slider.To - B->Slider.From);
    xslider = left + B->W * t;
    if (!TTP_UIControlMousePrev && Ttp->Anim.Input.Keys[VK_LBUTTON] &&
        (POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                       left, top, B->W, B->H) ||
         TTP_UI_POINT_IN_CIRCLE(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                                xslider, top + B->H / 2, r)))
      B->Slider.IsActive = TRUE;
    else if (TTP_UIControlMousePrev && !Ttp->Anim.Input.Keys[VK_LBUTTON])
      B->Slider.IsActive = FALSE;
    if (B->Slider.IsActive)
    {
      INT i;
      DBL pos1, pos2;

      pos1 = (Ttp->Anim.Input.Mx - left) / (DBL)B->W;
      B->Slider.Pos = B->Slider.From * (1 - pos1) + B->Slider.To * pos1;
      if (B->Slider.Step != 0)
      {
        if (B->Slider.Step > B->Slider.To)
          B->Slider.Step = B->Slider.To;
        else if (B->Slider.Step < B->Slider.From)
          B->Slider.Step = B->Slider.From;
        pos2 = (TTP_UIControlsRound(B->Slider.Pos / B->Slider.Step) * B->Slider.Step - B->Slider.From);
        if (B->Slider.To - B->Slider.Pos < B->Slider.Pos - pos2)
          B->Slider.Pos = B->Slider.To;
        else
          B->Slider.Pos = pos2;
      }
      if (B->Slider.Pos > B->Slider.To)
        B->Slider.Pos = B->Slider.To;
      else if (B->Slider.Pos < B->Slider.From)
        B->Slider.Pos = B->Slider.From;
      for (i = 0; i < B->Slider.NumOfElementsDBL; i++)
        *(B->Slider.ValuesDBL[i]) = B->Slider.Pos;
      for (i = 0; i < B->Slider.NumOfElementsFLT; i++)
        *(B->Slider.ValuesFLT[i]) = (FLT)B->Slider.Pos;
    }
    break;
  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    /* Clicked */
    if (Ttp->Anim.Input.KeysClick[VK_LBUTTON] &&
        POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                      left, top, B->W, B->H))
    {
      INT i;

      B->Checkbox.IsOn = !B->Checkbox.IsOn;
      for (i = 0; i < B->Checkbox.NumOfElements; i++)
        *(B->Checkbox.Values[i]) = B->Checkbox.IsOn;
    }
    break;
  case TTP_UI_CONTROL_TYPE_LIST:
    if (Ttp->Anim.Input.KeysClick[VK_LBUTTON] &&
        POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                      left, top, B->W, B->H))
    {
      INT i;

      B->List.IsOn = !B->List.IsOn;
      if (B->List.NumOfElements != 0)
        if (B->List.IsOn)
          for (i = B->IndexInTab + 1; i < TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->NumOfControls; i++)
            TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->Controls[i]->Top += B->List.Values[B->List.NumOfElements - 1]->Top + B->List.Values[B->List.NumOfElements - 1]->H;
        else
          for (i = B->IndexInTab + 1; i < TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->NumOfControls; i++)
            TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->Controls[i]->Top -= B->List.Values[B->List.NumOfElements - 1]->Top + B->List.Values[B->List.NumOfElements - 1]->H;
    }
    if (B->List.IsOn)
    {
      INT i;

      if (B->IsInTab)
        for (i = 0; i < B->List.NumOfElements; i++)
        {
          B->List.Values[i]->IsInList = FALSE;

          B->List.Values[i]->Top += top + B->H;
          B->List.Values[i]->Left += left;
          if (B->List.Values[i]->Top >= topTab && B->List.Values[i]->Top + B->List.Values[i]->H <= TTP_UIMainTab.H + topTab)
            TTP_UIControlResponseObject(B->List.Values[i]);
          B->List.Values[i]->Top -= top + B->H;
          B->List.Values[i]->Left -= left;

          B->List.Values[i]->IsInList = TRUE;
        }
      else
        for (i = 0; i < B->List.NumOfElements; i++)
        {
          B->List.Values[i]->IsInList = FALSE;

          B->List.Values[i]->Top += top + B->H;
          B->List.Values[i]->Left += left;
          TTP_UIControlResponseObject(B->List.Values[i]);
          B->List.Values[i]->Top -= top + B->H;
          B->List.Values[i]->Left -= left;

          B->List.Values[i]->IsInList = TRUE;
        }
    }
    break;
  }
} /* End of 'TTP_UIControlResponseButton' function */

/* Default UI control render button function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIControlRenderButton( ttpUI_CONTROL *B )
{
  CHAR Buf[30];
  INT r;
  INT screenPos;
  DBL pos;
  VEC4 col;
  INT left = B->Left, top = B->Top;

  if (!B->IsInTab && !B->IsInList)
  {
    if (left < 0)
      left = 0;
    else if (left + B->W >= Ttp->W)
      left = Ttp->W - B->W;
    if (top < 0)
      top = 0;
    else if (top + B->H >= Ttp->H)
      top = Ttp->H - B->H;
  }

  switch (B->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    if (B->NumOfImg >= 2)
    {
      if (B->Button.IsPressed || B->Button.ActiveTime > 0)
        Ttp->UI.DrawToTex.PutImg(B->Images[1], left, top, B->W, B->H);
      else
        Ttp->UI.DrawToTex.PutImg(B->Images[0], left, top, B->W, B->H);
      return;
    }
    if (B->NumOfImg == 1)
    {
      Ttp->UI.DrawToTex.PutImg(B->Images[0], left, top, B->W, B->H);
      return;
    }

    if (B->Button.IsPressed || B->Button.ActiveTime > 0)
      col = VEC4_GREEN;
    else
      col = VEC4_RED;
    if (B->Button.IsActive)
      Ttp->PutRect(left, top, left + B->W, top + B->H, VEC4_BLUE, col);
    else
      Ttp->PutRect(left, top, left + B->W, top + B->H, VEC4_WHITE, col);
    break;

  case TTP_UI_CONTROL_TYPE_SLIDER:
    r = B->H / 2;
    pos = (B->Slider.Pos - B->Slider.From) / (B->Slider.To - B->Slider.From);
    if (pos < 0)
      pos = 0;
    else if (pos > 1)
      pos = 1;
    screenPos = left + B->W * pos;
    Ttp->PutRect(left, top + B->H / 2 - r / 2, screenPos, top + B->H / 2 + r / 2, Vec4Set(34 * 3 / 2, 139 * 3 / 2, 34 * 3 / 2, 255), Vec4Set(34 * 3 / 2, 139 * 3 / 2, 34 * 3 / 2, 255));
    Ttp->PutRect(screenPos, top + B->H / 2 - r / 2, left + B->W, top + B->H / 2 + r / 2, Vec4Set(34, 139, 34, 255), Vec4Set(34, 139, 34, 255));
    Ttp->PutFilledCircle(screenPos, top + B->H / 2, r, Vec4Set(34 * 3 / 2, 139 * 3 / 2, 34 * 3 / 2, 255));
    sprintf(Buf, "%.2f", B->Slider.Pos);
    Ttp->PutTextSetSize(B->H);
    Ttp->PutTextSetFont("Arial", TTP_FONT_BOLD);
    Ttp->PutText(Buf, left, top, Vec4Set1(255));
    break;

  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    if (B->NumOfImg >= 2)
    {
      if (B->Checkbox.IsOn)
        Ttp->UI.DrawToTex.PutImg(B->Images[1], left, top, B->W, B->H);
      else
        Ttp->UI.DrawToTex.PutImg(B->Images[0], left, top, B->W, B->H);
      return;
    }

    if (B->Checkbox.IsOn)
      col = VEC4_GREEN;
    else
      col = VEC4_RED;
    Ttp->PutRect(left, top, left + B->W, top + B->H, Vec4Set(0, 0, 255, 255), col);
    break;

  case TTP_UI_CONTROL_TYPE_LIST:
    break;
  }
} /* End of 'TTP_UIControlRenderButton' function */

/* UI control response round button function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIResponseRoundButton( ttpUI_CONTROL *B )
{
  INT left = B->Left, top = B->Top;

  if (left < 0)
    left = 0;
  else if (left + B->W >= Ttp->W)
    left = Ttp->W - B->W;
  if (top < 0)
    top = 0;
  else if (top + B->H >= Ttp->H)
    top = Ttp->H - B->H;

  switch (B->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    /* Pressed */
    if (B->Button.ActiveTime > 0)
    {
      B->Button.IsPressed = TRUE;
      B->Button.ActiveTime -= Ttp->GlobalDeltaTime;
    }
    else
      B->Button.IsPressed = FALSE;

    if (!TTP_UIControlMousePrev && Ttp->Anim.Input.Keys[VK_LBUTTON])
      if (POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                        left, top, B->W, B->H))
        B->Button.IsActive = TRUE;
      else
        B->Button.IsActive = FALSE;

    if (B->Button.IsActive && !Ttp->Anim.Input.Keys[VK_LBUTTON] &&
        POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                      left, top, B->W, B->H))
    {
      INT i;

      B->Button.IsActive = FALSE;
      B->Button.ActiveTime = 0.1;
      B->Button.IsPressed = TRUE;
      for (i = 0; i < B->Button.NumOfElements; i++)
        *(B->Button.Values[i]) = B->Button.IsPressed;
    }
    else
    {
      INT i;

      for (i = 0; i < B->Button.NumOfElements; i++)
        *(B->Button.Values[i]) = B->Button.IsPressed;
    }
    if (B->Button.IsActive && !Ttp->Anim.Input.Keys[VK_LBUTTON])
      B->Button.IsActive = FALSE;
    break;

  case TTP_UI_CONTROL_TYPE_SLIDER:
    if (!TTP_UIControlMousePrev && Ttp->Anim.Input.Keys[VK_LBUTTON] &&
        POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
          left, top, B->W, B->H))
      B->Slider.IsActive = TRUE;
    else if (TTP_UIControlMousePrev && !Ttp->Anim.Input.Keys[VK_LBUTTON])
      B->Slider.IsActive = FALSE;
    if (B->Slider.IsActive)
    {
      INT i;
      DBL pos1, pos2;

      pos1 = (Ttp->Anim.Input.Mx - left) / (DBL)B->W;
      B->Slider.Pos = B->Slider.From * (1 - pos1) + B->Slider.To * pos1;
      if (B->Slider.Step != 0)
      {
        if (B->Slider.Step > B->Slider.To)
          B->Slider.Step = B->Slider.To;
        else if (B->Slider.Step < B->Slider.From)
          B->Slider.Step = B->Slider.From;
        pos2 = (TTP_UIControlsRound(B->Slider.Pos / B->Slider.Step) * B->Slider.Step - B->Slider.From);
        if (B->Slider.To - B->Slider.Pos < B->Slider.Pos - pos2)
          B->Slider.Pos = B->Slider.To;
        else
          B->Slider.Pos = pos2;
      }
      if (B->Slider.Pos > B->Slider.To)
        B->Slider.Pos = B->Slider.To;
      else if (B->Slider.Pos < B->Slider.From)
        B->Slider.Pos = B->Slider.From;
      for (i = 0; i < B->Slider.NumOfElementsDBL; i++)
        *(B->Slider.ValuesDBL[i]) = B->Slider.Pos;
      for (i = 0; i < B->Slider.NumOfElementsFLT; i++)
        *(B->Slider.ValuesFLT[i]) = (FLT)B->Slider.Pos;
    }
    break;
  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    /* Clicked */
    if (Ttp->Anim.Input.KeysClick[VK_LBUTTON] &&
        POINT_IN_RECT(Ttp->Anim.Input.Mx, Ttp->Anim.Input.My,
                      left, top, B->W, B->H))
    {
      INT i;

      B->Checkbox.IsOn = !B->Checkbox.IsOn;
      for (i = 0; i < B->Checkbox.NumOfElements; i++)
        *(B->Checkbox.Values[i]) = B->Checkbox.IsOn;
    }
    break;
  case TTP_UI_CONTROL_TYPE_LIST:
    break;
  }
} /* End of 'TTP_UIResponseRoundButton' function */

/* UI control render button function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIRenderRoundButton( ttpUI_CONTROL *B )
{
  INT xc, yc, i, NumOfC;
  VEC4 col;
  DBL pos;
  INT left = B->Left, top = B->Top;
  static INT flag = 0;

  switch (B->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    if (B->Button.IsPressed)
      col = VEC4_GREEN;
    else
      col = VEC4_RED;
    Ttp->PutFilledCircle(B->Left + B->W / 2, B->Top + B->W / 2, B->W / 2, col);
    break;
  case TTP_UI_CONTROL_TYPE_SLIDER:
    if (left < 0)
      left = 0;
    else if (left + B->W >= Ttp->W)
      left = Ttp->W - B->W;
    if (top < 0)
      top = 0;
    else if (top + B->H >= Ttp->H)
      top = Ttp->H - B->H;

    /* Changing sizes */
    if (flag == 0)
    {
      B->Top += (SLIDER_CIRCLE - SLIDER_HEIGHT) / 2;
      B->H = SLIDER_CIRCLE;
      flag = 1;
    }

    Ttp->PutTorus(left, top, /* B->Left, B->Top, */ SLIDER_HEIGHT, B->W - SLIDER_HEIGHT, VEC4_GREEN);
    pos = (B->Slider.Pos - B->Slider.From) / (B->Slider.To - B->Slider.From);
    if (pos < 0)
      pos = 0;
    else if (pos > 1)
      pos = 1;
    xc = B->Left + pos * B->Slider.Step * B->W, yc = B->Top - SLIDER_HEIGHT / 2;

    /* Changing size */
    if (xc < B->Left - SLIDER_CIRCLE / 2)
      xc = B->Left - SLIDER_CIRCLE / 2;
    if (xc > B->Left + B->W + SLIDER_CIRCLE / 2)
      xc = B->Left + B->W + SLIDER_CIRCLE / 2;
    xc -= SLIDER_CIRCLE;
    yc -= SLIDER_CIRCLE - SLIDER_HEIGHT;

    /* Draw lines */
    NumOfC = (B->Slider.To - B->Slider.From) / (B->Slider.Step + 1);
    for (i = 1; i < NumOfC; i++)
    {
      Ttp->UI.DrawToTex.PutLine(B->Left + B->W / NumOfC * i, B->Top - (SLIDER_CIRCLE + SLIDER_HEIGHT) / 2,
        B->Left + B->W / NumOfC * i, B->Top + SLIDER_CIRCLE + SLIDER_HEIGHT, VEC4_WHITE);
    }

    Ttp->PutFilledCircle(xc + SLIDER_CIRCLE, yc + SLIDER_CIRCLE, SLIDER_CIRCLE, VEC4_MAGENTA);
    break;
  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    if (B->Checkbox.IsOn)
      col = VEC4_GREEN;
    else
      col = VEC4_GRAY;

    /* Changing sizes */
    if (B->W - B->H < 0)
      B->W = B->H * 1.5;

    Ttp->PutTorus(B->Left, B->Top, B->H / 2, B->W - B->H, col);
    /*
    TTP_PutTorus(B->Left * 0.95, B->Top * 0.95, B->H / 2 * 1.1, (B->W - B->H) * 1.01, VEC4_WHITE);
    TTP_PutTorus(B->Left * 1.05, B->Top * 1.05, B->H / 2 * 0.9, (B->W - B->H) * 0.99, VEC4_WHITE);
    */
    if (B->Checkbox.IsOn)
      Ttp->PutFilledCircle(B->Left + B->H * 0.1 + B->W - B->H + B->H / 2 * 0.8, B->Top + B->H * 0.1 + B->H / 2 * 0.8, B->H / 2 * 0.8, VEC4_WHITE);
    else
      Ttp->PutFilledCircle(B->Left + B->H * 0.1 + B->H / 2 * 0.8,               B->Top + B->H * 0.1 + B->H / 2 * 0.8, B->H / 2 * 0.8, VEC4_WHITE);
    break;
  case TTP_UI_CONTROL_TYPE_LIST:
    break;
  }
} /* End of 'TTP_UIRenderRoundButton' function */

/* UI control close button function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIControlCloseButton( ttpUI_CONTROL *B )
{
  if (B->Images != NULL)
    free(B->Images);
  B->Images = NULL;
  B->NumOfImg = B->MaxNumOfImg = 0;
  switch (B->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    if (B->Button.Values != NULL)
      free(B->Button.Values);
    B->Button.Values = NULL;
    B->Button.NumOfElements = B->Button.MaxNumOfElements = 0;
    break;
  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    if (B->Checkbox.Values != NULL)
      free(B->Checkbox.Values);
    B->Checkbox.Values = NULL;
    B->Checkbox.NumOfElements = B->Checkbox.MaxNumOfElements = 0;
    break;
  case TTP_UI_CONTROL_TYPE_SLIDER:
    if (B->Slider.ValuesDBL != NULL)
      free(B->Slider.ValuesDBL);
    B->Slider.ValuesDBL = NULL;
    B->Slider.NumOfElementsDBL = B->Slider.MaxNumOfElementsDBL = 0;

    if (B->Slider.ValuesFLT != NULL)
      free(B->Slider.ValuesFLT);
    B->Slider.ValuesFLT = NULL;
    B->Slider.NumOfElementsFLT = B->Slider.MaxNumOfElementsFLT = 0;
    break;
  case TTP_UI_CONTROL_TYPE_LIST:
    if (B->List.Values != NULL)
      free(B->List.Values);
    B->List.Values = NULL;
    B->List.NumOfElements = B->List.MaxNumOfElements = 0;
    break;
  }
} /* End of 'TTP_UIControlCloseButton' function */

/***
 * Create Objects
 ***/

/* UI create button function.
 * ARGUMENTS:
 *   - coordinates of corners & sizes:
 *       INT Left, Top, W, H;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created button.
 */
static ttpUI_CONTROL * TTP_UICreateButton( INT Left, INT Top, INT W, INT H )
{
  ttpUI_CONTROL *B;

  /* Add new button */
  B = Ttp->UIAddObj();

  B->Left = Left;
  B->Top = Top;
  B->W = W;
  B->H = H;

  B->Type = TTP_UI_CONTROL_TYPE_BUTTON;
  B->Button.IsActive = FALSE;
  B->Button.IsPressed = FALSE;
  B->Button.ActiveTime = 0;
  B->Button.NumOfElements = 0;
  B->Button.MaxNumOfElements = 0;
  B->Button.Values = NULL;

  B->ResponseObject = TTP_UIControlResponseButton;
  B->RenderObject = TTP_UIControlRenderButton;
  B->CloseObject = TTP_UIControlCloseButton;
  B->NumOfImg = 0;
  B->Images = NULL;
  B->IsInList = FALSE;
  B->IsInTab = FALSE;
  return B;
} /* End of 'TTP_UICreateButton' function */

/* UI create slider function.
 * ARGUMENTS:
 *   - coordinates of corners & sizes:
 *       INT Left, Top, W, H;
 *   - step:
 *       DBL Step;
 *   - from/to:
 *       DBL From, To;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created slider.
 */
static ttpUI_CONTROL * TTP_UICreateSlider( INT Left, INT Top, INT W, INT H, DBL Step, DBL From, DBL To )
{
  ttpUI_CONTROL *B;

  /* Add new slider */
  B = Ttp->UIAddObj();

  B->Left = Left;
  B->Top = Top;
  B->W = W;
  B->H = H;

  B->Type = TTP_UI_CONTROL_TYPE_SLIDER;
  B->Slider.Pos = 0.5;
  B->Slider.NumOfElementsDBL = 0;
  B->Slider.MaxNumOfElementsDBL = 0;
  B->Slider.ValuesDBL = NULL;
  B->Slider.NumOfElementsFLT = 0;
  B->Slider.MaxNumOfElementsFLT = 0;
  B->Slider.ValuesFLT = NULL;

  B->Slider.Step = Step;
  B->Slider.From = From;
  B->Slider.To = To;
  B->ResponseObject = TTP_UIControlResponseButton;
  B->RenderObject = TTP_UIControlRenderButton;
  B->CloseObject = TTP_UIControlCloseButton;
  B->NumOfImg = 0;
  B->Images = NULL;
  B->IsInList = FALSE;
  B->IsInTab = FALSE;
  return B;
} /* End of 'TTP_UICreateSlider' function */

/* UI create checkbox function.
 * ARGUMENTS:
 *   - coordinates of corners & sizes:
 *       INT Left, Top, W, H;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created checkbox.
 */
static ttpUI_CONTROL * TTP_UICreateCheckBox( INT Left, INT Top, INT W, INT H )
{
  ttpUI_CONTROL *B;

  /* Add new button */
  B = Ttp->UIAddObj();
  
  B->Left = Left;
  B->Top = Top;
  B->W = W;
  B->H = H;

  B->Type = TTP_UI_CONTROL_TYPE_CHECKBOX;
  B->Checkbox.NumOfElements = 0;
  B->Checkbox.MaxNumOfElements = 0;
  B->Checkbox.Values = NULL;
  B->Checkbox.IsOn = FALSE;

  B->ResponseObject = TTP_UIControlResponseButton;
  B->RenderObject = TTP_UIControlRenderButton;
  B->CloseObject = TTP_UIControlCloseButton;
  B->NumOfImg = 0;
  B->Images = NULL;
  B->IsInList = FALSE;
  B->IsInTab = FALSE;
  return B;
} /* End of 'TTP_UICreateCheckBox' function */

/* UI create list function.
 * ARGUMENTS:
 *   - coordinates of corners & sizes:
 *       INT Left, Top, W, H;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created checkbox.
 */
static ttpUI_CONTROL * TTP_UICreateList( INT Left, INT Top, INT W, INT H )
{
  ttpUI_CONTROL *B;

  /* Add new button */
  B = Ttp->UIAddObj();

  B->Left = Left;
  B->Top = Top;
  B->W = W;
  B->H = H;

  B->Type = TTP_UI_CONTROL_TYPE_LIST;
  B->List.NumOfElements = 0;
  B->List.MaxNumOfElements = 0;
  B->List.Values = NULL;
  B->List.IsOn = FALSE;

  B->ResponseObject = TTP_UIControlResponseButton;
  B->RenderObject = TTP_UIControlRenderButton;
  B->CloseObject = TTP_UIControlCloseButton;
  B->NumOfImg = 0;
  B->Images = NULL;
  B->IsInList = FALSE;
  B->IsInTab = FALSE;
  return B;
} /* End of 'TTP_UICreateList' function */

/* UI create tab object function.
 * ARGUMENTS:
 *   - tab name:
 *       CHAR *Name;
 * RETURNS: 
 *   (ttpUI_CONTROL_TAB_OBJECT *) created tab object.
 */
static ttpUI_CONTROL_TAB_OBJECT * TTP_UICreateTabObj( CHAR *Name )
{
  ttpUI_CONTROL_TAB_OBJECT *Tab;

  if ((Tab = malloc(sizeof(ttpUI_CONTROL_TAB_OBJECT))) == NULL)
    return NULL;
  Tab->NumOfControls = 0;
  Tab->MaxNumOfControls = 0;
  Tab->Controls = NULL;
  Tab->Img = NULL;
  strncpy(Tab->Name, Name, TTP_UI_TAB_MAX_NAME);
  return Tab;
} /* End of 'TTP_UICreateTabObj' function */

/***
 * Create Objects by default
 ***/

/* UI create button by default function.
 * ARGUMENTS:
 *   - button to create by default:
 *       ttpUI_CONTROL *B;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created button.
 */
static ttpUI_CONTROL * TTP_UICreateDefButton( VOID )
{
  ttpUI_CONTROL *B;

  B = Ttp->UIAddObj();
  
  B->Left = 0;
  B->Top = 0;
  B->W = 100;
  B->H = 100;

  B->Type = TTP_UI_CONTROL_TYPE_BUTTON;
  B->Button.IsActive = FALSE;
  B->Button.IsPressed = FALSE;
  B->Button.ActiveTime = 0;
  B->Button.NumOfElements = 0;
  B->Button.MaxNumOfElements = 0;
  B->Button.Values = NULL;
  B->RenderObject = Ttp->UIControlRenderButton;
  B->CloseObject = TTP_UIControlCloseButton;
  B->NumOfImg = 0;
  B->Images = NULL;
  B->IsInList = FALSE;
  B->IsInTab = FALSE;
  return B;
} /* End of 'TTP_UICreateDefButton' function */

/* UI create slider by default function.
 * ARGUMENTS:
 *   - slider to create by default:
 *       ttpUI_CONTROL *B;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created button.
 */
static ttpUI_CONTROL * TTP_UICreateDefSlider( VOID )
{
  ttpUI_CONTROL *B2;

  B2 = Ttp->UI.UIMapper.UIAddObj();

  B2->Left = 0;
  B2->Top = 0;
  B2->W = 100;
  B2->H = 100;

  B2->Type = TTP_UI_CONTROL_TYPE_SLIDER;
  B2->Slider.Pos = 0.5;
  B2->Slider.NumOfElementsDBL = 0;
  B2->Slider.MaxNumOfElementsDBL = 0;
  B2->Slider.ValuesDBL = NULL;
  B2->Slider.NumOfElementsFLT = 0;
  B2->Slider.MaxNumOfElementsFLT = 0;
  B2->Slider.ValuesFLT = NULL;

  B2->Slider.Step = 30;
  B2->Slider.From = 0;
  B2->Slider.To = 100;
  B2->RenderObject = Ttp->UIControlRenderButton;
  B2->CloseObject = TTP_UIControlCloseButton;
  B2->NumOfImg = 0;
  B2->Images = NULL;
  B2->IsInList = FALSE;
  B2->IsInTab = FALSE;
  return B2;
} /* End of 'TTP_UICreateDefSlider' function */

/* UI create checkbox by default function.
 * ARGUMENTS:
 *   - slider to create by default:
 *       ttpUI_CONTROL *B;
 * RETURNS: 
 *   (ttpUI_CONTROL *) created button.
 */
static ttpUI_CONTROL * TTP_UICreateDefCheckBox( VOID )
{
  ttpUI_CONTROL *B1;

  B1 = Ttp->UI.UIMapper.UIAddObj();

  B1->Left = 0;
  B1->Top = 0;
  B1->W = 100;
  B1->H = 100;

  B1->Type = TTP_UI_CONTROL_TYPE_CHECKBOX;
  B1->Checkbox.NumOfElements = 0;
  B1->Checkbox.MaxNumOfElements = 0;
  B1->Checkbox.Values = NULL;
  B1->Checkbox.IsOn = FALSE;
  
  B1->RenderObject = Ttp->UIControlRenderButton;
  B1->CloseObject = TTP_UIControlCloseButton;
  B1->NumOfImg = 0;
  B1->Images = NULL;
  B1->IsInList = FALSE;
  B1->IsInTab = FALSE;
  return B1;
} /* End of 'TTP_UICreateDefCheckBox' function */

/* UI add object function.
 * ARGUMENTS: None.
 * RETURNS: 
 *   (ttpUI_CONTROL *) added object.
 */
static ttpUI_CONTROL * TTP_UIAddObj( VOID )
{
  ttpUI_CONTROL *O;

  O = TTP_StockAlloc(&TTP_UIControlObjects);
  return O;
} /* End of 'TTP_UIAddObj' function */

/* UI delete object function.
 * ARGUMENTS:
 *   - object to delete:
 *       ttpUI_CONTROL *O;
 * RETURNS: None.
 */
static VOID TTP_UIDelObj( ttpUI_CONTROL *O )
{
  TTP_StockDel(O);
} /* End of 'TTP_UIDelObj' function */

/***
 * Add value functions
 ***/

/* UI add bool value function.
 * ARGUMENTS:
 *   - object where to add:
 *       ttpUI_CONTROL *O;
 *   - bool value to add:
 *       BOOL *P;
 * RETURNS: None.
 */
static VOID TTP_UIAddBoolValue( ttpUI_CONTROL *O, BOOL *P )
{
  switch (O->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    if (O->Button.NumOfElements >= O->Button.MaxNumOfElements)
    {
      BOOL **New;

      if ((New = malloc(sizeof(BOOL *) * (O->Button.MaxNumOfElements + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
        return;
      O->Button.MaxNumOfElements += TTP_UI_ALLOCATION_ELEMENTS;
      if (O->Button.Values != NULL)
      {
        memcpy(New, O->Button.Values, sizeof(BOOL *) * O->Button.NumOfElements);
        free(O->Button.Values);
      }
      O->Button.Values = New;
    }
    O->Button.Values[O->Button.NumOfElements++] = P;
    break;
  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    if (O->Checkbox.NumOfElements >= O->Checkbox.MaxNumOfElements)
    {
      BOOL **New;

      if ((New = malloc(sizeof(BOOL *) * (O->Checkbox.MaxNumOfElements + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
        return;
      O->Checkbox.MaxNumOfElements += TTP_UI_ALLOCATION_ELEMENTS;
      if (O->Checkbox.Values != NULL)
      {
        memcpy(New, O->Checkbox.Values, sizeof(BOOL *) * O->Checkbox.NumOfElements);
        free(O->Checkbox.Values);
      }
      O->Checkbox.Values = New;
    }
    O->Checkbox.Values[O->Checkbox.NumOfElements++] = P;
    break;
  }
} /* End of 'TTP_UIAddBoolValue' function */

/* UI add double value function.
 * ARGUMENTS:
 *   - object where to add:
 *       ttpUI_CONTROL *O;
 *   - DBL value to add:
 *       DBL *P;
 * RETURNS: None.
 */
static VOID TTP_UIAddDblValue( ttpUI_CONTROL *O, DBL *P )
{
  if (O->Type == TTP_UI_CONTROL_TYPE_SLIDER)
  {
    if (O->Slider.NumOfElementsDBL >= O->Slider.MaxNumOfElementsDBL)
    {
      DBL **New;

      if ((New = malloc(sizeof(DBL *) * (O->Slider.MaxNumOfElementsDBL + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
        return;
      O->Slider.MaxNumOfElementsDBL += TTP_UI_ALLOCATION_ELEMENTS;
      if (O->Slider.ValuesDBL != NULL)
      {
        memcpy(New, O->Slider.ValuesDBL, sizeof(DBL *) * O->Slider.NumOfElementsDBL);
        free(O->Slider.ValuesDBL);
      }
      O->Slider.ValuesDBL = New;
    }
    O->Slider.ValuesDBL[O->Slider.NumOfElementsDBL++] = P;
  }
} /* End of 'TTP_UIAddDblValue' function */

/* UI add float value function.
 * ARGUMENTS:
 *   - object where to add:
 *       ttpUI_CONTROL *O;
 *   - FLT value to add:
 *       FLT *P;
 * RETURNS: None.
 */
static VOID TTP_UIAddFltValue( ttpUI_CONTROL *O, FLT *P )
{
  if (O->Type == TTP_UI_CONTROL_TYPE_SLIDER)
  {
    if (O->Slider.NumOfElementsFLT >= O->Slider.MaxNumOfElementsFLT)
    {
      FLT **New;

      if ((New = malloc(sizeof(FLT *) * (O->Slider.MaxNumOfElementsFLT + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
        return;
      O->Slider.MaxNumOfElementsFLT += TTP_UI_ALLOCATION_ELEMENTS;
      if (O->Slider.ValuesFLT != NULL)
      {
        memcpy(New, O->Slider.ValuesFLT, sizeof(FLT *) * O->Slider.NumOfElementsFLT);
        free(O->Slider.ValuesFLT);
      }
      O->Slider.ValuesFLT = New;
    }
    O->Slider.ValuesFLT[O->Slider.NumOfElementsFLT++] = P;
  }
} /* End of 'TTP_UIAddFltValue' function */

/* UI add image to control object function function.
 * ARGUMENTS:
 *   - object where to add:
 *       ttpUI_CONTROL *B;
 *   - pointer to image to add:
 *       ttpIMAGE *Img;
 * RETURNS:
 *   (INT) Number of image in array (-1 if wasn't added).
 */
static INT TTP_UIAddImg( ttpUI_CONTROL *B, ttpIMAGE *Img )
{
  if (B->NumOfImg >= B->MaxNumOfImg)
  {
    ttpIMAGE **New;

    if ((New = malloc(sizeof(ttpIMAGE *) * (B->MaxNumOfImg + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
      return -1;
    B->MaxNumOfImg += TTP_UI_ALLOCATION_ELEMENTS;
    if (B->Images != NULL)
    {
      memcpy(New, B->Images, sizeof(ttpIMAGE *) * B->NumOfImg);
      free(B->Images);
    }
    B->Images = New;
  }
  B->Images[B->NumOfImg] = Img;
  return B->NumOfImg++;
} /* End of 'TTP_UIAddImg' function */

/* UI add control object to bottom of list function.
 * ARGUMENTS:
 *   - object where to add:
 *       ttpUI_CONTROL *List;
 *   - object to add:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIAddToListBottom( ttpUI_CONTROL *List, ttpUI_CONTROL *B )
{
  if (List->Type == TTP_UI_CONTROL_TYPE_LIST)
  {
    if (List->List.NumOfElements >= List->List.MaxNumOfElements)
    {
      ttpUI_CONTROL **New;

      if ((New = malloc(sizeof(ttpUI_CONTROL *) * (List->List.MaxNumOfElements + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
        return;
      List->List.MaxNumOfElements += TTP_UI_ALLOCATION_ELEMENTS;
      if (List->List.Values != NULL)
      {
        memcpy(New, List->List.Values, sizeof(ttpUI_CONTROL *) * List->List.NumOfElements);
        free(List->List.Values);
      }
      List->List.Values = New;
    }

    B->IsInList = TRUE;
    B->IsInTab = List->IsInTab;
    if (List->List.NumOfElements == 0)
    {
      B->Left = 0;
      B->Top = 0;
    }
    else
    {
      B->Left = List->List.Values[List->List.NumOfElements - 1]->Left;
      B->Top = List->List.Values[List->List.NumOfElements - 1]->Top + List->List.Values[List->List.NumOfElements - 1]->H;
    }
    List->List.Values[List->List.NumOfElements++] = B;
  }
} /* End of 'TTP_UIAddToListBottom' function */

/* UI add tab object to tab function.
 * ARGUMENTS:
 *   - object where to add:
 *       ttpUI_CONTROL *List;
 *   - tab object to add:
 *       ttpUI_CONTROL_TAB_OBJECT *Tab;
 * RETURNS: None.
 */
static VOID TTP_UIAddTabObjToTab( ttpUI_CONTROL_TAB_OBJECT *Tab )
{
  if (TTP_UIMainTab.NumOfTabs >= TTP_UIMainTab.MaxNumOfTabs)
  {
    ttpUI_CONTROL_TAB_OBJECT **New;

    if ((New = malloc(sizeof(ttpUI_CONTROL_TAB_OBJECT *) * (TTP_UIMainTab.MaxNumOfTabs + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
      return;
    TTP_UIMainTab.MaxNumOfTabs += TTP_UI_ALLOCATION_ELEMENTS;
    if (TTP_UIMainTab.Tabs != NULL)
    {
      memcpy(New, TTP_UIMainTab.Tabs, sizeof(ttpUI_CONTROL_TAB_OBJECT  *) * TTP_UIMainTab.NumOfTabs);
      free(TTP_UIMainTab.Tabs);
    }
    TTP_UIMainTab.Tabs = New;
  }

  TTP_UIMainTab.Tabs[TTP_UIMainTab.NumOfTabs++] = Tab;
} /* End of 'TTP_UIAddTabObjToTab' function */

/* UI add button to tab object function.
 * ARGUMENTS:
 *   - tab object where to add:
 *       ttpUI_CONTROL_TAB_OBJECT *Tab;
 *   - object to add:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIAddButtonToTab( ttpUI_CONTROL_TAB_OBJECT *Tab, ttpUI_CONTROL *B )
{
  if (Tab->NumOfControls >= Tab->MaxNumOfControls)
  {
    ttpUI_CONTROL **New;

    if ((New = malloc(sizeof(ttpUI_CONTROL *) * (Tab->MaxNumOfControls + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
      return;
    Tab->MaxNumOfControls += TTP_UI_ALLOCATION_ELEMENTS;
    if (Tab->Controls != NULL)
    {
      memcpy(New, Tab->Controls, sizeof(ttpUI_CONTROL *) * Tab->NumOfControls);
      free(Tab->Controls);
    }
    Tab->Controls = New;
  }

  B->IsInTab = TRUE;
  B->IndexInTab = Tab->NumOfControls;
  Tab->Controls[Tab->NumOfControls++] = B;
  B->Left = 30;
  if (Tab->NumOfControls == 1)
    B->Top = 10;
  else
    B->Top = Tab->Controls[Tab->NumOfControls - 2]->Top + Tab->Controls[Tab->NumOfControls - 2]->H + 30;
} /* End of 'TTP_UIAddButtonToTab' function */

/* UI add button to active tab object function.
 * ARGUMENTS:
 *   - object to add:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIAddButtonToActiveTab( ttpUI_CONTROL *B )
{
  ttpUI_CONTROL_TAB_OBJECT *Tab;

  if (TTP_UIMainTab.CurrentTab < 0 || TTP_UIMainTab.CurrentTab >= TTP_UIMainTab.NumOfTabs)
    return;
  Tab = TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab];
  if (Tab->NumOfControls >= Tab->MaxNumOfControls)
  {
    ttpUI_CONTROL **New;

    if ((New = malloc(sizeof(ttpUI_CONTROL *) * (Tab->MaxNumOfControls + TTP_UI_ALLOCATION_ELEMENTS))) == NULL)
      return;
    Tab->MaxNumOfControls += TTP_UI_ALLOCATION_ELEMENTS;
    if (Tab->Controls != NULL)
    {
      memcpy(New, Tab->Controls, sizeof(ttpUI_CONTROL *) * Tab->NumOfControls);
      free(Tab->Controls);
    }
    Tab->Controls = New;
  }

  B->IsInTab = TRUE;
  B->IndexInTab = Tab->NumOfControls;
  Tab->Controls[Tab->NumOfControls++] = B;
  B->Left = 30;
  if (Tab->NumOfControls == 1)
    B->Top = 10;
  else
    B->Top = Tab->Controls[Tab->NumOfControls - 2]->Top + Tab->Controls[Tab->NumOfControls - 2]->H + 30;
} /* End of 'TTP_UIAddButtonToActiveTab' function */

/* UI set active tab by name.
 * ARGUMENTS:
 *   - name of tab:
 *       CHAR *Name;
 * RETURNS: None.
 */
static VOID TTP_UISetActiveTab( CHAR *Name )
{
  INT i;

  for (i = 0; i < TTP_UIMainTab.NumOfTabs; i++)
    if (strncmp(TTP_UIMainTab.Tabs[i]->Name, Name, TTP_UI_TAB_MAX_NAME - 1) == 0)
    {
      TTP_UIMainTab.CurrentTab = i;
      return;
    }
} /* End of 'TTP_UISetActiveTab' function */

/* UIMapper initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIMapperInit( VOID )
{
  Ttp->UIAddObj = TTP_UIAddObj;
  Ttp->UIAddBoolValue = TTP_UIAddBoolValue;
  Ttp->UIAddDblValue = TTP_UIAddDblValue;
  Ttp->UIAddFltValue = TTP_UIAddFltValue;
  Ttp->UIAddButtonToActiveTab = TTP_UIAddButtonToActiveTab;
  Ttp->UISetActiveTab = TTP_UISetActiveTab;
  Ttp->UIAddImg = TTP_UIAddImg;
  Ttp->UIAddToListBottom = TTP_UIAddToListBottom;
  Ttp->UIAddTabObjToTab = TTP_UIAddTabObjToTab;
  Ttp->UIAddButtonToTab = TTP_UIAddButtonToTab;
  Ttp->UICreateTabObj = TTP_UICreateTabObj;
  /* Create objects */
  Ttp->UICreateButton = TTP_UICreateButton;
  Ttp->UICreateSlider = TTP_UICreateSlider;
  Ttp->UICreateCheckBox = TTP_UICreateCheckBox;
  Ttp->UICreateList = TTP_UICreateList;
  Ttp->UICreateTabObj = TTP_UICreateTabObj;
  /* Create objects by default */
  Ttp->UICreateDefButton = TTP_UICreateDefButton;
  Ttp->UICreateDefSlider = TTP_UICreateDefSlider;
  Ttp->UICreateDefCheckBox = TTP_UICreateDefCheckBox;

  /* Round buttons */
  Ttp->UIResponseRoundButton = TTP_UIResponseRoundButton;
  Ttp->UIRenderRoundButton = TTP_UIRenderRoundButton;
} /* End of 'TTP_UIMapperInit' function */

/* UIMapper deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIMapperClose( VOID )
{
  Ttp->UIAddObj = NULL;
  Ttp->UIAddBoolValue = NULL;
  Ttp->UIAddDblValue = NULL;
  Ttp->UIAddFltValue = NULL;
  Ttp->UIAddToListBottom = NULL;
  Ttp->UIAddTabObjToTab = NULL;
  Ttp->UIAddButtonToTab = NULL;
  Ttp->UIAddImg = NULL;
  Ttp->UICreateButton = NULL;
  Ttp->UICreateSlider = NULL;
  Ttp->UICreateCheckBox = NULL;
  Ttp->UICreateDefButton = NULL;
  Ttp->UICreateDefSlider = NULL;
  Ttp->UICreateDefCheckBox = NULL;
  Ttp->UICreateList = NULL;
  Ttp->UICreateTabObj = NULL;
  Ttp->UIAddButtonToActiveTab = NULL;
  Ttp->UISetActiveTab = NULL;
  Ttp->UIResponseRoundButton = NULL;
  Ttp->UIRenderRoundButton = NULL;
} /* End of 'TTP_UIMapperClose' function */

/* END OF 'ttp_ui_mapper.c' FILE */
