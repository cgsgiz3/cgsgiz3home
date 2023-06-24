/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_topo_prims.c
 * PURPOSE     : Tough Third Planet project.
 *               Primitives from grid, points, trimeshes sample.
 * PROGRAMMER  : CGSG'2022.
 *               Artem Polishchuk (AP2).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"
#include "ttp_topo_def.h"

/* Build vertex array by material pattern function.
 * ARGUMENTS:
 *   - source vertex array (in topology format):
 *       ttpVERTEX *V;
 *   - source vertex array size:
 *       INT NumOfV;
 *   - material pattern pointer:
 *       ttpMATERIAL_PATTERN *MtlPat;
 * RETURNS:
 *   (VOID *) built vertex array.
 */
static VOID * TTP_TopoVertexArrayBuild( ttpVERTEX *V, INT NumOfV, ttpMATERIAL_PATTERN *MtlPat )
{
  INT vert_sem, i, j;
  FLT *res;
  INT
    FromTo[30][3],   /* from - to - size in floats */
    NumOfFromTo = 0; /* from-to array size */


  if ((res = malloc(MtlPat->VertexAttrsSize * NumOfV)) == NULL)
    return NULL;
  memset(res, 0, MtlPat->VertexAttrsSize * NumOfV);

  for (vert_sem = 0; vert_sem < MtlPat->VertNumOfAttrs; vert_sem++)
  {
    if (MtlPat->VertexAttrs[vert_sem].Name[1] == 0)
    {
      switch (MtlPat->VertexAttrs[vert_sem].Name[0])
      {
      case 'P':
        if (MtlPat->VertexAttrs[vert_sem].Size > 3 || MtlPat->VertexAttrs[vert_sem].Type != 'f')
          continue;
        FromTo[NumOfFromTo][0] = (INT)(&((ttpVERTEX *)0)->P);
        break;
      case 'T':
        if (MtlPat->VertexAttrs[vert_sem].Size > 2 || MtlPat->VertexAttrs[vert_sem].Type != 'f')
          continue;
        FromTo[NumOfFromTo][0] = (INT)(&((ttpVERTEX *)0)->T);
        break;
      case 'N':
        if (MtlPat->VertexAttrs[vert_sem].Size > 3 || MtlPat->VertexAttrs[vert_sem].Type != 'f')
          continue;
        FromTo[NumOfFromTo][0] = (INT)(&((ttpVERTEX *)0)->N);
        break;
      case 'C':
        if (MtlPat->VertexAttrs[vert_sem].Size > 4 || MtlPat->VertexAttrs[vert_sem].Type != 'f')
          continue;
        FromTo[NumOfFromTo][0] = (INT)(&((ttpVERTEX *)0)->C);
        break;
      case 't':
        if (MtlPat->VertexAttrs[vert_sem].Size > 3 || MtlPat->VertexAttrs[vert_sem].Type != 'f')
          continue;
        FromTo[NumOfFromTo][0] = (INT)(&((ttpVERTEX *)0)->Tan);
        break;
      case 'b':
        if (MtlPat->VertexAttrs[vert_sem].Size > 3 || MtlPat->VertexAttrs[vert_sem].Type != 'f')
          continue;
        FromTo[NumOfFromTo][0] = (INT)(&((ttpVERTEX *)0)->Bitan);
        break;
      }
      FromTo[NumOfFromTo][1] = MtlPat->VertexAttrs[vert_sem].Offset;
      FromTo[NumOfFromTo][2] = MtlPat->VertexAttrs[vert_sem].Size;
      NumOfFromTo++;
      if (NumOfFromTo >= 30)
        break;
    }
  }

  for (i = 0; i < NumOfV; i++)
    for (j = 0; j < NumOfFromTo; j++)
      memcpy(&res[i * MtlPat->VertexAttrsSize / 4 + FromTo[j][1] / 4],
             &((FLT *)V)[i * sizeof(ttpVERTEX) / 4 + FromTo[j][0] / 4],
             FromTo[j][2] * 4);
  return res;
} /* End of 'TTP_TopoVertexArrayBuild' function */

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 *   - primitive to be create:
 *       ttpPRIM *Pr;
 *   - material of primitive:
 *       ttpMATERIAL *Mtl;
 * RETURNS: None.
 */
