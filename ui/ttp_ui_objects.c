/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_ui_objects.c
 * PURPOSE     : Tough Third Planet project.
 *               User interface controls file.
 * PROGRAMMER  : CGSG'2022.
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

/* Control objects stock */
ttpSTOCK TTP_UIControlObjects;
/* Tab objects stock */
ttpSTOCK TTP_UIControlTabs;
/* Mouse state on previos frame */
BOOL TTP_UIControlMousePrev;
/* UI main single tab */
ttpUI_CONTROL_TAB TTP_UIMainTab;
/* Is responing/rendering in tab pass */
BOOL TTP_UITabPass;

/* Self-pointer to UI render primitive */
static ttpPRIM *TTP_UIRenderPrim;
/* TRUE if state of any button was changed */
static BOOL TTP_UIIsChanged;

/* UI DRAW2D */
ttpDRAW2D *TTP_UIRenderDraw2D;

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

/* UI control update object function.
 * ARGUMENTS: 
 *   - object to update:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIUpdateObject( ttpUI_CONTROL *B )
{
  INT i, j;
  ttpUI_CONTROL prev;

  if (!TTP_UIIsChanged)
    memcpy(&prev, B, sizeof(ttpUI_CONTROL));

  switch (B->Type)
  {
  case TTP_UI_CONTROL_TYPE_BUTTON:
    for (i = 0; i < B->Button.NumOfElements; i++) 
      if (*(B->Button.Values[i]) != B->Button.IsPressed)
      {
        B->Button.IsPressed = *(B->Button.Values[i]);
        for (j = 0; j < B->Button.NumOfElements; j++)
          *(B->Button.Values[j]) = B->Button.IsPressed;
        break;
      }
    break;
  case TTP_UI_CONTROL_TYPE_SLIDER:
    for (i = 0; i < B->Slider.NumOfElementsDBL; i++)
      if (*(B->Slider.ValuesDBL[i]) != B->Slider.Pos)
      {
        B->Slider.Pos = *(B->Slider.ValuesDBL[i]);
        for (j = 0; j < B->Slider.NumOfElementsDBL; j++)
          *(B->Slider.ValuesDBL[j]) = B->Slider.Pos;
        for (j = 0; j < B->Slider.NumOfElementsFLT; j++)
          *(B->Slider.ValuesFLT[j]) = (FLT)B->Slider.Pos;
        break;
      }

    for (i = 0; i < B->Slider.NumOfElementsFLT; i++)
      if (*(B->Slider.ValuesFLT[i]) != (FLT)B->Slider.Pos)
      {
        B->Slider.Pos = (DBL)(*(B->Slider.ValuesFLT[i]));
        for (j = 0; j < B->Slider.NumOfElementsDBL; j++)
          *(B->Slider.ValuesDBL[j]) = B->Slider.Pos;
        for (j = 0; j < B->Slider.NumOfElementsFLT; j++)
          *(B->Slider.ValuesFLT[j]) = (FLT)B->Slider.Pos;
        break;
      }
    break;
  case TTP_UI_CONTROL_TYPE_CHECKBOX:
    for (i = 0; i < B->Checkbox.NumOfElements; i++)
      if (*(B->Checkbox.Values[i]) != B->Checkbox.IsOn)
      {
        B->Checkbox.IsOn = *(B->Checkbox.Values[i]);
        for (j = 0; j < B->Checkbox.NumOfElements; j++)
          *(B->Checkbox.Values[j]) = B->Checkbox.IsOn;
        break;
      }
    break;
  }

  if (!TTP_UIIsChanged)
    if (memcmp(&prev, B, sizeof(ttpUI_CONTROL)) != 0)
      TTP_UIIsChanged = TRUE;
} /* End of 'TTP_UIUpdateObject' function */

/* UI control update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIUpdate( VOID )
{
  TTP_StockWalk(&TTP_UIControlObjects, TTP_UIUpdateObject);
} /* End of 'TTP_UIUpdate' function */

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

/* UI control response object function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
VOID TTP_UIControlResponseObject( ttpUI_CONTROL *B )
{
  ttpUI_CONTROL prev;

  if (B->IsInList || (B->IsInTab && !TTP_UITabPass))
    return;

  if (!TTP_UIIsChanged)
    memcpy(&prev, B, sizeof(ttpUI_CONTROL));

  if (B->ResponseObject != NULL)
    B->ResponseObject(B);
  else
    TTP_UIControlResponseButton(B);

  if (!TTP_UIIsChanged)
    if (memcmp(&prev, B, sizeof(ttpUI_CONTROL)) != 0)
      TTP_UIIsChanged = TRUE;
} /* End of 'TTP_UIControlResponseObject' function */

