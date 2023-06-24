/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_cloth.c
 * PURPOSE     : Tough Third Planet project.
 *               Cloth simulation.
 * PROGRAMMER  : CGSG'2022.
 *               Sofrygin Luka (LS4),
 * LAST UPDATE : 17.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"

/* Cloth force accumulation function.
 * ARGUMENTS:
 *   - Cloth object to apply forces to:
 *       ttpCLOTH *Cloth;
 * RETURNS: None.
 */
static VOID TTP_ClothAccumulateForcesDefault( ttpCLOTH *Cloth )
{
  INT i;

  for (i = 0; i < Cloth->W  * Cloth->H; i++)
    Cloth->Forces[i] = Vec3MulNum(VEC3_DOWN, Cloth->Weight);
} /* End of 'TTP_ClothAccumulateForcesDefault' function */

/* Point-sphere penetration points finder
 * ARGUMENTS:
 *   - Point to check penetration:
 *       VEC3 Point;
 *   - Center of a sphere:
 *       VEC3 C;
 *   - Radius of a sphere:
 *       DBL R;
 *   - Closest to the Point point on the surface:
 *       VEC3 Q;
 * RETURNS: TRUE of FALSE whether the penetration succeed.
 */
static BOOL TTP_GetPointSpherePenetrationPoints( VEC3 Point, VEC3 C, DBL R, VEC3 *Q )
{
  if ((Vec3Len2(Vec3SubVec(Point, C)) > R * R))
    return FALSE;
  *Q = Vec3AddVec(C, Vec3MulNum(Vec3SubVec(Point, C), R / Vec3Len(Vec3SubVec(Point, C))));
  return TRUE;
} /* End of 'TTP_GetPointSpherePenetrationPoints' function */

/* Point-circle penetration points finder
 * ARGUMENTS:
 *   - Point to check penetration:
 *       VEC3 Point;
 *   - Center of a circle:
 *       VEC3 C;
 *   - Radius of a circle:
 *       DBL R;
 *   - Closest to the Point point on the surface:
 *       VEC3 Q;
 * RETURNS: TRUE of FALSE whether the penetration succeed.
 */
static BOOL TTP_GetPointCirclePenetrationPoints( VEC3 Point, VEC3 C, DBL R, VEC3 *Q )
{
  if ((Vec2Len2(Vec2SubVec(Vec2Set(Point.X, Point.Z), Vec2Set(C.X, C.Z))) > R * R) || Point.Y > C.Y || Point.Y < C.Y - 1)
    return FALSE;

  *Q = Vec3Set(Point.X, C.Y, Point.Z);
  return TRUE;
} /* End of 'TTP_GetPointCirclePenetrationPoints' function */

/* Cloth initialization function.
 * ARGUMENTS:
 *   - Cloth object to initialize:
 *       ttpCLOTH *Cloth;
 *   - Height and width of cloth in particles:
 *       INT W, INT H;
 *   - Amount of constraints to be allocated:
 *        INT ConstrainCount;
 *   - Weight of cloth
 *        FLOAT Weight;
 * RETURNS: None.
 */
static VOID TTP_ClothCreate( ttpCLOTH *Cloth, INT W, INT H, INT ConstrainCount, FLT Weight, FLT Stiffness, FLT Friction )
{
  memset(Cloth, 0, sizeof(ttpCLOTH));
  Cloth->P = malloc(W * H * sizeof(VEC3));
  Cloth->OldP = malloc(W * H * sizeof(VEC3));
  Cloth->Forces = malloc(W * H * sizeof(VEC3));
  Cloth->Normals = malloc(W * H * sizeof(VEC3));
  Cloth->Constraints = malloc(ConstrainCount * sizeof(ttpCONSTRAINT));
  memset(Cloth->Constraints, 0, ConstrainCount * sizeof(ttpCONSTRAINT));
  Cloth->NumOfConstraints = ConstrainCount;
  Cloth->Weight = Weight;
  Cloth->W = W;
  Cloth->H = H;
  Cloth->Stiffness = Stiffness;
  Cloth->Friction = Friction;
  Cloth->AccumulateForces = TTP_ClothAccumulateForcesDefault;
} /* End of 'TTP_UnitClose' function */

/* Verlet step taking function. Counts new position using current and previous ones.
 * ARGUMENTS:
 *   - Cloth object to move:
 *       ttpCLOTH *Cloth;
 * RETURNS: None.
 */
static VOID TTP_ClothVerletStep( ttpCLOTH *Cloth ) 
{
  INT i;
  VEC3 Old;

  for (i = 0; i < Cloth->W * Cloth->H; i++) 
  {
    Old = Cloth->P[i];

    Cloth->P[i] = Vec3AddVec3(
      Cloth->P[i], 
      Vec3MulNum(Vec3SubVec(Cloth->P[i], Cloth->OldP[i]), Cloth->Friction), 
      Vec3MulNum(Cloth->Forces[i], Ttp->GlobalDeltaTime));
    Cloth->OldP[i] = Old;
  }
} /* End of 'TTP_ClothVerletStep' function */

