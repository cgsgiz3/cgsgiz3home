/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_rigid_body.c
 * PURPOSE     : Tough Third Planet project.
 *               Rigid body simulation support.
 * PROGRAMMER  : CGSG'2022.
 *               Timur Minazhetdinov (TM2).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"

#include "ttp_rigid_body_def.h"
#include "ttp_rigid_body.h"

/***
 * Computing inertia tensor for different bodies functions.
 ***/

/* Compute inertia tensor for block function.
 * ARGUMENTS:
 *   - rigid block mass:
 *       DBL Mass;
 *   - sizes of block:
 *       INT SX, SY, SZ;
 * RETURNS:
 *   (MATR) computed block's inertia tensor.
 */
static MATR TTP_CompBlockInertiaTensor( DBL Mass, INT SX, INT SY, INT SZ )
{
  MATR M;

  M = MatrSet(Mass / 12 * SY * SY + SZ * SZ, 0, 0, 0,
              0, Mass / 12 * SX * SX + SZ * SZ, 0, 0,
              0, 0, Mass / 12 * SX * SX + SY * SY, 0,
              0,          0,          0,          1);
  return M;
} /* End of 'TTP_CompBlockInertiaTensor' function */

/***
 * Computing functions.
 ***/

/* Copy the state information into an array function.
 * ARGUMENTS:
 *   - source rigid body:
 *       ttpRIGID_BODY *RB;
 *   - destination info array:
 *       DBL *Y;
 * RETURNS: None.
 */
static VOID TTP_StateToArray( ttpRIGID_BODY *RB, DBL *Y )
{
  *Y++ = RB->Pos.X; /* X component of position */
  *Y++ = RB->Pos.Y; /* etc. */
  *Y++ = RB->Pos.Z;

  /* Copy quaternion */
  *Y++ = RB->q.R;
  *Y++ = RB->q.I;
  *Y++ = RB->q.J;
  *Y++ = RB->q.K;

  /* Copy momentums */
  *Y++ = RB->P.X; /* Linear momentum */
  *Y++ = RB->P.Y;
  *Y++ = RB->P.Z;
  *Y++ = RB->L.X; /* Angular momentum */
  *Y++ = RB->L.Y;
  *Y++ = RB->L.Z;
} /* End of 'TTP_StateToArray' function */

/* Copy information from an array into the state variables function.
 * ARGUMENTS:
 *   - destination rigid body:
 *       ttpRIGID_BODY *RB;
 *   - source info array:
 *       DBL *Y;
 * RETURNS: None.
 */
static VOID TTP_ArrayToState( ttpRIGID_BODY *RB, DBL *Y )
{
  RB->Pos.X = *Y++;
  RB->Pos.Y = *Y++;
  RB->Pos.Z = *Y++;

  /* Copy quaternion */
  RB->q.R = *Y++;
  RB->q.I = *Y++;
  RB->q.J = *Y++;
  RB->q.K = *Y++;

  /* Copy momentums */
  RB->P.X = *Y++; /* Linear momentum */
  RB->P.Y = *Y++;
  RB->P.Z = *Y++;
  RB->L.X = *Y++; /* Angular momentum */
  RB->L.Y = *Y++;
  RB->L.Z = *Y++;

  /* Compute auxiliary variables */
  RB->v = Vec3DivNum(RB->P, RB->mass);       /* v(t) = P(t) / M */
  /* I^(-1)(t) = R(t) I^(-1)\body R(t)^T */
  RB->Iinv = MatrMulMatr3(RB->R, RB->Ibodyinv, MatrTranspose(RB->R));
  RB->omega = VecMulMatr(RB->L, RB->Iinv);   /* w(t) = I-1(t)L(t) */
  RB->R = QuadToMatr(QuadNormalize(RB->q));  /* R(t) = quadtomatr(rb->q) */
} /* End of 'TTP_ArrayToState' function */

/* Calculate R(t) for body by omega vector function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC A;
 * RETURNS:
 *   (MATR3) final matrix3x3.
 */
static MATR3 Star( VEC3 A )
{
  MATR3 M;

  M = Matr3Set(0,   -A.Z, A.Y,
               A.Z, 0,    -A.X,
               A.Y, A.X,  0);
  return M;
} /* End of 'Star' function */

/* Compute result impact on contacting bodies function.
 * ARGUMENTS:
 *   - pointer to source contact:
 *       ttpCONTACT *CT;
 *   - coeff:
 *       DBL Epsilon;
 * RETURNS: None.
 */