static ttpPRIM * TTP_TopoGridToPrim( ttpTOPO *T, ttpMATERIAL *Mtl )
{
  INT *Ind, i, j, k;
  ttpPRIM *Pr = NULL;
  VOID *V;

  Ind = malloc(sizeof(INT) * ((T->H - 1) * (T->W * 2 + 1) - 1));
  for (i = 0, k = 0; i < T->H - 1; i++)
  {
    for (j = 0; j < T->W; j++)
    {
      Ind[k++] = (i + 1) * T->W + j;
      Ind[k++] = i * T->W + j;
    }
    if (i != T->H - 2)
      Ind[k++] = -1;
  }
  V = TTP_TopoVertexArrayBuild(T->V, T->W * T->H, Mtl->MtlPat);
  Pr = Ttp->PrimCreate(Mtl, TTP_RND_PRIM_TRISTRIP, V, T->W * T->H, Ind, (T->H - 1) * (T->W * 2 + 1) - 1);
  Pr->MinBB = T->MinBB;
  Pr->MaxBB = T->MaxBB;

  free(V);
  free(Ind);

  return Pr;
} /* End of 'TTP_TopoGridToPrim' function */

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 *   - material of primitive:
 *       ttpMATERIAL *Mtl;
 *   - vertex numbers:
 *       INT NumOfVertexes;
 *   - indes array:
 *       INT *Ind;
 *   - index numbers:
 *       INT NumOfIndexes;
 * RETURNS: primitive pointer.
 */
ttpPRIM * TTP_TopoPrim( ttpTOPO *T, ttpMATERIAL *Mtl, INT NumOfVertexes, INT *Ind, INT NumOfI )
{
  ttpPRIM *Pr = NULL;
  VOID *V;

  V = TTP_TopoVertexArrayBuild(T->V, NumOfVertexes, Mtl->MtlPat);
  
  Pr = Ttp->PrimCreate(Mtl, TTP_RND_PRIM_TRIMESH, V, NumOfVertexes, Ind, NumOfI);
  Pr->MinBB = T->MinBB;
  Pr->MaxBB = T->MaxBB;

  free(V);
  return Pr;
}/* End of 'TE1_TTP_TopoPrim' */

/* Create primitive from points function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 *   - primitive to be create:
 *       ttpPRIM *Pr;
 *   - material of primitive:
 *       ttpMATERIAL *Mtl;
 * RETURNS: None.
 */
static ttpPRIM * TTP_TopoPointsToPrim( ttpTOPO *T, ttpMATERIAL *Mtl )
{
  ttpPRIM *Pr = NULL;
  VOID *V;

  V = TTP_TopoVertexArrayBuild(T->V, T->NumOfV, Mtl->MtlPat);
  Pr = Ttp->PrimCreate(Mtl, TTP_RND_PRIM_POINTS, V, T->NumOfV, NULL, 0);
  Pr->MinBB = T->MinBB;
  Pr->MaxBB = T->MaxBB;

  free(V);
  return Pr;
} /* End of 'TTP_TopoPointsToPrim' function */

/* Create primitive from trimeshes function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 *   - primitive to be create:
 *       ttpPRIM *Pr;
 *   - material of primitive:
 *       ttpMATERIAL *Mtl;
 * RETURNS: None.
 */
static ttpPRIM * TTP_TopoTrimeshToPrim( ttpTOPO *T, ttpMATERIAL *Mtl )
{
  ttpPRIM *Pr = NULL;
  VOID *V;

  V = TTP_TopoVertexArrayBuild(T->V, T->NumOfV, Mtl->MtlPat);
  Pr = Ttp->PrimCreate(Mtl, TTP_RND_PRIM_TRIMESH, V, T->NumOfV, T->I, T->NumOfI);
  Pr->MinBB = T->MinBB;
  Pr->MaxBB = T->MaxBB;

  free(V);
  return Pr;
} /* End of 'TTP_TopoTrimeshToPrim' function */

