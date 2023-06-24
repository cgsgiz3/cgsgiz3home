/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_balls.h
 * PURPOSE     : Tough Third Planet project.
 *               ...
 * PROGRAMMER  : CGSG'2022.
 *               Mihail Zalesskiy (MZ2).
 * LAST UPDATE : 17.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_balls_h_
#define __ttp_balls_h_

#include "ttp_def.h"

/* Ball type */
typedef struct tagttpBALL
{
  VEC3 Pos;    /* Position */
  VEC3 Vel;    /* Velocity */
  DBL Speed;   /* Speed */
  FLT Rad;     /* Radius */
  FLT Mass;    /* Mass */
} ttpBALL;

/* Plane type */
typedef struct tagttpPLANE
{
  VEC3 Pos;    /* Position */
  VEC3 Size;   /* Size */
} ttpPLANE;

/* Point type */
typedef struct tagttpBALL_POINT
{
  CHAR Flag;        /* Left or Right */
  INT BallId;       /* Id in balls array */
  FLT PosX;         /* Position on OX ray */
} ttpBALL_POINT;

/* Balls controller type */
typedef struct tagttpBALL_CONTROLLER
{
  ttpBALL *Arr;                      /* Array of balls */
  ttpBALL_POINT *PointsX;            /* Array of points on OX ray */
  INT **HitArr;                      /* Array of balls that we have to check on collision */
  ttpPLANE Plane;                    /* Plane */
  VEC3 Grav;                         /* Gravitation vector*/
  INT NumOfBalls;                    /* Count of balls */
  INT BlockOfBalls;                  /* Count of balls */
  DBL MaxSpeed;                      /* Max speed of balls */
  DBL MinSpeed;                      /* Min speed of balls */   
} ttpBALL_CONTROLLER;

/* Add point of ball function.
 * ARGUMENTS:
 *   - Position:
 *       VEC3 Pos;
 *   - Velocity:
 *       VEC3 Vel;
 *   - Speed:
 *       DBL Speed;
 *   - Radius:
 *       FLT Rad;
 *   - Mass:
 *       FLT Mass;
 * RETURNS:
 *   - Created ball:
 *       ttpBALL.
 */
ttpBALL TTP_BallCreate( VEC3 Pos, VEC3 Vel, DBL Speed, FLT Rad, FLT Mass );

/* Free balls function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_BallFree( ttpBALL_CONTROLLER *Cntrl );


/* Add point of ball function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ttpBALL_CONTROLLER *Cntrl;
 *   - id in PointX array:
 *       INT i;
 *   - Left or right:
 *       CHAR Flag;
 *   - Position:
 *       FLT PosX;
 *   - Id in Array of balls:
 *       INT BallId;
 * RETURNS: None.
 */
VOID TTP_BallAddPoint(ttpBALL_CONTROLLER *Cntrl, INT i, CHAR Flag, FLT PosX, INT BallId );

/* Swap variables function.
 * ARGUMENTS:
 *   - pointer to first element:
 *       ttpBALL_POINT *A;
 *   - pointer to second element:
 *       ttpBALL_POINT *B;
 * RETURNS: None.
 */
VOID TTP_BallSwapVar( ttpBALL_POINT *A, ttpBALL_POINT *B );

/* Selection sort function.
 * ARGUMENTS:
 *   - array to sort:
 *       ttpBALL_POINT *a;
 *   - count of elements in array:
 *       INT n;
 * RETURNS: None.
 */
VOID TTP_BallSelectionSort( ttpBALL_POINT *a, INT n );


/* Set HitArray function.
 * ARGUMENTS:
 *   - Pointer to balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_FillHitArray( ttpBALL_CONTROLLER *Cntrl );

/* Check collision function.
 * ARGUMENTS:
 *   - Pointer to balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_BallCheckCollision( ttpBALL_CONTROLLER *Cntrl );

/* Balls prepare function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_BallPrepare( ttpBALL_CONTROLLER *Cntrl );

/* Add a ball function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 *   - Position:
 *       VEC3 Pos;
 *   - Velosity:
 *       VEC3 Vel;
 *   - Speed:
 *       DBL Speed;
 *   - Radius:
 *       FLT Rad;
 *   - Mass:
 *       FLT Mass;
 * RETURNS: None.
 */
VOID TTP_BallAdd( ttpBALL_CONTROLLER *Cntrl, VEC3 Pos, VEC3 Vel, DBL Speed, FLT Rad, FLT Mass );

