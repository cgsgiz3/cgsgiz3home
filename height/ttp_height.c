/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_height.c
 * PURPOSE     : Tough Third Planet project.
 *               Height system handle module.
 * PROGRAMMER  : CGSG'2022.
 *               Alexander Skorikov(AS1),
 *               Roman Pischenko(RP5).
 * LAST UPDATE : 28.04.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"


#define FORWARD_ALLOCATION 100

typedef struct tagttpSTORE
{
  ttpTRIANGLE *Triangles;
  INT NumOfTriangles, MaxNumOfTriangles;
} ttpSTORE;

static ttpSTORE *Bulk;

/* Triangles sort by X-axis function.
 * ARGUMENTS:
 *   - Array to sort:
 *       ttpSTORE *A;
 *   - Size of array:
 *       INT N;
 * RETURNS: None.
 */
static VOID SortX( ttpSTORE *A, int N )
{
  INT j, i, min;
  ttpTRIANGLE tmp;

  for (j = 0; j < N; j++)
  {
    for (i = j + 1, min = j; i < N; i++)
      if (A->Triangles[i].MinXCoord < A->Triangles[min].MinXCoord)
        min = i;
    COM_SWAP(A->Triangles[i], A->Triangles[min], tmp);
  }
} /* End of 'SortX' function */


/* Add new triangle to collection function.
 * ARGUMENTS:
 *   - triangle vertices(CCW format):
 *       VEC3 P0, P1, P2;
 * RETURNS: None.
 */
static VOID TTP_SystemHeightAddTriangle( VEC3 P0, VEC3 P1, VEC3 P2 )
{
  FLT S;

  if (Bulk->NumOfTriangles >= Bulk->MaxNumOfTriangles)
  {
    ttpTRIANGLE *NewBlock;

    if ((NewBlock = malloc(sizeof(ttpTRIANGLE) * (Bulk->MaxNumOfTriangles + FORWARD_ALLOCATION))) == NULL)
      return;
    Bulk->MaxNumOfTriangles += FORWARD_ALLOCATION;
    if (Bulk->Triangles != NULL)
    {
      memcpy(NewBlock, Bulk->Triangles, sizeof(ttpTRIANGLE) * Bulk->NumOfTriangles);
      free(Bulk->Triangles);
    }
    Bulk->Triangles = NewBlock;
  }
  S = Vec3Len(Vec3CrossVec(Vec3SubVec(P1, P0), Vec3SubVec(P2, P0))) / 2.0;
  Bulk->NumOfTriangles++;
  Bulk->Triangles[Bulk->NumOfTriangles - 1].P0 = P0;
  Bulk->Triangles[Bulk->NumOfTriangles - 1].P1 = S > 0 ? P1 : P2;
  Bulk->Triangles[Bulk->NumOfTriangles - 1].P2 = S > 0 ? P2 : P1;
  Bulk->Triangles[Bulk->NumOfTriangles - 1].S = fabs(S);
  Bulk->Triangles[Bulk->NumOfTriangles - 1].NumOfTriangle = Bulk->NumOfTriangles;
  Bulk->Triangles[Bulk->NumOfTriangles - 1].MinXCoord = MIN3(P0.X, P1.X, P2.X);
  Bulk->Triangles[Bulk->NumOfTriangles - 1].MaxXCoord = MAX3(P0.X, P1.X, P2.X);
  Bulk->Triangles[Bulk->NumOfTriangles - 1].MinZCoord = MIN3(P0.Z, P1.Z, P2.Z);
  Bulk->Triangles[Bulk->NumOfTriangles - 1].MaxZCoord = MAX3(P0.Z, P1.Z, P2.Z);

  SortX(Bulk, Bulk->NumOfTriangles - 1);
} /* End of 'TTP_SystemHeightAddTriangle' function */

/* System height reset function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_SystemHeightReset( VOID )
{
  free(Bulk->Triangles);
  Bulk->NumOfTriangles = Bulk->MaxNumOfTriangles = 0;
} /* End of 'TTP_SystemHeightReset' function */

/* Get height by point coordinates function.
 * ARGUMENTS:
 *   - point coordinates to get height:
 *       VEC3 P;
 * RETURNS:
 *   (FLT)height of point.
 */
