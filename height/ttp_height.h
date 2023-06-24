/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_height.c
 * PURPOSE     : Tough Third Planet project.
 *               Height system declaration module.
 * PROGRAMMER  : CGSG'2022.
 *               Alexander Skorikov(AS1),
 *               Roman Pischenko(RP5).
 * LAST UPDATE : 28.04.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#ifndef __ttp_height_h
#define __ttp_height_h

#include "ttp_def.h"

/* Height system struct */
typedef struct tagttpSYSTEM_HEIGHT
{
  /* Add new triangle to collection function.
   * ARGUMENTS:
   *   - triangle vertices(CCW format):
   *       VEC3 P0, P1, P2;
   * RETURNS: None.
   */
  VOID (* HeightAddTriangle)( VEC3 P0, VEC3 P1, VEC3 P2 );

  /* System height reset function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (* HeightReset)( VOID );

  /* Get height by point coordinates function.
   * ARGUMENTS:
   *   - point coordinates to get height:
   *       VEC3 P;
   * RETURNS:
   *   (FLT)height of point.
   */
  FLT (* HeightGetH)( VEC3 P );

  /* Get height by point coordinates function.
   * ARGUMENTS:
   *   - point coordinates to get height:
   *       VEC3 P;
   *   - handle to normal vector variable:
   *       VEC3 *N;
   * RETURNS:
   *   (FLT)height of point.
   */
  FLT (* HeightGetNH)( VEC3 P, VEC3 *N );

  /* Get height by point coordinates function.
   * ARGUMENTS:
   *   - point coordinates to get height:
   *       VEC3 P;
   * RETURNS:
   *   (BOOL)TRUE if triangle including P.X & P.Z exists, (BOOL)FALSE otherwise.
   */
  BOOL (* HeightIsHit)( VEC3 P );

} ttpSYSTEM_HEIGHT;

/* Triangle struct */
typedef struct tagttpTRIANGLE
{
  VEC3
    P0, P1, P2;           /* Points of triangle in CCW */
  FLT S,                  /* Area of triangle */
    MinXCoord, MaxXCoord, /* Min and max X-coordinates of triangle */
    MinZCoord, MaxZCoord; /* Min and max Z-coordinates of triangle */
  INT NumOfTriangle;      /* Number of triangle */
} ttpTRIANGLE;

/* System height initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_SystemHeightInit( VOID );

/* System height deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_SystemHeightClose( VOID );
#endif /* __ttp_height_h */
/* END OF 'ttp_height.c' FILE */