/* Cloth constraints satisfacion function.
 * ARGUMENTS:
 *   - Cloth object to correct:
 *       ttpCLOTH *Cloth;
 * RETURNS: None.
 */
static VOID TTP_ClothSatisfyConstraints( ttpCLOTH *Cloth ) 
{
  INT i, j;

  for (i = 0; i < TTP_CLOTH_NUM_OF_ITERATIONS; i++)
  {
    if (Cloth->HandleCollisions != NULL)
      Cloth->HandleCollisions(Cloth);

    /* Satisfy cloth form */
    for (j = 0; j < Cloth->NumOfConstraints; j++)
    {
      VEC3 Delta = Vec3SubVec(Cloth->P[Cloth->Constraints[j].ParticleB], Cloth->P[Cloth->Constraints[j].ParticleA]);

      if (Vec3Len2(Delta) > Cloth->Constraints[j].RestLength * Cloth->Constraints[j].RestLength * 1.1 && Ttp->Keys['T'])
        Cloth->Constraints[j].ParticleA = Cloth->Constraints[j].ParticleB = 0;

      Cloth->Constraints[j].Stretch = Vec3Set1(0);

      /* If no damper */
      if (Cloth->Constraints[j].Damper == 0)
      {
        Delta = Vec3MulNum(
          Delta, 
          Cloth->Stiffness * Cloth->Constraints[j].RestLength * Cloth->Constraints[j].RestLength / 
          (Vec3Len2(Delta) + Cloth->Constraints[j].RestLength * Cloth->Constraints[j].RestLength) - 0.5);
        Cloth->Constraints[j].Stretch = Delta;

        Cloth->P[Cloth->Constraints[j].ParticleA] = Vec3SubVec(Cloth->P[Cloth->Constraints[j].ParticleA], Delta);
        Cloth->P[Cloth->Constraints[j].ParticleB] = Vec3AddVec(Cloth->P[Cloth->Constraints[j].ParticleB], Delta);
      } 
      /* If not Damper satisfied */
      else if (
        (Cloth->Constraints[j].RestLength - Cloth->Constraints[j].Damper) *
        (Cloth->Constraints[j].RestLength - Cloth->Constraints[j].Damper) > Vec3Len2(Delta) ||
        Cloth->Constraints[j].RestLength * Cloth->Constraints[j].RestLength < Vec3Len2(Delta)) 
      {
        Delta = Vec3MulNum(
          Delta, 
          Cloth->Stiffness * Cloth->Constraints[j].RestLength * Cloth->Constraints[j].RestLength / 
          (Vec3Len2(Delta) + (Cloth->Constraints[j].RestLength - Cloth->Constraints[j].Damper) * 
                             (Cloth->Constraints[j].RestLength - Cloth->Constraints[j].Damper)) - 0.5);
        Cloth->Constraints[j].Stretch = Delta;
        Cloth->P[Cloth->Constraints[j].ParticleA] = Vec3SubVec(Cloth->P[Cloth->Constraints[j].ParticleA], Delta);
        Cloth->P[Cloth->Constraints[j].ParticleB] = Vec3AddVec(Cloth->P[Cloth->Constraints[j].ParticleB], Delta);
      } else
         Cloth->Constraints[j].Stretch = Vec3Set(0, 1, 0);
    }
  }
  if (Cloth->HandleHardConstraints != NULL)
    Cloth->HandleHardConstraints(Cloth);
} /* End of 'TTP_ClothSatisfyConstraints' function */
 

/* Cloth freeing function.
 * ARGUMENTS:
 *   - Cloth object to free:
 *       ttpCLOTH *Cloth;
 * RETURNS: None.
 */
static VOID TTP_ClothFree( ttpCLOTH *Cloth )
{
  free(Cloth->P);
  free(Cloth->OldP);
  free(Cloth->Forces);
  free(Cloth->Constraints);
} /* End of 'TTP_ClothFree' function */
 
/* Default flat cloth creation function.
 * ARGUMENTS:
 *   - Pointer to cloth object:
 *       ttpCLOTH *Cloth;
 *   - Cloth width:
 *       INT W;
 *   - Cloth height:
 *       INT H;
 *   - Cloth height above ground:
 *       FLT Height;
 *   - Stiffness level:
 *       INT MaxBr;
 *   - Cloth particle weight:
 *       FLT Weight;
 * RETURNS: None.
 */