static FLT TTP_SystemHeightGetH( VEC3 P )
{
  INT i;
  FLT D, t = 0, u, v, s1dots2;
  VEC3 p0, p1, p2, M, N, s1, s2;


  for (i = 0; i < Bulk->MaxNumOfTriangles; i++)
  {
    if (P.X > Bulk->Triangles[i].MaxXCoord || P.X < Bulk->Triangles[i].MinXCoord || P.Z > Bulk->Triangles[i].MaxZCoord || P.Z < Bulk->Triangles[i].MinZCoord)
      continue;
    p0 = Bulk->Triangles[i].P0;
    p1 = Bulk->Triangles[i].P1;
    p2 = Bulk->Triangles[i].P2;
    s1 = Vec3SubVec(p1, p0), s2 = Vec3SubVec(p2, p0), s1dots2 = Vec3DotVec(s1, s2);
    N = Vec3CrossVec(s1, s2);
    M = Vec3Set((p0.X + p1.X + p2.X) / 3.0, (p0.Y + p1.Y + p2.Y) / 3.0, (p0.Z + p1.Z + p2.Z) / 3.0);
    M = (P.X == M.X && P.Z == M.Z) ? M : p0;
    D = N.X * M.X + N.Y * M.Y + N.Z * M.Z;
    u = Vec3DotVec(Vec3SubVec(P, p0), Vec3DivNum(Vec3SubVec(Vec3MulNum(s1, Vec3Len2(s2)), Vec3MulNum(s2, s1dots2)), Vec3Len2(s1) * Vec3Len2(s2) - s1dots2 * s1dots2));
    v = Vec3DotVec(Vec3SubVec(P, p0), Vec3DivNum(Vec3SubVec(Vec3MulNum(s2, Vec3Len2(s1)), Vec3MulNum(s1, s1dots2)), Vec3Len2(s1) * Vec3Len2(s2) - s1dots2 * s1dots2));
    if (u >= -0.001 && v >= -0.001 && u <= 1.001 && v <= 1.001 && u + v <= 1.001)
    {
      t = -(Vec3DotVec(Vec3Set(P.X, 0, P.Z), N) - D) / Vec3DotVec(Vec3Set(0, 1, 0), N);
      break;
    }
  }
  return t;
} /* End of 'TTP_SystemHeightGetH' function */

/* Get height by point coordinates function.
 * ARGUMENTS:
 *   - point coordinates to get height:
 *       VEC3 P;
 *   - handle to normal vector variable:
 *       VEC3 *N;
 * RETURNS:
 *   (FLT)height of point.
 */
static FLT TTP_SystemHeightGetNH( VEC3 P, VEC3 *N )
{
  INT i;
  FLT D, t = 0, u, v, s1dots2;
  VEC3 p0, p1, p2, M, N1, s1, s2;

  for (i = 0; i < Bulk->MaxNumOfTriangles; i++)
  {
    if (P.X >= Bulk->Triangles[i].MaxXCoord || P.X < Bulk->Triangles[i].MinXCoord || P.Z >= Bulk->Triangles[i].MaxZCoord || P.Z < Bulk->Triangles[i].MinZCoord)
      continue;
    p0 = Bulk->Triangles[i].P0;
    p1 = Bulk->Triangles[i].P1;
    p2 = Bulk->Triangles[i].P2;
    s1 = Vec3SubVec(p1, p0), s2 = Vec3SubVec(p2, p0), s1dots2 = Vec3DotVec(s1, s2);
    N1 = Vec3CrossVec(s1, s2);
    M = Vec3Set((p0.X + p1.X + p2.X) / 3.0, (p0.Y + p1.Y + p2.Y) / 3.0, (p0.Z + p1.Z + p2.Z) / 3.0);
    M = (P.X == M.X && P.Z == M.Z) ? M : p0;
    D = N1.X * M.X + N1.Y * M.Y + N1.Z * M.Z;
    u = Vec3DotVec(Vec3SubVec(P, p0), Vec3DivNum(Vec3SubVec(Vec3MulNum(s1, Vec3Len2(s2)), Vec3MulNum(s2, s1dots2)), Vec3Len2(s1) * Vec3Len2(s2) - s1dots2 * s1dots2));
    v = Vec3DotVec(Vec3SubVec(P, p0), Vec3DivNum(Vec3SubVec(Vec3MulNum(s2, Vec3Len2(s1)), Vec3MulNum(s1, s1dots2)), Vec3Len2(s1) * Vec3Len2(s2) - s1dots2 * s1dots2));
    if (u >= -0.001 && v >= -0.001 && u <= 1.001 && v <= 1.001 && u + v <= 1.001)
    {
      t = -(Vec3DotVec(Vec3Set(P.X, 0, P.Z), N1) - D) / Vec3DotVec(Vec3Set(0, 1, 0), N1), N = &N1;
      break;
    }
  }
  return t;
} /* End of 'TTP_SystemHeightGetNH' function */

