/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_ui.h
 * PURPOSE     : Tough Third Planet project.
 *               User interface declaration module.
 * PROGRAMMER  : CGSG'2022.
 *               Nikita Koptelov (NK1),
 *               Alice Perfileva (AP4),
 *               Daniil Shapovalov (DS5).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_ui_h_
#define __ttp_ui_h_

#include "ttp_def.h"

/***
 * Control types & functions.
 ***/

/* Forward allocation for button array */
#define TTP_UI_ALLOCATION_ELEMENTS 10
/* Maximum tab name length */
#define TTP_UI_TAB_MAX_NAME 300

/* Type of UI control element */
typedef enum tagttpUI_CONTROL_TYPE
{
  TTP_UI_CONTROL_TYPE_BUTTON,   /* Button type */
  TTP_UI_CONTROL_TYPE_SLIDER,   /* Slider type */
  TTP_UI_CONTROL_TYPE_CHECKBOX, /* Checkbox type */
  TTP_UI_CONTROL_TYPE_LIST,     /* List type */
} ttpUI_CONTROL_TYPE;

/* UI button representation type */
typedef struct tagttpUI_CONTROL_BUTTON
{
  BOOL IsPressed;                      /* Is button pressed */
  BOOL IsActive;                       /* Was button mouse pressed */
  DBL ActiveTime;                      /* Time after pressing that button is drawing like active */
  INT NumOfElements, MaxNumOfElements; /* Num of pointed values */
  BOOL **Values;                       /* Array of pointed values */
} ttpUI_CONTROL_BUTTON;

/* UI slider representation type */
typedef struct tagttpUI_CONTROL_SLIDER
{
  DBL From, To;                            /* Variation of value from to */
  BOOL IsActive;                           /* Was button mouse pressed */
  DBL Pos;                                 /* Pos on slider from 0 to 1 (0 - From, 1 - To) */
  INT
    NumOfElementsDBL, MaxNumOfElementsDBL, /* Num of DBL pointed values */
    NumOfElementsFLT, MaxNumOfElementsFLT; /* Num of FLT pointed values */
  DBL **ValuesDBL;                         /* Array of DBL pointed values */
  FLT **ValuesFLT;                         /* Array of FLT pointed values */
  DBL Step;                                /* Step of slider (0 if slider doesn't have step) */
} ttpUI_CONTROL_SLIDER;

/* UI checkbox representation type */
typedef struct tagttpUI_CONTROL_CHECKBOX
{
  BOOL IsOn;                           /* Is checkbox on */
  INT NumOfElements, MaxNumOfElements; /* Num of pointed values */
  BOOL **Values;                       /* Array of pointed values */
} ttpUI_CONTROL_CHECKBOX;

/* Forward reference */
typedef struct tagttpUI_CONTROL ttpUI_CONTROL;

/* UI list representation type */
typedef struct tagttpUI_CONTROL_LIST
{
  BOOL IsOn;                           /* Is checkbox on */
  INT NumOfElements, MaxNumOfElements; /* Num of control objects */
  ttpUI_CONTROL **Values;              /* Array of control objects */
} ttpUI_CONTROL_LIST;

/* UI control object representation type */
struct tagttpUI_CONTROL
{
  ttpUI_CONTROL_TYPE Type; /* Type of control object */
  INT Left, Top, W, H;     /* Position & sizes of object */
  /* Render UI control object function.
   * ARGUMENTS:
   *   - object to draw:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*RenderObject)( ttpUI_CONTROL *B );
  /* UI control response object function.
   * ARGUMENTS:
   *   - responsing object:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*ResponseObject)( ttpUI_CONTROL *B );
  /* Close UI control object function.
   * ARGUMENTS:
   *   - object to draw:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*CloseObject)( ttpUI_CONTROL *B );
  INT NumOfImg, MaxNumOfImg;               /* Number of images */
  ttpIMAGE **Images;                       /* Array of object iamges */
  BOOL IsInList;                           /* Is in drop-out list flag */
  BOOL IsInTab;                            /* Is in tab flag */
  INT IndexInTab;                          /* Index in controls array of tab object */
  union
  {
    ttpUI_CONTROL_BUTTON Button;
    ttpUI_CONTROL_SLIDER Slider;
    ttpUI_CONTROL_CHECKBOX Checkbox;
    ttpUI_CONTROL_LIST List;
  };
};