/* Create primitive from topology function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 *   - primitive to be create:
 *       ttpPRIM *Pr;
 *   - material of primitive:
 *       ttpMATERIAL *Mtl;
 * RETURNS: None.
 */
static ttpPRIM * TTP_TopoToPrim( ttpTOPO *T, ttpMATERIAL *Mtl )
{
  ttpPRIM *Pr = NULL;

  switch (T->Type)
  {
  case TTP_TOPO_TYPE_POINTS:
    Pr = TTP_TopoPointsToPrim(T, Mtl);
    break;
  case TTP_TOPO_TYPE_GRID:
    Pr = TTP_TopoGridToPrim(T, Mtl);
    break;
  case TTP_TOPO_TYPE_TRIMESH:
    Pr = TTP_TopoTrimeshToPrim(T, Mtl);
    break;
  }

  return Pr;
} /* End of 'TTP_TopoToPrim' function */

/* Create sphere topology function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - sphere radius:
 *       FLT R;
 *   - sphere size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TTP_TopoCreateSphere( ttpTOPO *T, FLT R, INT W, INT H )
{
  INT i, j;
  DBL theta, phi;

  if (!Ttp->TopoCreateGrid(T, W, H))
    return FALSE;

  for (i = 0, theta = PI; i < H; i++, theta -= PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
    {
      T->V[i * W + j].N = Vec3Set(sin(phi) * sin(theta),
                                  cos(theta),
                                  sin(theta) * cos(phi));
      T->V[i * W + j].P = Vec3MulNum(T->V[i * W + j].N, R);
      T->V[i * W + j].Tan = Vec3Normalize(Vec3Set(T->V[i * W + j].N.Z * (-1), T->V[i * W + j].N.Y, T->V[i * W + j].N.X));
      T->V[i * W + j].Bitan = Vec3Normalize(Vec3CrossVec(T->V[i * W + j].Tan, T->V[i * W + j].N));
    }
  return TRUE;
} /* End of 'TTP_TopoCreateSphere' function */

/* Create torus topology function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - outside radius:
 *       FLT R;
 *   - inside radius:
 *       FLT r;
 *   - torus size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TTP_TopoCreateTorus( ttpTOPO *T, FLT R, FLT r, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;
  
  if (!Ttp->TopoCreateGrid(T, W, H))
    return FALSE;
  
  for (i = 0, k = 0, theta = 0; i < H; i++, theta += 2 * PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
    {
      T->V[k].P = Vec3Set((R + r * cos(theta)) * sin(phi),
                               r * sin(theta),
                               (R + r * cos(theta)) * cos(phi));
      T->V[k].N = Vec3Set(cos(theta) * sin(phi),
                               sin(theta),
                               cos(theta) * cos(phi)); 
      T->V[i * W + j].Tan = Vec3Normalize(Vec3Set(T->V[i * W + j].N.Z, T->V[i * W + j].N.Y, T->V[i * W + j].N.X));
      T->V[i * W + j].Bitan = Vec3Normalize(Vec3CrossVec(T->V[i * W + j].Tan, T->V[i * W + j].N));

      k++;
    }
  
  return TRUE;
} /* End of 'TTP_TopoCreateTorus' function */