/* Get height by point coordinates function.
 * ARGUMENTS:
 *   - point coordinates to get height:
 *       VEC3 P;
 * RETURNS:
 *   (BOOL)TRUE if triangle including P.X & P.Z exists, (BOOL)FALSE otherwise.
 */
static BOOL TTP_SystemHeightIsHit( VEC3 P )
{
  INT i;
  FLT D, t = 0, u, v, s1dots2;
  VEC3 p0, p1, p2, M, N, s1, s2;

  for (i = 0; i < Bulk->MaxNumOfTriangles; i++)
  {
    if (P.X >= Bulk->Triangles[i].MaxXCoord || P.X < Bulk->Triangles[i].MinXCoord || P.Z >= Bulk->Triangles[i].MaxZCoord || P.Z < Bulk->Triangles[i].MinZCoord)
      continue;
    p0 = Bulk->Triangles[i].P0;
    p1 = Bulk->Triangles[i].P1;
    p2 = Bulk->Triangles[i].P2;
    s1 = Vec3SubVec(p1, p0), s2 = Vec3SubVec(p2, p0), s1dots2 = Vec3DotVec(s1, s2);
    N = Vec3CrossVec(s1, s2);
    M = Vec3Set((p0.X + p1.X + p2.X) / 3.0, (p0.Y + p1.Y + p2.Y) / 3.0, (p0.Z + p1.Z + p2.Z) / 3.0);
    M = (P.X == M.X && P.Z == M.Z) ? M : p0;
    D = N.X * M.X + N.Y * M.Y + N.Z * M.Z;
    u = Vec3DotVec(Vec3SubVec(P, p0), Vec3DivNum(Vec3SubVec(Vec3MulNum(s1, Vec3Len2(s2)), Vec3MulNum(s2, s1dots2)), Vec3Len2(s1) * Vec3Len2(s2) - s1dots2 * s1dots2));
    v = Vec3DotVec(Vec3SubVec(P, p0), Vec3DivNum(Vec3SubVec(Vec3MulNum(s2, Vec3Len2(s1)), Vec3MulNum(s1, s1dots2)), Vec3Len2(s1) * Vec3Len2(s2) - s1dots2 * s1dots2));
    if (u >= -0.001 && v >= -0.001 && u <= 1.001 && v <= 1.001 && u + v <= 1.001)
    {
      t = -(Vec3DotVec(Vec3Set(P.X, 0, P.Z), N) - D) / Vec3DotVec(Vec3Set(0, 1, 0), N);
      break;
    }
  }
  return !!t;
} /* End of 'TTP_SystemHeightIsHit' function */

/* System height deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_SystemHeightClose( VOID )
{
  if (Bulk->Triangles != NULL)
    free(Bulk->Triangles);
  Bulk->Triangles = NULL;
  Bulk->NumOfTriangles = Bulk->MaxNumOfTriangles = 0;
} /* End of 'TTP_SystemHeightClose' function */

/* System height initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_SystemHeightInit( VOID )
{
  Ttp->HeightReset = TTP_SystemHeightReset;
  Ttp->HeightAddTriangle = TTP_SystemHeightAddTriangle;
  Ttp->HeightGetH = TTP_SystemHeightGetH;
  Ttp->HeightGetNH = TTP_SystemHeightGetNH;
  Ttp->HeightIsHit = TTP_SystemHeightIsHit;

  if ((Bulk = malloc(sizeof(ttpSTORE))) != NULL)
  {
    Bulk->MaxNumOfTriangles = 10;
    if ((Bulk->Triangles = malloc(sizeof(ttpTRIANGLE) * Bulk->MaxNumOfTriangles)) != NULL)
      Bulk->NumOfTriangles = 0;
  }
} /* End of 'TTP_SystemHeightInit' function */

/* END OF 'ttp_height.c' FILE */