VOID TTP_CompContactRes( ttpCONTACT *CT, DBL Epsilon )
{
  VEC3
    Vpa, Vpb,                           /* Velocity of bodies points */
    ra = Vec3SubVec(CT->P, CT->A->Pos), /* Displacement of A-body contact point */
    rb = Vec3SubVec(CT->P, CT->B->Pos); /* Displacement of B-body contact point */
  DBL Vrel;

  /* Compute velocity of contacting points */
  Vpa = Vec3AddVec(CT->A->v, Vec3CrossVec(CT->A->omega, ra));
  Vpb = Vec3AddVec(CT->B->v, Vec3CrossVec(CT->B->omega, rb));

  /* Compute relative velocity */
  Vrel = Vec3DotVec(CT->N, Vec3SubVec(Vpa, Vpb));

  /* Check situation */
  if (Vrel > 0) /* Bodies are moving apart */
    return; /* We don't need to worry about it */
  else if (Vrel = 0) /* Resting contact */
  {
    ///TODO: resting contact.
  }
  else /* Bodies collision */
  {
    
  }

} /* End of 'TTP_CompContactRes' function */

#if 0 ///TODO: Copying to/out array, temporarily don't use.

/* Copy information from an array into the bodies function.
 * ARGUMENTS:
 *   - source array:
 *       DBL *X;
 * RETURNS: None.
 */
static VOID TTP_ArrayToBodies( DBL *X )
{
  INT i;

  for (i = 0; i < NBODIES; i++)
    ArrayToState(&Bodies[i], &X[i * STATE_SIZE]);
} /* End of 'TTP_ArrayToBodies' function */

/* Copy information from the bodies into an array function.
 * ARGUMENTS:
 *   - destination array:
 *       DBL *X;
 * RETURNS: None.
 */
static VOID TTP_BodiesToArray( DBL *X )
{
  INT i;

  for(i = 0; i < NBODIES; i++)
    StateToArray(&Bodies[i], &X[i * STATE_SIZE]);
} /* End of 'TTP_BodiesToArray' function */

#endif /* 0 */

#if 0

/* Compute Dxdt for every body function.
 * ARGUMENTS:
 *   - change in time:
 *       DBL T;
 *   - information array:
 *       DBL *X;
 *   - information array 2:
 *       DBL *Xdot;
 * RETURNS: None.
 */
VOID TTP_Dxdt( DBL T, DBL *X, DBL *Xdot )
{
  /* Put data in x[] into Bodies[] */
  ArrayToBodies(x);
  for(int i = 0; i < NBODIES; i++)
  {
    ComputeForceAndTorque(t, &Bodies[i]);
    DdtStateToArray(&Bodies[i], &xdot[i * STATE_SIZE]);
  }
} /* End of 'TTP_Dxdt' function */

/* Real computing Dxdt for every bodies function.
 * ARGUMENTS:
 *   - source rigid body:
 *       RIGID_BODY *RB;
 *   - information array:
 *       DBL *Xdot;
 * RETURNS: None.
 */
void TTP_DdtStateToArray( ttpRIGID_BODY *RB, DBL *xdot )
{
  QUAD qdot;

  /* copy ddt x(t) = v(t) into xdot */
  *xdot++ = RB->v[0];
  *xdot++ = RB->v[1];
  *xdot++ = RB->v[2];

  /* Compute R'(t) = w(t)xR(t) */
  qdot = 0.5 * (rb->omega * rb->q);

  *xdot++ = qdot.r;
  *xdot++ = qdot.i;
  *xdot++ = qdot.j;
  *xdot++ = qdot.k;

  *xdot++ = rb->force[0]; /* ddt P(t) = F(t) */
  *xdot++ = rb->force[1];
  *xdot++ = rb->force[2];

  *xdot++ = rb->torque[0]; /* ddt L(t) = t(t) */
  *xdot++ = rb->torque[1];
  *xdot++ = rb->torque[2];
} /* End of 'TTP_DdtStateToArray' function */

#endif /* 0 */

/* Create rigid cube function.
 * ARGUMENTS:
 *   - rigid body total mass:
 *       DBL M;
 *   - rigid body position:
 *       VEC Pos;
 *   - rigid cube sizes:
 *       VEC3 Size;
 * RETURNS:
 *   (ttpRIGID_BODY *) pointer to filled with parameters cube.
 */
ttpRIGID_BODY * TTP_CreateRigidCube( DBL M, VEC3 Pos, VEC3 Size )
{
  ttpRIGID_BODY *RB;

  if ((RB = malloc(sizeof(ttpRIGID_BODY))) == NULL)
    return NULL;

  memset(RB, 0, sizeof(ttpRIGID_BODY));
  RB->mass = M;
  RB->Ibody = TTP_CompBlockInertiaTensor(M, Size.X, Size.Y, Size.Z);
  RB->Ibodyinv = MatrInverse(RB->Ibody);
  RB->Pos = Pos;
  RB->force = Vec3MulNum(VEC3_DOWN, 9.81);

  return RB;
} /* End of 'TTP_CreateRigidCube' function */

/* END OF 'ttp_rigid_body.c' FILE */