/* Create cube topology function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 *   - cube center point:
 *       VEC3 C;
 *   - length side of cubeXYZ:
 *       VEC3 L;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TTP_TopoCreateCube( ttpTOPO *T, VEC3 C, VEC3 L )
{ 
  INT i, j;
  
  if (!Ttp->TopoCreateGrid(T, L.X * 6, L.Y * 6))
    return FALSE;
  
  L.X /= 2;
  L.Y /= 2;
  L.Z /= 2;

  /*for (i = 0; i < C.X + L.X; i++)
    for (j = 0; j < C.Y + L.Y; j++)
      for (k = 0; k < C.Z + L.Z; k++)
  */
  /*
  Vert = 
  {
    Vec3Set(C.X - L.X, C.Y - L.Y, C.Z - L.Z),
    Vec3Set(C.X - L.X, C.Y - L.Y, C.Z + L.Z),
    Vec3Set(C.X - L.X, C.Y + L.Y, C.Z - L.Z),
    Vec3Set(C.X - L.X, C.Y + L.Y, C.Z + L.Z),
    Vec3Set(C.X + L.X, C.Y - L.Y, C.Z - L.Z),
    Vec3Set(C.X + L.X, C.Y - L.Y, C.Z + L.Z),
    Vec3Set(C.X + L.X, C.Y + L.Y, C.Z - L.Z),
    Vec3Set(C.X + L.X, C.Y + L.Y, C.Z + L.Z)
  };

  Ind = 
  {
    0, 1, 2, 2, 1, 3,
    4, 6, 5, 5, 6, 7, 
    0, 4, 1, 1, 4, 5, 
    2, 3, 6, 6, 3, 7, 
    0, 2, 4, 4, 2, 6,
    1, 5, 3, 3, 5, 7
  };
  
  for (i = 0; i < 36; i++)
    T->V[i].P = Vec3Set(Vert[Ind[i]]);
  
  */
  
  for (i = 0; i < L.Y * 2; i++)
    for (j = 0; j < L.X * 2; j++)
    {
      T->V[i * (INT)L.X * 2 + j].P = Vec3Set(C.X - L.X + j, C.Y - L.Y + i, C.Z - L.Z);
      T->V[i * (INT)L.X * 2 + j].N = Vec3Set(0, 1, 0);
    }

  for (i = 0; i < L.Y * 2; i++)
    for (j = 0; j < L.X * 2; j++)
    {
      T->V[(INT)L.X * (INT)L.Y * 4 + i * (INT)L.X * 2 + j].P = Vec3Set(C.X - L.X + j, C.Y - L.Y + i, C.Z + L.Z);
      T->V[(INT)L.X * (INT)L.Y * 4 + i * (INT)L.X * 2 + j].N = Vec3Set(0, 1, 0);
    }
  
  for (i = 0; i < L.X * 2; i++)
    for (j = 0; j < L.Z * 2; j++)
    {
      T->V[(INT)L.X * (INT)L.Y * 8 + i * (INT)L.Z * 2 + j].P = Vec3Set(C.X - L.X + i, C.Y - L.Y, C.Z - L.Z + j);
      T->V[(INT)L.X * (INT)L.Y * 8 + i * (INT)L.Z * 2 + j].N = Vec3Set(0, 1, 0);
    }
  
  for (i = 0; i < L.X * 2; i++)
    for (j = 0; j < L.Z * 2; j++)
    {
      T->V[(INT)L.X * (INT)L.Y * 8 + (INT)L.X * (INT)L.Z * 4 + i * (INT)L.Z * 2 + j].P = Vec3Set(C.X - L.X + i, C.Y + L.Y, C.Z - L.Z + j); 
      T->V[(INT)L.X * (INT)L.Y * 8 + (INT)L.X * (INT)L.Z * 4 + i * (INT)L.Z * 2 + j].N = Vec3Set(0, 1, 0);
    }
  
  for (i = 0; i < L.Z * 2; i++)
    for (j = 0; j < L.Y * 2; j++)
    {
      T->V[(INT)L.X * (INT)L.Y * 8 + (INT)L.X * (INT)L.Z * 8 + i * (INT)L.Y * 2 + j].P = Vec3Set(C.X - L.X, C.Y - L.Y + j, C.Z - L.Z + i);
      T->V[(INT)L.X * (INT)L.Y * 8 + (INT)L.X * (INT)L.Z * 8 + i * (INT)L.Y * 2 + j].N = Vec3Set(0, 1, 0);
    }

  for (i = 0; i < L.Z * 2; i++)
    for (j = 0; j < L.Y * 2; j++)
    {
      T->V[(INT)L.X * (INT)L.Y * 8 + (INT)L.X * (INT)L.Z * 8 + (INT)L.Y * (INT)L.Z * 4 + i * (INT)L.Y * 2 + j].P = Vec3Set(C.X + L.X, C.Y - L.Y + j, C.Z - L.Z + i);
      T->V[(INT)L.X * (INT)L.Y * 8 + (INT)L.X * (INT)L.Z * 8 + (INT)L.Y * (INT)L.Z * 4 + i * (INT)L.Y * 2 + j].N = Vec3Set(0, 1, 0);
    }
  
  return TRUE;
} /* End of 'TTP_TopoCreateCube' function */