/* UI control response tab function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIControlResponseTab( VOID )
{
  static BOOL IsActiveLeft = FALSE, IsActiveTop = FALSE, IsActiveBottom = FALSE, IsActiveRight = FALSE;
  static DBL Speed = 0.5;
  INT left = TTP_UIMainTab.Left, top = TTP_UIMainTab.Top, i;

  if (Ttp->KeysClick[VK_LBUTTON] && POINT_IN_RECT(Ttp->Mx, Ttp->My, Ttp->W - 10, 0, 10, 10))
  {
    Ttp->IsTabHidden = !Ttp->IsTabHidden;
    TTP_UIIsChanged = TRUE;
  }
  if (Ttp->IsTabHidden)
    return;

  if (left < 0)
    left = 0;
  else if (left + TTP_UIMainTab.W >= Ttp->W)
    left = Ttp->W - TTP_UIMainTab.W;
  if (top < 0)
    top = 0;
  else if (top + TTP_UIMainTab.H >= Ttp->H)
    top = Ttp->H - TTP_UIMainTab.H;

  /* Scaling & moving tab */
  if (!TTP_UIControlMousePrev && Ttp->Keys[VK_LBUTTON] &&
      Ttp->My - top - TTP_UIMainTab.H <= 5 && Ttp->My - top - TTP_UIMainTab.H >= -5 &&
      Ttp->Mx - TTP_UIMainTab.Left >= -5  && Ttp->Mx - TTP_UIMainTab.Left - TTP_UIMainTab.W <= 5)
    IsActiveBottom = TRUE;
  else if (!TTP_UIControlMousePrev && Ttp->Keys[VK_LBUTTON] &&
           Ttp->My - top <= 5 && Ttp->My - top >= -5 &&
           Ttp->Mx - TTP_UIMainTab.Left >= -5  && Ttp->Mx - TTP_UIMainTab.Left - TTP_UIMainTab.W <= 5)
    IsActiveTop = TRUE;
  if (!TTP_UIControlMousePrev && Ttp->Keys[VK_LBUTTON] &&
      Ttp->Mx - left - TTP_UIMainTab.W <= 5 && Ttp->Mx - left - TTP_UIMainTab.W >= -5 &&
      Ttp->My - TTP_UIMainTab.Top >= -5  && Ttp->My - TTP_UIMainTab.Top - TTP_UIMainTab.H <= 5)
    IsActiveRight = TRUE;
  else if (!TTP_UIControlMousePrev && Ttp->Keys[VK_LBUTTON] &&
           Ttp->Mx - left <= 5 && Ttp->Mx - left >= -5 &&
           Ttp->My - TTP_UIMainTab.Top >= -5  && Ttp->My - TTP_UIMainTab.Top - TTP_UIMainTab.H <= 5)
    IsActiveLeft = TRUE;
  if (!Ttp->Keys[VK_LBUTTON])
    IsActiveLeft = IsActiveRight = IsActiveTop = IsActiveBottom = FALSE;

  if (IsActiveRight && IsActiveBottom)
    SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
  if (IsActiveTop || IsActiveLeft)
    SetCursor(LoadCursor(NULL, IDC_SIZEALL));

  if (IsActiveLeft)
  {
    TTP_UIMainTab.Left = Ttp->Mx;
    TTP_UIIsChanged = TRUE;
  }
  else if (IsActiveRight && Ttp->Mx - TTP_UIMainTab.Left >= TTP_UIMainTab.ButtonW)
  {
    if (!IsActiveBottom && !IsActiveTop)
      SetCursor(LoadCursor(NULL, IDC_SIZEWE));
    TTP_UIMainTab.W = Ttp->Mx - TTP_UIMainTab.Left;
    TTP_UIIsChanged = TRUE;
  }
  if (IsActiveTop)
  {
    TTP_UIMainTab.Top = Ttp->My;
    TTP_UIIsChanged = TRUE;
  }
  else if (IsActiveBottom && Ttp->My - TTP_UIMainTab.Top >= TTP_UIMainTab.ButtonH)
  {
    if (!IsActiveRight && !IsActiveLeft)
      SetCursor(LoadCursor(NULL, IDC_SIZENS));
    TTP_UIMainTab.H = Ttp->My - TTP_UIMainTab.Top;
    TTP_UIIsChanged = TRUE;
  }

  /* Choosing tab */
  if (Ttp->KeysClick[VK_LBUTTON] && Ttp->Mx <= left && Ttp->My >= top)
  {
    INT
      k = TTP_UIMainTab.ButtonH / 10,
      n =
      (Ttp->My - top) / (TTP_UIMainTab.ButtonH + k) +
      TTP_UIMainTab.H / TTP_UIMainTab.ButtonH * ((left - Ttp->Mx) / (TTP_UIMainTab.ButtonW + k));

    if (n < TTP_UIMainTab.NumOfTabs)
    {
      TTP_UIMainTab.CurrentTab = n;
      TTP_UIMainTab.Pos = 0;
      TTP_UIIsChanged = TRUE;
    }
  }

  if (TTP_UIMainTab.CurrentTab < 0 && TTP_UIMainTab.CurrentTab >= TTP_UIMainTab.NumOfTabs)
    return;

  if (Ttp->Mdz != 0 &&
      POINT_IN_RECT(Ttp->Mx, Ttp->My,
                    left, top, TTP_UIMainTab.W, TTP_UIMainTab.H))
  {
    INT lowest;
    ttpUI_CONTROL *c = TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->Controls[TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->NumOfControls - 1];

    if (c->Type != TTP_UI_CONTROL_TYPE_LIST || !c->List.IsOn || c->List.NumOfElements == 0)
      lowest = c->Top + c->H - TTP_UIMainTab.H;
    else
      lowest = c->Top + c->List.Values[c->List.NumOfElements - 1]->H;

    if (TTP_UIMainTab.Pos > 0 && Ttp->Mdz > 0)
    {
      TTP_UIMainTab.Pos -= Ttp->Mdz * Speed;
      if (TTP_UIMainTab.Pos < 0)
        TTP_UIMainTab.Pos = 0;
      TTP_UIIsChanged = TRUE;
    }
    else if (TTP_UIMainTab.Pos < lowest && Ttp->Mdz < 0)
    {
      TTP_UIMainTab.Pos -= Ttp->Mdz * Speed;
      if (TTP_UIMainTab.Pos > lowest)
        TTP_UIMainTab.Pos = lowest;
      TTP_UIIsChanged = TRUE;
    }
  }
  TTP_UITabPass = TRUE;

  for (i = 0; i < TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->NumOfControls; i++)
  {
    ttpUI_CONTROL *c = TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->Controls[i];

    c->Top -= TTP_UIMainTab.Pos;
    c->Top += top;
    c->Left += left;
    if ((c->Type == TTP_UI_CONTROL_TYPE_LIST && c->List.IsOn && c->List.NumOfElements != 0) || (c->Top >= top && c->Top + c->H <= TTP_UIMainTab.H + top))
      TTP_UIControlResponseObject(c);
    c->Top += TTP_UIMainTab.Pos;
    c->Top -= top;
    c->Left -= left;
  }

  TTP_UITabPass = FALSE;
} /* End of 'TTP_UIControlResponseTab' function */