/* UI tab object representation type */
typedef struct tagttpUI_CONTROL_TAB_OBJECT
{
  INT NumOfControls, MaxNumOfControls; /* Num of controls */
  ttpUI_CONTROL **Controls;            /* Array of controls */
  ttpIMAGE *Img;                       /* Image of switch button in tab */
  CHAR Name[TTP_UI_TAB_MAX_NAME];      /* Name of tab */
} ttpUI_CONTROL_TAB_OBJECT;

/* UI tab representation type */
typedef struct tagttpUI_CONTROL_TAB
{
  INT Left, Top, W, H;             /* Position & sizes of tabs */
  INT ButtonW, ButtonH;            /* Sizes of switch buttons */
  INT NumOfTabs, MaxNumOfTabs;     /* Num of different tabs */
  ttpUI_CONTROL_TAB_OBJECT **Tabs; /* Array of tabs */
  INT CurrentTab;                  /* Index of current tab */
  DBL Pos;                         /* Position in current tab */
} ttpUI_CONTROL_TAB;

/* Control objects stock */
extern ttpSTOCK TTP_UIControlObjects;
/* Tab objects stock */
extern ttpSTOCK TTP_UIControlTabs;
/* Mouse state on previos frame */
extern BOOL TTP_UIControlMousePrev;
/* UI main single tab */
extern ttpUI_CONTROL_TAB TTP_UIMainTab;
/* Is responing/rendering in tab pass */
extern BOOL TTP_UITabPass;

/* UI control initializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIControlInit( VOID );

/* UI control deinitializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIControlClose( VOID );

/***
 * Draw to texture module types and prototypes.
 ***/

/* Bit flags for font params */
#define TTP_FONT_LIGHT         0x0001
#define TTP_FONT_MEDIUM        0x0002
#define TTP_FONT_BOLD          0x0004
#define TTP_FONT_ITALIC_OFF    0x0008
#define TTP_FONT_ITALIC_ON     0x0010
#define TTP_FONT_UNDERLINE_OFF 0x0020
#define TTP_FONT_UNDERLINE_ON  0x0040
#define TTP_FONT_STRIKE_OFF    0x0080
#define TTP_FONT_STRIKE_ON     0x0100

#define TTP_UI_WIN_BUTTON    DFC_BUTTON
#define TTP_UI_WIN_CAPTION   DFC_CAPTION
#define TTP_UI_WIN_MENU      DFC_MENU
#define TTP_UI_WIN_POPUPMENU DFC_POPUPMENU
#define TTP_UI_WIN_SCROLL    DFC_SCROLL

#define TTP_UI_WIN_BUTTON_3STATE      DFCS_BUTTON3STATE
#define TTP_UI_WIN_BUTTON_CHECK       DFCS_BUTTONCHECK
#define TTP_UI_WIN_BUTTON_PUSH        DFCS_BUTTONPUSH
#define TTP_UI_WIN_BUTTON_RADIO       DFCS_BUTTONRADIO
#define TTP_UI_WIN_BUTTON_RADIO_IMAGE DFCS_BUTTONRADIOIMAGE
#define TTP_UI_WIN_BUTTON_RADIO_MASK  DFCS_BUTTONRADIOMASK

#define TTP_UI_WIN_CAPTION_CLOSE   DFCS_CAPTIONCLOSE
#define TTP_UI_WIN_CAPTION_HELP    DFCS_CAPTIONHELP
#define TTP_UI_WIN_CAPTION_MAX     DFCS_CAPTIONMAX
#define TTP_UI_WIN_CAPTION_MIN     DFCS_CAPTIONMIN
#define TTP_UI_WIN_CAPTION_RESTORE DFCS_CAPTIONRESTORE

#define TTP_UI_WIN_MENU_ARROW       DFCS_MENUARROW
#define TTP_UI_WIN_MENU_ARROW_RIGHT DFCS_MENUARROWRIGHT
#define TTP_UI_WIN_MENU_BULLET      DFCS_MENUBULLET
#define TTP_UI_WIN_MENU_CHECK       DFCS_MENUCHECK