/* Evaluate triangle tangent space function.
 * ARGUMENTS:
 *   - triangle vertices:
 *       ttpVERTEX *P0, *P1, *P2;
 * RETURNS: None.
 */
static VOID TTP_TopoTriangleTangentSpace( ttpVERTEX *P0, ttpVERTEX *P1, ttpVERTEX *P2 )
{
  FLT
    s1 = P1->T.X - P0->T.X,
    s2 = P2->T.X - P0->T.X,
    t1 = P1->T.Y - P0->T.Y,
    t2 = P2->T.Y - P0->T.Y,
    det = s1 * t2 - s2 * t1;
  if (det != 0)
  {
    VEC3
      Q1 = Vec3SubVec(P1->P, P0->P),
      Q2 = Vec3SubVec(P2->P, P0->P),
      T = Vec3DivNum(Vec3SubVec(Vec3MulNum(Q1, t2), Vec3MulNum(Q2, t1)), det),
      B = Vec3DivNum(Vec3SubVec(Vec3MulNum(Q2, s1), Vec3MulNum(Q1, s2)), det);

    T = Vec3Normalize(T);
    B = Vec3Normalize(B);

    P0->Tan = Vec3AddVec(P0->Tan, T);
    P0->Bitan = Vec3AddVec(P0->Bitan, B);
    P1->Tan = Vec3AddVec(P1->Tan, T);
    P1->Bitan = Vec3AddVec(P1->Bitan, B);
    P2->Tan = Vec3AddVec(P2->Tan, T);
    P2->Bitan = Vec3AddVec(P2->Bitan, B);
  }
} /* End of 'TTP_TopoTriangleTangentSpace' function */

/* Make orhogonal basis TBN for vertex function.
 * ARGUMENTS:
 *   - vertex:
 *       ttpVERTEX *V;
 * RETURNS: None.
 */
static VOID TTP_TopoVertexTBNOrtho( ttpVERTEX *V )
{
  /* Normalize directions */
  V->N = Vec3Normalize(V->N);
  V->Tan = Vec3Normalize(V->Tan);
  V->Bitan = Vec3Normalize(V->Bitan);

  /* Orthogonalize directions */
  V->Tan = Vec3Normalize(Vec3SubVec(V->Tan, 
                                    Vec3MulNum(V->N, Vec3DotVec(V->N, V->Tan))));
  V->Bitan =
    Vec3Normalize(Vec3SubVec(Vec3SubVec(V->Bitan,
                                        Vec3MulNum(V->N, Vec3DotVec(V->N, V->Bitan))),
                                        Vec3MulNum(V->Tan, Vec3DotVec(V->Tan, V->Bitan))));
} /* End of 'TTP_TopoVertexTBNOrtho' function */

/* Evaluation of bound box function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 * RETURNS: None.
 */
static VOID TTP_TopoEvalBB( ttpTOPO *T )
{
  INT i, Size = 0;

  if (T->V == NULL || T->NumOfV == 0)
    return;
  T->MinBB = T->MaxBB = T->V[0].P;

  if (T->Type == TTP_TOPO_TYPE_TRIMESH || T->Type == TTP_TOPO_TYPE_POINTS)
    Size = T->NumOfV;
  else if (T->Type == TTP_TOPO_TYPE_GRID)
    Size = T->W * T->H;

  for (i = 1; i < Size; i++)
  {
    if (T->MinBB.X > T->V[i].P.X)
      T->MinBB.X = T->V[i].P.X;
    if (T->MaxBB.X < T->V[i].P.X)
      T->MaxBB.X = T->V[i].P.X;

    if (T->MinBB.Y > T->V[i].P.Y)
      T->MinBB.Y = T->V[i].P.Y;
    if (T->MaxBB.Y < T->V[i].P.Y)
      T->MaxBB.Y = T->V[i].P.Y;
    
    if (T->MinBB.Z > T->V[i].P.Z)
      T->MinBB.Z = T->V[i].P.Z;
    if (T->MaxBB.Z < T->V[i].P.Z)
      T->MaxBB.Z = T->V[i].P.Z;
  }
} /* End of 'TTP_TopoEvalBB' function */