/* Init ball system function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 *   - Count of balls in one block:
 *       INT BlockOfBalls;
 *   - Position of plane:
 *       VEC3 Pos;
 *   - Size:
 *       VEC3 Size;
 *   - Max speed:
 *       DBL MaxSpeed;
 *   - Min speed:
 *       DBL MinSpeed;
 * RETURNS: None.
 */
VOID TTP_BallInit( ttpBALL_CONTROLLER *Cntrl, INT BlockOfBalls, VEC3 Pos, VEC3 Size, DBL MaxSpeed, DBL MinSpeed );

/* Structure of system of balls */
typedef struct tagttpSYSTEM_BALLS
{
  /* Add point of ball function.
   * ARGUMENTS:
   *   - Position:
   *       VEC3 Pos;
   *   - Velocity:
   *       VEC3 Vel;
   *   - Speed:
   *       DBL Speed;
   *   - Radius:
   *       FLT Rad;
   *   - Mass:
   *       FLT Mass;
   * RETURNS:
   *   - Created ball:
   *       ttpBALL.
   */
  ttpBALL (* BallCreate) ( VEC3 Pos, VEC3 Vel, DBL Speed, FLT Rad, FLT Mass );

  /* Free balls function.
   * ARGUMENTS:
   *   - Pointer to Balls controller:
   *       ttpBALL_CONTROLLER *Cntrl;
   * RETURNS: None.
   */
  VOID (* BallFree) ( ttpBALL_CONTROLLER *Cntrl );

  /* Add point of ball function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ttpBALL_CONTROLLER *Cntrl;
   *   - id in PointX array:
   *       INT i;
   *   - Left or right:
   *       CHAR Flag;
   *   - Position:
   *       FLT PosX;
   *   - Id in Array of balls:
   *       INT BallId;
   * RETURNS: None.
   */
  VOID (* BallAddPoint) (ttpBALL_CONTROLLER *Cntrl, INT i, CHAR Flag, FLT PosX, INT BallId );

  /* Set HitArray function.
   * ARGUMENTS:
   *   - Pointer to balls controller:
   *       ttpBALL_CONTROLLER *Cntrl;
   * RETURNS: None.
   */
  VOID (* FillHitArray) ( ttpBALL_CONTROLLER *Cntrl );

  /* Check collision function.
   * ARGUMENTS:
   *   - Pointer to balls controller:
   *       ttpBALL_CONTROLLER *Cntrl;
   * RETURNS: None.
   */
  VOID (* BallCheckCollision) ( ttpBALL_CONTROLLER *Cntrl );

  /* Balls prepare function.
   * ARGUMENTS:
   *   - Pointer to Balls controller:
   *       ttpBALL_CONTROLLER *Cntrl;
   * RETURNS: None.
   */
  VOID (* BallPrepare) ( ttpBALL_CONTROLLER *Cntrl );

  /* Add a ball function.
   * ARGUMENTS:
   *   - Pointer to Balls controller:
   *       ttpBALL_CONTROLLER *Cntrl;
   *   - Position:
   *       VEC3 Pos;
   *   - Velosity:
   *       VEC3 Vel;
   *   - Speed:
   *       DBL Speed;
   *   - Radius:
   *       FLT Rad;
   *   - Mass:
   *       FLT Mass;
   * RETURNS: None.
   */
  VOID (* BallAdd) ( ttpBALL_CONTROLLER *Cntrl, VEC3 Pos, VEC3 Vel, DBL Speed, FLT Rad, FLT Mass );

  /* Init ball system function.
   * ARGUMENTS:
   *   - Pointer to Balls controller:
   *       ttpBALL_CONTROLLER *Cntrl;
   *   - Count of balls in one block:
   *       INT BlockOfBalls;
   *   - Position of plane:
   *       VEC3 Pos;
   *   - Size:
   *       VEC3 Size;
   *   - Max speed:
   *       DBL MaxSpeed;
   *   - Min speed:
   *       DBL MinSpeed;
   * RETURNS: None.
   */
  VOID (* BallInit) ( ttpBALL_CONTROLLER *Cntrl, INT BlockOfBalls, VEC3 Pos, VEC3 Size, DBL MaxSpeed, DBL MinSpeed);

} ttpSYSTEM_BALLS;

/* System init ball system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_BallsInit( VOID );

#endif /* __ttp_balls_h_ */

/* END OF 'ttp_balls.h' FILE */