/* UI control response all buttons in array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIControlResponseAll( VOID )
{
  TTP_StockWalk(&TTP_UIControlObjects, TTP_UIControlResponseObject);
  if (TTP_UIMainTab.NumOfTabs != 0)
    TTP_UIControlResponseTab();
} /* End of 'TTP_UIControlResponseAll' function */

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

/* UI control render object function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
VOID TTP_UIControlRenderObject( ttpUI_CONTROL *B )
{
  if (B->IsInList || (B->IsInTab && !TTP_UITabPass))
    return;
  
  if (B->RenderObject != NULL)
    B->RenderObject(B);
  else
    TTP_UIControlRenderButton(B);
} /* End of 'TTP_UIControlRenderObject' function */

/* UI control render tab function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIControlRenderTab( VOID )
{
  INT left = TTP_UIMainTab.Left, top = TTP_UIMainTab.Top, i, n, k = TTP_UIMainTab.ButtonH / 10;

  if (Ttp->IsTabHidden)
  {
    Ttp->PutRect(Ttp->W - 10, 0, Ttp->W, 10, Vec4Set(127, 127, 127, 255), Vec4Set1(255));
    return;
  }
  if (left < 0)
    left = 0;
  else if (left + TTP_UIMainTab.W >= Ttp->W)
    left = Ttp->W - TTP_UIMainTab.W;
  if (top < 0)
    top = 0;
  else if (top + TTP_UIMainTab.H >= Ttp->H)
    top = Ttp->H - TTP_UIMainTab.H;

  n = TTP_UIMainTab.H / TTP_UIMainTab.ButtonH;
  Ttp->PutRect(left, top, left + TTP_UIMainTab.W, top + TTP_UIMainTab.H, Vec4Set(63, 63, 63, 200), Vec4Set(63, 63, 63, 200));

  for (i = 0; i < TTP_UIMainTab.NumOfTabs; i++)
  {
    if (i == TTP_UIMainTab.CurrentTab)
      Ttp->PutRect(left - TTP_UIMainTab.ButtonW - k - 1 - (TTP_UIMainTab.ButtonW + k) * (i / n), top + (TTP_UIMainTab.ButtonH + k) * (i % n), left + 1 - k - (TTP_UIMainTab.ButtonW + k) * (i / n), top + TTP_UIMainTab.ButtonH + 2 + (TTP_UIMainTab.ButtonH + k) * (i % n), Vec4SetVec3(Vec3Set1(255), 127), Vec4SetVec3(Vec3Set1(255), 127));
    else
      Ttp->PutRect(left - TTP_UIMainTab.ButtonW - k - 1 - (TTP_UIMainTab.ButtonW + k) * (i / n), top + (TTP_UIMainTab.ButtonH + k) * (i % n), left + 1 - k - (TTP_UIMainTab.ButtonW + k) * (i / n), top + TTP_UIMainTab.ButtonH + 2 + (TTP_UIMainTab.ButtonH + k) * (i % n), Vec4Set1(127), Vec4Set1(127));
    if (TTP_UIMainTab.Tabs[i]->Img != NULL)
      Ttp->PutImg(TTP_UIMainTab.Tabs[i]->Img, left - TTP_UIMainTab.ButtonW - k - (TTP_UIMainTab.ButtonW + k) * (i / n), top + 1 + (TTP_UIMainTab.ButtonH + k) * (i % n), TTP_UIMainTab.ButtonW, TTP_UIMainTab.ButtonH);
  }

  if (TTP_UIMainTab.CurrentTab < 0 || TTP_UIMainTab.CurrentTab >= TTP_UIMainTab.NumOfTabs)
    return;
  TTP_UITabPass = TRUE;

  Ttp->PutLimitsSet(left, top, left + TTP_UIMainTab.W, top + TTP_UIMainTab.H);
  TTP_UIRenderDraw2D->IsLimitsActive = TRUE;

  for (i = 0; i < TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->NumOfControls; i++)
  {
    ttpUI_CONTROL *c = TTP_UIMainTab.Tabs[TTP_UIMainTab.CurrentTab]->Controls[i];

    c->Top -= TTP_UIMainTab.Pos;
    c->Top += top;
    c->Left += left;
    if (c->Top >= top && c->Top + c->H <= TTP_UIMainTab.H + top && c->Left >= left && c->Left + c->W <= left + TTP_UIMainTab.W)
      TTP_UIControlRenderObject(c);
    else if ((c->Type == TTP_UI_CONTROL_TYPE_LIST && c->List.IsOn && c->List.NumOfElements != 0) || (c->Top < top && c->Top + c->H > top) || (c->Top < top + TTP_UIMainTab.H && c->Top + c->H > top + TTP_UIMainTab.H) || (c->Left < left && c->Left + c->H > left) || (c->Left < left + TTP_UIMainTab.W && c->Left + c->W > left + TTP_UIMainTab.W))
    {
      TTP_UIControlRenderObject(c);
      if (c->Top < top)
        Ttp->PutRect(c->Left, c->Top, c->Left + c->W, top, Vec4Set1(0), Vec4Set1(0));
      if (c->Top + c->H > top + TTP_UIMainTab.H)
        Ttp->PutRect(c->Left, top + TTP_UIMainTab.H, c->Left + c->W, c->Top + c->H, Vec4Set1(0), Vec4Set1(0));
      if (c->Left < left)
        Ttp->PutRect(c->Left, c->Top, left, c->Top + c->H, Vec4Set1(0), Vec4Set1(0));
      if (c->Left + c->W > left + TTP_UIMainTab.W)
        Ttp->PutRect(left + TTP_UIMainTab.W, c->Top, c->Left + c->W, c->Top + c->H, Vec4Set1(0), Vec4Set1(0));
    }
    c->Left -= left;
    c->Top -= top;
    c->Top += TTP_UIMainTab.Pos;
  }

  TTP_UIRenderDraw2D->IsLimitsActive = FALSE;
  Ttp->PutRect(Ttp->W - 10, 0, Ttp->W, 10, Vec4Set(127, 127, 127, 255), Vec4Set1(255));

  TTP_UITabPass = FALSE;
} /* End of 'TTP_UIControlRenderTab' function */