/* Evaluate normals function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 * RETURNS: None.
 */
static VOID TTP_TopoEvalNormals( ttpTOPO *T )
{
  INT i, j, Size = 0;

  if (T->Type == TTP_TOPO_TYPE_TRIMESH || T->Type == TTP_TOPO_TYPE_POINTS)
    Size = T->NumOfV;
  else if (T->Type == TTP_TOPO_TYPE_GRID)
    Size = T->W * T->H;

  for (i = 0; i < Size; i++)
    T->V[i].N = Vec3Set(0, 0, 0);

  if (T->Type == TTP_TOPO_TYPE_GRID)
  {
    for (i = 0; i < T->H - 1; i++)
      for (j = 0; j < T->W - 1; j++)
      {
        ttpVERTEX
          *P00 = T->V + i * T->W + j,
          *P01 = T->V + i * T->W + j + 1,
          *P10 = T->V + (i + 1) * T->W + j,
          *P11 = T->V + (i + 1) * T->W + j + 1;
        VEC3 N;

        N = Vec3Normalize(Vec3CrossVec(Vec3SubVec(P00->P, P10->P),
                                       Vec3SubVec(P11->P, P10->P)));
        P00->N = Vec3AddVec(P00->N, N);
        P10->N = Vec3AddVec(P10->N, N);
        P11->N = Vec3AddVec(P11->N, N);

        N = Vec3Normalize(Vec3CrossVec(Vec3SubVec(P11->P, P01->P),
                                       Vec3SubVec(P00->P, P01->P)));
        P00->N = Vec3AddVec(P00->N, N);
        P01->N = Vec3AddVec(P01->N, N);
        P11->N = Vec3AddVec(P11->N, N);
      }
  }

  else if (T->Type == TTP_TOPO_TYPE_TRIMESH)
  {
    for (i = 0; i < T->NumOfI; i += 3)
    {
      VEC3
        P0 = T->V[T->I[i]].P,
        P1 = T->V[T->I[i + 1]].P,
        P2 = T->V[T->I[i + 2]].P,
        N = Vec3Normalize(Vec3CrossVec(Vec3SubVec(P1, P0), Vec3SubVec(P2, P0)));

      T->V[T->I[i]].N = Vec3AddVec(T->V[T->I[i]].N, N);
      T->V[T->I[i + 1]].N = Vec3AddVec(T->V[T->I[i + 1]].N, N);
      T->V[T->I[i + 2]].N = Vec3AddVec(T->V[T->I[i + 2]].N, N);
    }
  }
  for (i = 0; i < Size; i++)
    T->V[i].N = Vec3Normalize(T->V[i].N);
} /* End of 'TTP_TopoEvalNormals' function */

/* Evaluate tangent space function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 * RETURNS: None.
 */
static VOID TTP_TopoEvalTangentSpace( ttpTOPO *T )
{
  INT i, j, Size = 0;

  if (T->Type == TTP_TOPO_TYPE_TRIMESH || T->Type == TTP_TOPO_TYPE_POINTS)
    Size = T->NumOfV;
  else if (T->Type == TTP_TOPO_TYPE_GRID)
    Size = T->W * T->H;

  for (i = 0; i < Size; i++)
  {
    T->V[i].Tan = Vec3Set(0, 0, 0);
    T->V[i].Bitan = Vec3Set(0, 0, 0);
  }

  if (T->Type == TTP_TOPO_TYPE_GRID)
  {
    for (i = 0; i < T->H - 1; i++)
      for (j = 0; j < T->W - 1; j++)
      {
        ttpVERTEX
          *P00 = T->V + i * T->W + j,
          *P01 = T->V + i * T->W + j + 1,
          *P10 = T->V + (i + 1) * T->W + j,
          *P11 = T->V + (i + 1) * T->W + j + 1;

      Ttp->TopoTriangleTangentSpace(P00, P10, P11);
      Ttp->TopoVertexTBNOrtho(P00);
      Ttp->TopoVertexTBNOrtho(P10);
      Ttp->TopoVertexTBNOrtho(P11);

      Ttp->TopoTriangleTangentSpace(P00, P01, P11);
      Ttp->TopoVertexTBNOrtho(P00);
      Ttp->TopoVertexTBNOrtho(P01);
      Ttp->TopoVertexTBNOrtho(P11);
      }
  }

  else if (T->Type == TTP_TOPO_TYPE_TRIMESH)
  {
    for (i = 0; i < T->NumOfI; i += 3)
    {
      ttpVERTEX
        *P0 = &T->V[T->I[i]],
        *P1 = &T->V[T->I[i + 1]],
        *P2 = &T->V[T->I[i + 2]];

      if (T->I == NULL)
      {
        P0 = &T->V[i],
        P1 = &T->V[i + 1],
        P2 = &T->V[i + 2];
      }

      Ttp->TopoTriangleTangentSpace(P0, P1, P2);
    }
    for (i = 0; i < T->NumOfV; i++)
      Ttp->TopoVertexTBNOrtho(&T->V[i]);
  }
} /* End of 'TTP_TopoEvalTangentSpace' function */