#define TTP_UI_WIN_SCROLL_COMBOBOX DFCS_SCROLLCOMBOBOX
#define TTP_UI_WIN_SCROLL_DOWN     DFCS_SCROLLDOWN
#define TTP_UI_WIN_SCROLL_LEFT     DFCS_SCROLLLEFT
#define TTP_UI_WIN_SCROLL_RIGHT    DFCS_SCROLLRIGHT
#define TTP_UI_WIN_SCROLL_SIZEGRIP DFCS_SCROLLSIZEGRIP
#define TTP_UI_WIN_SCROLL_UP       DFCS_SCROLLUP

#define TTP_UI_WIN_CHECKED     DFCS_CHECKED
#define TTP_UI_WIN_FLAT        DFCS_FLAT
#define TTP_UI_WIN_HOT         DFCS_HOT
#define TTP_UI_WIN_INACTIVE    DFCS_INACTIVE
#define TTP_UI_WIN_MONO        DFCS_MONO
#define TTP_UI_WIN_PUSHED      DFCS_PUSHED
#define TTP_UI_WIN_TRANSPARENT DFCS_TRANSPARENT

/* Draw to texture structure type */
typedef struct tagttpDRAW2D
{
  ttpIMAGE Img;                /* Image */
  ttpTEXTURE *Tex;             /* Pointer to texture */
  HFONT hFnt;                  /* Font of this structure */
  BOOL ToReverseText;          /* Flag for text reverse or not */
  INT Size;                    /* Size of font */
  CHAR FontName[TTP_MAX_NAME]; /* Name of font */
  DWORD Params;                /* Additional params of font */
  /* Borders of drawing (limits) */
  INT
    LimitX1, LimitY1,
    LimitX2, LimitY2;
  BOOL IsLimitsActive; /* Is limits active */
} ttpDRAW2D;

/* UI objects Draw2D pointer */
extern ttpDRAW2D *TTP_UIRenderDraw2D;

/* UI draw to texture pointers initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIDrawInit( VOID );

/* UI draw to texture pointers deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIDrawClose( VOID );

/* User interface controls context */
typedef struct tagttpSYSTEM_UI_CONTROLS
{
  /* UI control response all buttons in array function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*UIControlResponseAll)( VOID );

  /* UI control render all buttons in array function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*UIControlRenderAll)( VOID );

  /* UI control render button function.
   * ARGUMENTS:
   *   - self-pointer to button:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*UIControlRenderButton)( ttpUI_CONTROL *B );

  ttpTEXTURE *UITexture; /* Full-screen UI drawing texture */
  BOOL IsTabHidden;
} ttpSYSTEM_UI_CONTROLS;