/* UI control render all buttons in array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIControlRenderAll( VOID )
{
  if (TTP_UIIsChanged || Ttp->W != TTP_UIRenderDraw2D->Img.W || Ttp->H != TTP_UIRenderDraw2D->Img.H)
  {
    if (TTP_UIRenderDraw2D->Img.W != Ttp->W || TTP_UIRenderDraw2D->Img.H != Ttp->H)
    {
      Ttp->ImgFree(&TTP_UIRenderDraw2D->Img);
      Ttp->ImgCreate(&TTP_UIRenderDraw2D->Img, Ttp->W, Ttp->H, 4, "UI image sample");
      Ttp->TexResize(TTP_UIRenderDraw2D->Tex, Ttp->W, Ttp->H);
      Ttp->Draw2DReverse(TTP_UIRenderDraw2D);
    }
    Ttp->DrawStart(TTP_UIRenderDraw2D);
    memset(TTP_UIRenderDraw2D->Img.Bits, 0, 4 * TTP_UIRenderDraw2D->Img.W * TTP_UIRenderDraw2D->Img.H);
    if (TTP_UIMainTab.NumOfTabs != 0)
      TTP_UIControlRenderTab();
    TTP_StockWalk(&TTP_UIControlObjects, TTP_UIControlRenderObject);

    /* User's drawings (WIP) */
    /*
    Ttp->PutSmoothRect(100, 150, 400, 250, 20, Vec4Set1(255), Vec4Set(122, 122, 122, 200));
    Ttp->PutPill(200, 400, 300, 450, Vec4Set1(255), Vec4Set(122, 122, 122, 200));
    Ttp->PutTextSetFont("Arial", TTP_FONT_BOLD | TTP_FONT_STRIKE_ON);
    Ttp->PutTextSetSize(15);
    Ttp->PutText("Super Ultra Mega CGSG test Русские буквы 0123", 0, 40, Vec4Set1(255));
    */
    Ttp->DrawEnd();

    /* TTP_UIRenderPrim->Mtl->Tex[0] = TTP_UIRenderDraw2D->Tex; */
  }
  /* Ttp->PrimDraw(TTP_UIRenderPrim, MatrIdentity()); */
  TTP_UIControlMousePrev = Ttp->Keys[VK_LBUTTON];
  TTP_UIIsChanged = FALSE;
} /* End of 'TTP_UIControlRenderAll' function */