/* Create topology function.
 * ARGUMENTS:
 *   - topology type:
 *       ttpTOPO_TYPE Type;
 *   - topology data:
 *       ttpTOPO *T;
 *   - grid size:
 *       INT W, H;
 *   - Number of vertexes:
 *       INT NumOfV;
 *   - Number of indexes:
 *       INT NumOfI;
 * RETURNS: 
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TTP_TopoCreate( ttpTOPO_TYPE Type, ttpTOPO *T, INT W, INT H, INT NumOfV, INT NumOfI )
{
  switch (Type)
  {
  case TTP_TOPO_TYPE_POINTS:
    return Ttp->TopoCreatePoints(T, NumOfV);
    break;
  case TTP_TOPO_TYPE_GRID:
    return Ttp->TopoCreateGrid(T, W, H);
    break;
  case TTP_TOPO_TYPE_TRIMESH:
    return Ttp->TopoCreateTrimesh(T, NumOfV, NumOfI);
    break;
  default:
    return TRUE;
  }
} /* End of 'TTP_TopoCreate' function */

/* Free topology function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *T;
 * RETURNS: None.
 */
static VOID TTP_TopoFree( ttpTOPO *T )
{
  if (T->V != NULL)
    free(T->V);
  
  if (T->Type == TTP_TOPO_TYPE_TRIMESH)
    if (T->I != NULL)
      free(T->I);
} /* End of 'TTP_TopoFree' function */

/* Init topology function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_TopoInit( VOID )
{
  Ttp->TopoCreatePoints = TTP_TopoCreatePoints; 
  Ttp->TopoCreateGrid = TTP_TopoCreateGrid; 
  Ttp->TopoCreateTrimesh = TTP_TopoCreateTrimesh; 
  Ttp->TopoPointsToPrim = TTP_TopoPointsToPrim;
  Ttp->TopoGridToPrim = TTP_TopoGridToPrim;
  Ttp->TopoTrimeshToPrim = TTP_TopoTrimeshToPrim;
  Ttp->TopoToPrim = TTP_TopoToPrim;
  Ttp->TopoCreateSphere = TTP_TopoCreateSphere;
  Ttp->TopoCreateTorus = TTP_TopoCreateTorus;
  Ttp->TopoCreateCube = TTP_TopoCreateCube;
  Ttp->TopoTriangleTangentSpace = TTP_TopoTriangleTangentSpace;
  Ttp->TopoVertexTBNOrtho = TTP_TopoVertexTBNOrtho;
  Ttp->TopoEvalBB = TTP_TopoEvalBB;
  Ttp->TopoEvalNormals = TTP_TopoEvalNormals;
  Ttp->TopoEvalTangentSpace = TTP_TopoEvalTangentSpace;
  Ttp->TopoCreate = TTP_TopoCreate;
  Ttp->TopoFree = TTP_TopoFree;

  /* TE1 function for G3DM */
  Ttp->TopoPrim = TTP_TopoPrim;
} /* End of 'TTP_TopoInit' function */

/* END OF 'ttp_topo_prims.c' FILE */