/* User interface draw to texture context */
typedef struct tagttpSYSTEM_UI_DRAW
{
  /* Create struct for 2D drawing function.
   * ARGUMENTS:
   *   - width and height of image:
   *       INT W, H;
   * RETURNS:
   *   (ttpDRAW2D *) pointer to created 2D drawing structure.
   */
  ttpDRAW2D * (*Draw2DCreate)( INT W, INT H );

  /* Free struct for 2D drawing function.
   * ARGUMENTS:
   *   - pointer to structure:
   *       ttpDRAW2D *Dr;
   * RETURNS: None.
   */
  VOID (*Draw2DFree)( ttpDRAW2D *Dr );

  /* Clear image in 2D drawing structure and recreate reversed hbitmap function.
   * ARGUMENTS:
   *   - 2D drawing structure pointer:
   *       ttpDRAW2D *Dr;
   * RETURNS: None.
   */
  VOID (*Draw2DReverse)( ttpDRAW2D *Dr );

  /* Display 2D drawing to screen function.
   * ARGUMENTS:
   *   - pointer to structure:
   *       ttpDRAW2D *Dr;
   *   - screen position:
   *       INT X, Y;
   * RETURNS: None.
   */
  VOID (*Draw2DToScreen)( ttpDRAW2D *Dr, INT X, INT Y );

  /* Start draw to texture function.
   * ARGUMENTS:
   *   - 2D drawing structure to draw:
   *       ttpDRAW2D *Dr;
   * RETURNS: None.
   */
  VOID (*DrawStart)( ttpDRAW2D *Dr );

  /* Put limits in 2D drawing struct function.
   * ARGUMENTS:
   *   - left top point (left top corner is begin of coord):
   *       INT X1, Y1;
   *   - right bottom point (left top corner is begin of coord):
   *       INT X2, Y2;
   * RETURNS: None.
   */
  VOID (*PutLimitsSet)( INT X1, INT Y1, INT X2, INT Y2 );

  /* Set limits to default in 2D drawing struct function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PutLimitsDef)( VOID );

  /* Get inactive limits in 2D drawing structure function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PutLimitsOff)( VOID );

  /* Get active limits in 2D drawing structure function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PutLimitsOn)( VOID );

  /* Put pixel in texture function.
   * ARGUMENTS:
   *   - position of pixel:
   *       INT X, Y;
   *   - color to draw:
   *       VEC4 Col;
   * RETURNS: None.
   */
  VOID (*PutPixel)( INT X, INT Y, VEC4 Col );

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
  VOID (*PutLine)( INT X1, INT Y1, INT X2, INT Y2, VEC4 Col );

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
  VOID (*PutRect)( INT X1, INT Y1, INT X2, INT Y2,
                   VEC4 ColBorder, VEC4 ColFill );

  /* Put rect which clear BG in texture function.
   * ARGUMENTS:
   *   - color to fill rectangle:
   *       VEC4 Col;
   * RETURNS: None.
   */
  VOID (*PutClearBG)( VEC4 Col );

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
  VOID (*PutCircle)( INT X0, INT Y0, INT R, VEC4 Col );

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
  VOID (*PutFilledCircle)( INT X0, INT Y0, INT R, VEC4 Col );

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
  VOID (*PutTorus)( INT X0, INT Y0, INT R, INT Len, VEC4 Col );

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
  VOID (*PutSmoothRect)( INT X1, INT Y1, INT X2, INT Y2, INT R,
                         VEC4 ColBorder, VEC4 ColFill );

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
  VOID (*PutPill)( INT X1, INT Y1, INT X2, INT Y2,
                   VEC4 ColBorder, VEC4 ColFill );

  /* Set font size in current 2D drawing structure function.
   * ARGUMENTS:
   *   - size of font:
   *       INT Size;
   * RETURNS: None.
   */
  VOID (*PutTextSetSize)( INT Size );

  /* Set font size in current 2D drawing structure function.
   * ARGUMENTS:
   *   - font name (can be taken from MS Word):
   *       CHAR *FontName;
   *   - bit flags (see TTP_FONT params):
   *       DWORD BitFlags;
   * RETURNS: None.
   */
  VOID (*PutTextSetFont)( CHAR *FontName, DWORD BitFlags );

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
  VOID (*PutText)( CHAR *Str, INT X, INT Y, VEC4 Col );

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
  VOID (*PutWinControl)( INT X1, INT Y1, INT W, INT H, DWORD ControlType, DWORD ControlState );

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
  VOID (*PutImg)( ttpIMAGE *Img, INT X, INT Y, INT W, INT H );

  /* End draw to texture function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*DrawEnd)( VOID );
} ttpSYSTEM_UI_DRAW;

/***
 * UIMapper functions prototypes.
 ***/

/* UIMapper initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIMapperInit( VOID );

/* UIMapper deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIMapperClose( VOID );

/* User interface mapper context */
typedef struct tagttpSYSTEM_UI_MAPPER
{
  /* UI add object function.
   * ARGUMENTS: None.
   * RETURNS: 
   *   (ttpUI_CONTROL *) added object.
   */
  ttpUI_CONTROL * (*UIAddObj)( VOID );

  /* UI add bool value function.
   * ARGUMENTS: None.
   *   - object where to add:
   *       ttpUI_CONTROL *O
   *   - bool value to add:
   *       BOOL *P
   * RETURNS: None.
   */
  VOID (*UIAddBoolValue)( ttpUI_CONTROL *O, BOOL *P );

  /* UI add double value function.
   * ARGUMENTS: None.
   *   - object where to add:
   *       ttpUI_CONTROL *O
   *   - DBL value to add:
   *       DBL *P
   * RETURNS: None.
   */
  VOID (*UIAddDblValue)( ttpUI_CONTROL *O, DBL *P );

  /* UI add float value function.
   * ARGUMENTS:
   *   - object where to add:
   *       ttpUI_CONTROL *O;
   *   - FLT value to add:
   *       FLT *P;
   * RETURNS: None.
   */
  VOID (*UIAddFltValue)( ttpUI_CONTROL *O, FLT *P );

  /* UI add image to control object function function.
   * ARGUMENTS: None.
   *   - object where to add:
   *       ttpUI_CONTROL *B;
   *   - pointer to image to add:
   *       ttpIMAGE *Img;
   * RETURNS:
   *   (INT) Number of image in array (-1 if NumOfImg >= TTP_UI_MAX_IMG).
   */
  INT (*UIAddImg)( ttpUI_CONTROL *B, ttpIMAGE *Img );

  /* UI add control object to bottom of list function.
   * ARGUMENTS: None.
   *   - object where to add:
   *       ttpUI_CONTROL *List;
   *   - object to add:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*UIAddToListBottom)( ttpUI_CONTROL *List, ttpUI_CONTROL *B );

  /* UI control update function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*UIUpdate)( VOID );

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
  ttpUI_CONTROL * (*UICreateButton)( INT Left, INT Top, INT W, INT H );

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
  ttpUI_CONTROL * (*UICreateSlider)( INT Left, INT Top, INT W, INT H, DBL Step, DBL From, DBL To );

  /* UI create checkbox function.
   * ARGUMENTS:
   *   - coordinates of corners & sizes:
   *       INT Left, Top, W, H;
   * RETURNS: 
   *   (ttpUI_CONTROL *) created checkbox.
   */
  ttpUI_CONTROL * (*UICreateCheckBox)( INT Left, INT Top, INT W, INT H );

  /* UI create list function.
   * ARGUMENTS:
   *   - coordinates of corners & sizes:
   *       INT Left, Top, W, H;
   * RETURNS: 
   *   (ttpUI_CONTROL *) created checkbox.
   */
  ttpUI_CONTROL * (*UICreateList)( INT Left, INT Top, INT W, INT H );

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
  ttpUI_CONTROL * (*UICreateDefButton)( VOID );

  /* UI create slider by default function.
   * ARGUMENTS:
   *   - slider to create by default:
   *       ttpUI_CONTROL *B;
   * RETURNS: 
   *   (ttpUI_CONTROL *) created button.
   */
  ttpUI_CONTROL * (*UICreateDefSlider)( VOID );

  /* UI create checkbox by default function.
   * ARGUMENTS:
   *   - slider to create by default:
   *       ttpUI_CONTROL *B;
   * RETURNS: 
   *   (ttpUI_CONTROL *) created button.
   */
  ttpUI_CONTROL * (*UICreateDefCheckBox)( VOID );

  /* UI add tab object to tab function.
   * ARGUMENTS:
   *   - object where to add:
   *       ttpUI_CONTROL *List;
   *   - tab object to add:
   *       ttpUI_CONTROL_TAB_OBJECT *Tab;
   * RETURNS: None.
   */
  VOID (*UIAddTabObjToTab)( ttpUI_CONTROL_TAB_OBJECT *Tab );

  /* UI add button to tab object function.
   * ARGUMENTS:
   *   - tab object where to add:
   *       ttpUI_CONTROL_TAB_OBJECT *Tab;
   *   - object to add:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*UIAddButtonToTab)( ttpUI_CONTROL_TAB_OBJECT *Tab, ttpUI_CONTROL *B );

  /* UI create tab object function.
   * ARGUMENTS:
   *   - tab name:
   *       CHAR *Name;
   * RETURNS: 
   *   (ttpUI_CONTROL_TAB_OBJECT *) created tab object.
   */
  ttpUI_CONTROL_TAB_OBJECT * (*UICreateTabObj)( CHAR *Name );

  /* UI set active tab by name.
   * ARGUMENTS:
   *   - name of tab:
   *       CHAR *Name;
   * RETURNS: None.
   */
  VOID (*UISetActiveTab)( CHAR *Name );

  /* UI add button to active tab object function.
   * ARGUMENTS:
   *   - object to add:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*UIAddButtonToActiveTab)( ttpUI_CONTROL *B );

  /* UI control response round button function.
   * ARGUMENTS:
   *   - self-pointer to button:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*UIResponseRoundButton)( ttpUI_CONTROL *B );

  /* UI control render button function.
   * ARGUMENTS:
   *   - self-pointer to button:
   *       ttpUI_CONTROL *B;
   * RETURNS: None.
   */
  VOID (*UIRenderRoundButton)( ttpUI_CONTROL *B );
} ttpSYSTEM_UI_MAPPER;

/* Texture view store data type */
typedef struct tagttpTEXTURE_VIEW
{
  ttpTEXTURE *Tex;      /* Draw texture pointer (non-deletable) */
  ttpTEXTURE *TexTitle; /* Window caption texture pointer (deletable) */
  ttpMATERIAL *Mtl;     /* Texture view draw material */
  ttpPRIM *Pr;          /* Texture view primitive */
  VEC4 UBOData[2];      /* Texture material draw data: <WinX, WinY, WinW, WinTitleH>,<TexW, TexH, WinH, IsDownward> */
  BOOL IsSize;          /* Resize or move window flag */
  BOOL IsCloseble;      /* Window ñlose possibility flag  */
  BOOL IsResizable;     /* Window resize possibility flag  */
} ttpTEXTURE_VIEW;

/* User interface context */
typedef struct tagttpSYSTEM_UI
{
  /* Draw to texture system */
  union
  {
    ttpSYSTEM_UI_DRAW;
    ttpSYSTEM_UI_DRAW DrawToTex;
  };
  /* UI controls system */
  union
  {
    ttpSYSTEM_UI_CONTROLS;
    ttpSYSTEM_UI_CONTROLS UIControls;
  };
  /* UI mapper system */
  union
  {
    ttpSYSTEM_UI_MAPPER;
    ttpSYSTEM_UI_MAPPER UIMapper;
  };

  /***
   * Texture view function group
   ***/

  /* Add texture to view function.
   * ARGUMENTS:
   *   - texture to add:
   *       ttpTEXTURE *Tex;
   *   - window screen position:
   *       INT X, Y;
   *   - window width:
   *       INT WinW;
   *   - window closable flag:
   *       BOOL IsCloseble;
   *   - window resizable flag:
   *       BOOL IsResizable;
   *   - texture vertical flip flag:
   *       BOOL IsTextureFlip;
   * RETURNS:
   *   (ttpTEXTURE_VIEW *) added texture info data.
   */
  ttpTEXTURE_VIEW * (*TexViewAdd)( ttpTEXTURE *Tex, INT X, INT Y, INT WinW, BOOL IsCloseble, BOOL IsResizable, BOOL IsTextureFlip );

  /* Add texture to view function.
   * ARGUMENTS:
   *   - texture view data pointer to delete:
   *       ttpTEXTURE_VIEW *TV;
   * RETURNS: None.
   */
  VOID (*TexViewDelete)( ttpTEXTURE_VIEW *TV );

  /* Resize texture view function.
   * ARGUMENTS:
   *   - texture view data pointer to delete:
   *       ttpTEXTURE_VIEW *TV;
   *   - new texture view window width:
   *       INT NewW;
   * RETURNS: None.
   */
  VOID (*TexViewResize)( ttpTEXTURE_VIEW *TV, INT NewW );

  /* Move texture view function.
   * ARGUMENTS:
   *   - texture view data pointer to delete:
   *       ttpTEXTURE_VIEW *TV;
   *   - new texture view window position:
   *       INT X, Y;
   * RETURNS: None.
   */
  VOID (*TexViewMove)( ttpTEXTURE_VIEW *TV, INT X, INT Y );
} ttpSYSTEM_UI;

/***
 * User interface init & deinit functions.
 ***/

/* UI initializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIInit( VOID );

/* UI deinitializtion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UIClose( VOID );

/* Texture view initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_UITexViewInit( VOID );

#endif /* __ttp_ui_h_ */

/* END OF 'ttp_ui.h' FILE */