static VOID TTP_ClothCreateDefault( ttpCLOTH *Cloth, INT W, INT H, FLT Height, INT MaxBr )
{
  INT x, y, p = 0;
  /* Mid line break */
  INT Br;

  if (Cloth == NULL)
    return;

  TTP_ClothCreate(Cloth, W, H, 100000, 0.06, 1, 0.99);

  for (Br = 1; Br <= MaxBr; Br++)
  {
    /* Horisontal bonds */
    for (y = 0; y < Cloth->H; y++)
      for (x = 0; x < Cloth->W - Br; x++)
      {
        Cloth->Constraints[p + y * (Cloth->W - Br) + x].ParticleA = y * Cloth->W + x;
        Cloth->Constraints[p + y * (Cloth->W - Br) + x].ParticleB = y * Cloth->W + x + Br;
        Cloth->Constraints[p + y * (Cloth->W - Br) + x].RestLength = Br;
      }

    /* Offset */
    p += Cloth->H * (Cloth->W - Br);

    /* Vertical bonds */
    for (y = 0; y < Cloth->H - Br; y++)
      for (x = 0; x < Cloth->W; x++)
      {
        Cloth->Constraints[p + y * Cloth->W + x].ParticleA = y * Cloth->W + x;
        Cloth->Constraints[p + y * Cloth->W + x].ParticleB = (y + Br) * Cloth->W + x;
        Cloth->Constraints[p + y * Cloth->W + x].RestLength = Br;
      }
    p += Cloth->W * (Cloth->H - Br);
  }
  Cloth->NumOfConstraints = p;

  /* Cloth particle positions */
  for (y = 0; y < Cloth->H; y++)
    for (x = 0; x < Cloth->W; x++)
      Cloth->OldP[y * Cloth->W + x] = Cloth->P[y * Cloth->W + x] = Vec3Set(x, Height, y);
} /* End of 'TTP_ClothCreateDefault' function */

/* System unit inter frame events handle function.
 * ARGUMENTS:
 *   - Pointer to cloth object:
 *       ttpCLOTH *Cloth;
 *   - Speed (Amount of iterations)
 *       INT Speed;
 * RETURNS: None.
 */
static VOID TTP_ClothResponse( ttpCLOTH *Cloth, INT Speed )
{
  INT i;

  if (!Ttp->IsPause)
    for (i = 0; i < Speed; i++)
    {
      Cloth->AccumulateForces(Cloth);
      TTP_ClothVerletStep(Cloth);
      TTP_ClothSatisfyConstraints(Cloth);
    }
} /* End of 'TTP_ClothResponse' function */

/* System unit render function.
 * ARGUMENTS:
 *   - Pointer to a cloth to draw:
 *       ttpCLOTH *Cloth;
 * RETURNS: None.
 */
static VOID TTP_ClothDraw( ttpCLOTH *Cloth )
{
  INT x, y;

  /* Vertical bonds */
  for (y = 0; y < Cloth->H; y++)
    for (x = 0; x < Cloth->W; x++)
      Ttp->DrawCylinder(Cloth->P[Cloth->Constraints[y * (Cloth->W) + x].ParticleA], 0.3, 
                  Cloth->P[Cloth->Constraints[y * (Cloth->W) + x].ParticleB], 0.3,
                  Vec4SetVec3(Vec3MulNum(Cloth->Constraints[y * (Cloth->W) + x].Stretch, 80), 1), MatrIdentity());
  for (y = 0; y < Cloth->H - 1; y++)
    for (x = 0; x < Cloth->W; x++)
      Ttp->DrawCylinder(Cloth->P[Cloth->Constraints[Cloth->H * (Cloth->W - 1) + y * (Cloth->W) + x].ParticleA], 0.1, 
                  Cloth->P[Cloth->Constraints[Cloth->H * (Cloth->W - 1) + y * (Cloth->W) + x].ParticleB], 0.1,
                  Vec4Set(1 - Cloth->Constraints[Cloth->H * (Cloth->W - 1) + y * (Cloth->W) + x].Stretch.X * 80, 
                          1 - Cloth->Constraints[Cloth->H * (Cloth->W - 1) + y * (Cloth->W) + x].Stretch.Y * 80, 
                          1 - Cloth->Constraints[Cloth->H * (Cloth->W - 1) + y * (Cloth->W) + x].Stretch.Z * 80, 
                          1), MatrIdentity());
} /* End of 'TTP_ClothDraw' function */

/* Cloth hierarchy init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_ClothInit( VOID )
{
  Ttp->Cloth.ClothCreateDefault = TTP_ClothCreateDefault;
  Ttp->Cloth.ClothCreate = TTP_ClothCreate;
  Ttp->Cloth.ClothDraw = TTP_ClothDraw;
  Ttp->Cloth.ClothResponse = TTP_ClothResponse;
  Ttp->Cloth.ClothFree = TTP_ClothFree;
} /* End of 'TTP_ClothInit' function */

/* END OF 'ttp_cloth.c' FILE */