/* UI control initializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIControlInit( VOID )
{
  /*
  ttpMATERIAL_PATTERN *mtlpt = Ttp->MtlPatCreate("UI Material Pattern", "", "", "user_interface", 1);
  ttpMATERIAL *mtl = Ttp->MtlCreate("UI Material", mtlpt, "", "UI Texture", FALSE);
  */
  TTP_UIRenderDraw2D = Ttp->Draw2DCreate(Ttp->W, Ttp->H);

  Ttp->UITexture = TTP_UIRenderDraw2D->Tex;
  Ttp->IsTabHidden = FALSE;
  TTP_UITabPass = FALSE;
  TTP_UIMainTab.Left = 1400;
  TTP_UIMainTab.Top = 10;
  TTP_UIMainTab.W = 500;
  TTP_UIMainTab.H = 540;
  TTP_UIMainTab.ButtonW = TTP_UIMainTab.ButtonH = 50;
  TTP_UIMainTab.NumOfTabs = 0;
  TTP_UIMainTab.CurrentTab = 0;
  TTP_UIMainTab.Pos = 0;
  TTP_UIMainTab.NumOfTabs = TTP_UIMainTab.MaxNumOfTabs = 0;
  TTP_UIMainTab.Tabs = NULL;
  /*
  mtl->Tex[0] = TTP_UIRenderDraw2D->Tex;

  TTP_UIRenderPrim = Ttp->PrimCreate(mtl, TTP_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  */
  TTP_StockCreate(&TTP_UIControlObjects, "UI Controls Stock", sizeof(ttpUI_CONTROL), 10, NULL);

  Ttp->UIControlResponseAll = TTP_UIControlResponseAll;
  Ttp->UIUpdate = TTP_UIUpdate;
  Ttp->UIControlRenderAll = TTP_UIControlRenderAll;
  Ttp->UIControlRenderButton = TTP_UIControlRenderButton;
  TTP_StockAdd(&Ttp->FreezeStocks, TTP_PTR2REF(&TTP_UIControlObjects));
  Ttp->Draw2DReverse(TTP_UIRenderDraw2D);
} /* End of 'TTP_UIControlInit' function */

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

