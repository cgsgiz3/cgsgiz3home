/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_rigid_body.h
 * PURPOSE     : Tough Third Planet project.
 *               Rigid body simulation declaration file.
 * PROGRAMMER  : CGSG'2022.
 *               Timur Minazhetdinov (TM2).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_rigid_body_h_
#define __ttp_rigid_body_h_

#include "ttp_def.h"

/* Number of values for each rigid body */
#define STATE_SIZE 13

/* Rigid body representation type */
typedef struct tagttpRIGID_BODY
{
  /* Constant quantities */
  DBL mass;      /* mass M */
  MATR Ibody,    /* Ibody */
       Ibodyinv; /* I^-1 body (inverse of Ibody) */

  /* State variables */
  VEC3 Pos;      /* x(t) - body position */
  QUAD q;        /* q(t) - rotation quaternion */
  VEC3 P,        /* P(t) - body linear momentum */
       L;        /* L(t) - body angular momentum */

  /* Derived quantities (auxiliary variables) */
  MATR Iinv,     /* I^-1(t) */
       R;        /* R(t) */
  VEC3 v,        /* v(t) - body velocity */
       omega;    /* w(t) */

  /* Computed quantities */
  VEC3 force,    /* F(t) */
       torque;   /* t(t) */
} ttpRIGID_BODY;

/* Two bodies contact representation type */
typedef struct tagttpCONTACT
{
  ttpRIGID_BODY *A,  /* body containing vertex */
                *B;  /* body containing face */
  VEC3 P,            /* world-space contact vertex location */
       N,            /* outwards pointing normal of face */
       Ea,           /* edge direction for A */
       Eb;           /* edge direction for B */
  BOOL Vf;           /* true if vertex/face contact, otherwise edge/edge */
} ttpCONTACT;

#endif /* __ttp_rigid_body_h_ */

/* END OF 'ttp_rigid_body.h' FILE */