/* UI control close object function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIControlCloseObject( ttpUI_CONTROL *B )
{
  if (B->CloseObject != NULL)
    B->CloseObject(B);
} /* End of 'TTP_UIControlCloseObject' function */

/* UI control deinitializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIControlClose( VOID )
{
  Ttp->Draw2DFree(TTP_UIRenderDraw2D);
  
  TTP_StockWalk(&TTP_UIControlObjects, TTP_UIControlCloseObject);

  TTP_StockFree(&TTP_UIControlObjects);
  Ttp->UIControlResponseAll = NULL;
  Ttp->UIControlRenderAll = NULL;
  Ttp->UIControlRenderButton = NULL;
  if (TTP_UIMainTab.Tabs != NULL)
  {
    INT i;

    for (i = 0; i < TTP_UIMainTab.NumOfTabs; i++)
    {
      if (TTP_UIMainTab.Tabs[i]->Controls != NULL)
        free(TTP_UIMainTab.Tabs[i]->Controls);
      TTP_UIMainTab.Tabs[i]->Controls = NULL;
      TTP_UIMainTab.Tabs[i]->NumOfControls = TTP_UIMainTab.Tabs[i]->MaxNumOfControls = 0;
      free(TTP_UIMainTab.Tabs[i]);
    }
    free(TTP_UIMainTab.Tabs);
  }
  TTP_UIMainTab.Tabs = NULL;
  TTP_UIMainTab.NumOfTabs = TTP_UIMainTab.MaxNumOfTabs = 0;
} /* End of 'TTP_UIControlClose' function */

/***
 * CGSG logo deafult output.
 ***/

/* Render logo function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIRenderLogo( ttpUI_CONTROL *B )
{
  if (B->Type != TTP_UI_CONTROL_TYPE_BUTTON || B->NumOfImg == 0)
  {
    Ttp->UIControlRenderButton(B);
    return;
  }
  /* Draw image */
  Ttp->UI.DrawToTex.PutImg(B->Images[0], 0, Ttp->H - B->Images[0]->H / 2, B->Images[0]->W / 2, B->Images[0]->H / 2);
} /* End of 'TTP_UIRenderLogo' function */

/* Response log function.
 * ARGUMENTS:
 *   - self-pointer to button:
 *       ttpUI_CONTROL *B;
 * RETURNS: None.
 */
static VOID TTP_UIResponseLogo( ttpUI_CONTROL *B )
{
  /* Blank funciton */
} /* End of 'TTP_UIResponseLogo' function */

static ttpIMAGE TTP_UILogo;

/* UI create default control objects function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIDefInit( VOID )
{
  ttpUI_CONTROL *B;

  B = Ttp->UICreateButton(0, 0, 0, 0);
  B->ResponseObject = TTP_UIResponseLogo;
  B->RenderObject = TTP_UIRenderLogo;
  Ttp->ImgLoad(&TTP_UILogo, "bin/textures/images/CGSG-Logo.png");
  Ttp->UIAddImg(B, &TTP_UILogo);
} /* End of 'TTP_UIDefInit' function */

/* UI close default control objects function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_UIDefClose( VOID )
{
  Ttp->ImgFree(&TTP_UILogo);
} /* End of 'TTP_UIDefClose' function */

/* UI initializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIInit( VOID )
{
  TTP_UIIsChanged = TRUE;
  TTP_UIControlMousePrev = FALSE;
  TTP_UIDrawInit();
  TTP_UIControlInit();
  TTP_UIMapperInit();
  TTP_UIDefInit();
  TTP_UITexViewInit();
} /* End of 'TTP_UIInit' function */

/* UI deinitializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIClose( VOID )
{
  TTP_UIDefClose();
  TTP_UIMapperClose();
  TTP_UIControlClose();
  TTP_UIDrawClose();
} /* End of 'TTP_UIClose' function */

/* END OF 'ttp_ui_objects.c' FILE */
