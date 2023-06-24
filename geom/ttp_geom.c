/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_geom.c
 * PURPOSE     : Tough Third Planet project.
 *               Hierarhial modeling.
 * PROGRAMMER  : CGSG'2022.
 *               Egorov Timofey (TE1),
 *               Aleksey Zheleznyakov (AZ3).
 * LAST UPDATE : 15.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"

/* Contains all geom information */
static ttpGEOM *TTP_GeomCurrentRoot;
#define TTP_GEOM_FORWARD_ALLOCATION 30  /* Number of nodes/primitives forward allocation */
/* Setup geometry structure (ttpGEOM *G) function.
 * ARGUMENTS:
 *   - pointer to geom structure:
 *       ttpGEOM *G;
 * RETURNS: None.
 */
static VOID TTP_GeomSet( ttpGEOM *G )
{
  if (G == NULL)
    return;
  memset(G, 0, sizeof(ttpGEOM));
  G->Root.Id = 0;
  G->Root.IsNeedRecalc = TRUE;
  G->Root.IsNeedMemory = TRUE;
  G->Root.Parent = NULL;
  G->Root.LocalTrans = MatrIdentity();
  G->Root.ParentTrans = MatrIdentity();
  G->Current = &G->Root;
} /* End of 'TTP_GeomSet' function */

/* Set active geometry function.
 * ARGUMENTS:
 *   - pointer to geom structure:
 *       ttpGEOM *G;
 * RETURNS: None.
 */
static VOID TTP_GeomSetActive( ttpGEOM *G )
{
  TTP_GeomCurrentRoot = G;
  if (TTP_GeomCurrentRoot != NULL)
    TTP_GeomCurrentRoot->Current = &TTP_GeomCurrentRoot->Root;
} /* End of 'TTP_GeomSetActive' function */

/* Trans primitive to another level function function.
 * ARGUMENTS:
 *   - num of primitive:
 *       INT NumOfPrim;
 *   - id of node in array of subnodes:
 *       INT NumOfNode;
 * RETURNS: None.
 */
static VOID TTP_GeomLevel( INT NumOfPrim, INT NumOfNode )
{
  ttpPRIM *Pr;
  ttpGEOM_NODE *LocalSubNodes = NULL;           /* Local nodes array */

  if (TTP_GeomCurrentRoot->Current->IsNeedMemory)
  {
    /*
    if (TTP_GeomCurrentRoot->Current->NumOfNodes != 0)
    {
      LocalSubNodes = malloc(sizeof(ttpGEOM_NODE) * TTP_GeomCurrentRoot->Current->NumOfNodes);
      memcpy(LocalSubNodes, TTP_GeomCurrentRoot->Current->SubNodes, sizeof(ttpGEOM_NODE) * TTP_GeomCurrentRoot->Current->NumOfNodes);
      TTP_GeomCurrentRoot->Current->SubNodes = malloc(sizeof(ttpGEOM_NODE) * TTP_GeomCurrentRoot->Current->NumOfNodes + 1);
      memcpy(TTP_GeomCurrentRoot->Current->SubNodes, LocalSubNodes, sizeof(ttpGEOM_NODE) * (TTP_GeomCurrentRoot->Current->NumOfNodes));
    }
    else
      TTP_GeomCurrentRoot->Current->SubNodes = malloc(sizeof(ttpGEOM_NODE));
    Ttp->GeomChangeMemory();
    */
    Ttp->GeomOpenNode(TTP_GeomCurrentRoot->Current->NumOfNodes);
  }

  Ttp->GeomChangeCurrent(1, NumOfNode);
  Pr = TTP_GeomCurrentRoot->Current->Prims[NumOfPrim];
  TTP_GeomCurrentRoot->Current->Prims[NumOfPrim] = NULL;
  TTP_GeomCurrentRoot->Current->IsNeedTrans = FALSE;
  Ttp->GeomChangeCurrent(0, NumOfNode);

  /* Add primitive */
  Ttp->GeomAddPrim(Pr);
  TTP_GeomCurrentRoot->Current->IsNeedTrans = TRUE;
} /* End of 'TTP_GeomLevel' function */

/* Get active one of nodes with number function.
 * ARGUMENTS:
   *   - open node Id:
   *       INT Id; 
 * RETURNS:
 *   (BOOL) succes of operation.
 */ 
static BOOL TTP_GeomOpenNode( INT Id )
{
  INT i;
  ttpGEOM_NODE *Node;

  if (TTP_GeomCurrentRoot == NULL)
    return FALSE;

  /* Look for existed node with specified Id */
  for (i = 0; i < TTP_GeomCurrentRoot->Current->NumOfNodes; i++)
    if (TTP_GeomCurrentRoot->Current->SubNodes[i].Id == Id)
    {
      TTP_GeomCurrentRoot->Current = &TTP_GeomCurrentRoot->Current->SubNodes[i];
      return TRUE;
    }

  /* Add a new node */
  if (TTP_GeomCurrentRoot->Current->NumOfNodes >= TTP_GeomCurrentRoot->Current->MaxNumOfNodes)
  {
    ttpGEOM_NODE *NewBlock;

    if ((NewBlock = malloc(sizeof(ttpGEOM_NODE) * (TTP_GeomCurrentRoot->Current->MaxNumOfNodes + TTP_GEOM_FORWARD_ALLOCATION))) == NULL)
      return FALSE;
    TTP_GeomCurrentRoot->Current->MaxNumOfNodes += TTP_GEOM_FORWARD_ALLOCATION;
    if (TTP_GeomCurrentRoot->Current->SubNodes != NULL)
    {
      memcpy(NewBlock, TTP_GeomCurrentRoot->Current->SubNodes, sizeof(ttpGEOM_NODE) * TTP_GeomCurrentRoot->Current->NumOfNodes);
      free(TTP_GeomCurrentRoot->Current->SubNodes);
    }
    TTP_GeomCurrentRoot->Current->SubNodes = NewBlock;
  }
  Node = &TTP_GeomCurrentRoot->Current->SubNodes[TTP_GeomCurrentRoot->Current->NumOfNodes++];
  memset(Node, 0, sizeof(ttpGEOM_NODE));
  Node->Id = Id;
  Node->IsNeedRecalc = TRUE;
  Node->Parent = TTP_GeomCurrentRoot->Current;
  Node->LocalTrans = MatrIdentity();
  Node->ParentTrans = MatrIdentity();
  Node->IsNeedMemory = TRUE;
  TTP_GeomCurrentRoot->Current = Node;
  return TRUE;
} /* End of 'TTP_GeomOpenNode' function */

/* Geom open function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) succes of operation.
 */ 
static BOOL TTP_GeomCloseNode( VOID )
{
  if (TTP_GeomCurrentRoot == NULL)
    return FALSE;
  if (TTP_GeomCurrentRoot->Current->Parent != NULL)
    TTP_GeomCurrentRoot->Current = TTP_GeomCurrentRoot->Current->Parent;
  return TRUE;
} /* End of 'TTP_GeomCloseNode' function */

/* Add primitive to current geometry open node function.
 * ARGUMENTS:
 *   - primitive:
 *       ttpPRIM *Pr;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TTP_GeomAddPrim( ttpPRIM *Pr )
{
  if (TTP_GeomCurrentRoot == NULL || Pr == NULL)
    return FALSE;

  /* Add a new prim */
  if (TTP_GeomCurrentRoot->Current->NumOfPrims >= TTP_GeomCurrentRoot->Current->MaxNumOfPrims)
  {
    ttpPRIM **NewBlock;

    if ((NewBlock = malloc(sizeof(ttpPRIM *) * (TTP_GeomCurrentRoot->Current->MaxNumOfPrims + TTP_GEOM_FORWARD_ALLOCATION))) == NULL)
      return FALSE;
    TTP_GeomCurrentRoot->Current->MaxNumOfPrims += TTP_GEOM_FORWARD_ALLOCATION;
    if (TTP_GeomCurrentRoot->Current->Prims != NULL)
    {
      memcpy(NewBlock, TTP_GeomCurrentRoot->Current->Prims, sizeof(ttpPRIM *) * TTP_GeomCurrentRoot->Current->NumOfPrims);
      free(TTP_GeomCurrentRoot->Current->Prims);
    }
    TTP_GeomCurrentRoot->Current->Prims = NewBlock;
  }
  TTP_GeomCurrentRoot->Current->Prims[TTP_GeomCurrentRoot->Current->NumOfPrims++] = Pr;
  return TRUE;
} /* End of 'TTP_GeomAddPrim' function */
 
/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - material pattern:
 *       ttpMATERIAL_PATTERN *MtlPat;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
static BOOL TTP_GeomAddG3DM( ttpMATERIAL_PATTERN *MtlPat, CHAR *FileName )
{ 
  FILE *F;
  INT flen, p, m, t, i, k = 0;
  BYTE
    *mem,     /* Copy of the file */
    *ptr;     /* Pointer to walk by the file */
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  ttpG3DM_MATERIAL *FileMtls; /* Load materials */
  ttpTOPO *FilePrims;         /* Load primitives (topology) */
  ttpTEXTURE **Texs;          /* Textures pointers array */
  ttpMATERIAL **Mtls;         /* Material pointers array */
  INT *FilePrimMtlNos;        /* Materials number for primitives */
  ttpPRIM *Pr;                /* Primitive pointer */
  INT check = 0;

  if (TTP_GeomCurrentRoot == NULL)
    return FALSE;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  /* Allocate memory and load whole file to memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(mem, 1, flen, F);
  fclose(F);

  ptr = mem;
  /* Signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;

  /* Prepare memory for load objects */
  FilePrims = malloc(sizeof(ttpMATERIAL *) * NumOfMaterials +
                     (sizeof(ttpTOPO) * sizeof(INT)) * NumOfPrims +
                     sizeof(ttpTEXTURE *) * NumOfTextures);
  if (FilePrims == NULL)
  {
    free(mem);
    return FALSE;
  }
  /* Pointer placing */
  Texs = (ttpTEXTURE **)(FilePrims + NumOfPrims);
  Mtls = (ttpMATERIAL **)(Texs + NumOfTextures);
  FilePrimMtlNos = (INT *)(Mtls + NumOfMaterials);

  /* Move matptr */
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;  /* num of facets * 3 */
    ttpG3DM_VERTEX *Vbuf;     /* without tan, bitan */
    INT *Ind;

    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    FilePrimMtlNos[p] = *(DWORD *)ptr;
    ptr += 4;

    Vbuf = (ttpG3DM_VERTEX *)ptr;
    ptr += sizeof(ttpG3DM_VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;

    if (Ttp->TopoCreateTrimesh(&FilePrims[p], NumOfVertexes, NumOfFacetIndexes))
    {
      for (i = 0; i < NumOfVertexes; i++)
      {
        FilePrims[p].V[i].Bitan = Vec3Set(0, 0, 0);
        FilePrims[p].V[i].Tan = Vec3Set(0, 0, 0);
        if (i == 24)
          FilePrims[p].V[i].C = Vec4Set(255, 0, 0, 1);
        else
          FilePrims[p].V[i].C = Vbuf[i].C;
        FilePrims[p].V[i].N = Vbuf[i].N;
        FilePrims[p].V[i].P = Vbuf[i].P;
        FilePrims[p].V[i].T = Vbuf[i].T;
      }
      memcpy(FilePrims[p].I, Ind, sizeof(INT) * NumOfFacetIndexes);
      /* Build tangent space */
      Ttp->TopoEvalBB(&FilePrims[p]);
      Ttp->TopoEvalTangentSpace(&FilePrims[p]);
    }
  }

  /* Load materials */
  FileMtls = (ttpG3DM_MATERIAL *)ptr;
  ptr += sizeof(ttpG3DM_MATERIAL) * NumOfMaterials;

  /* Load textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR *Name;
    DWORD W, H, C;

    Name = (CHAR *)ptr;
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    Texs[t] = Ttp->TexAddFmt(Name, W, H, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, TTP_TEXTURE_2D, ptr);
    ptr += W * H * C;
  }

  /* Create materials */
  for (m = 0; m < NumOfMaterials; m++)
  {
    CHAR Coeffs[TTP_MAX_NAME];
    ttpTEXTURE *MtlTex[8]; /* Texture references */

    sprintf(Coeffs, "Ka(%f, %f, %f);Kd(%f, %f, %f);Ks(%f, %f, %f);Ph(%f);Trans(%f)",
      FileMtls[m].Ka.X, FileMtls[m].Ka.Y, FileMtls[m].Ka.Z,
      FileMtls[m].Kd.X, FileMtls[m].Kd.Y, FileMtls[m].Kd.Z,
      FileMtls[m].Ks.X, FileMtls[m].Ks.Y, FileMtls[m].Ks.Z,
      FileMtls[m].Ph, FileMtls[m].Trans);
    for (t = 0; t < 8; t++)
      MtlTex[t] = FileMtls[m].Tex[t] == -1 ? NULL : Texs[FileMtls[m].Tex[t]];
    Mtls[m] = Ttp->MtlCreateTex(FileMtls[m].Name, MtlPat, Coeffs, MtlTex, 8, FileMtls[m].Trans != 1);
  }

  /* Load primitives */
  for (p = 0; p < NumOfPrims; p++)
  {
    Pr = Ttp->TopoToPrim(&FilePrims[p], Mtls[FilePrimMtlNos[p]]);
    TTP_GeomAddPrim(Pr);
    /// TTP_GeomCurrentRoot->Root->NumOfPrims++;
    /// /* TTP_GeomCurrentRoot->Root->Prims[p] = Pr; */
    /// TTP_GeomCurrentRoot->Root->SubNodes[TTP_GeomCurrentRoot->Root->ActiveNumber]->Prims[p] = Pr;
    /// TTP_GeomCurrentRoot->Root->LocalTrans = MatrIdentity();
    /// check++;
  }
  /*
    if (p == 0)
      Node->MinBB = Node->SubNodes[p]->MinBB, Node->MaxBB = Node->SubNodes[p]->MaxBB;
    else
    {
      if (Node->MinBB.X > Node->SubNodes[p]->MinBB.X)
        Node->MinBB.X = Node->SubNodes[p]->MinBB.X;
      if (Node->MaxBB.X < Node->SubNodes[p]->MaxBB.X)
        Node->MaxBB.X = Node->SubNodes[p]->MaxBB.X;

      if (Node->MinBB.Y > Node->SubNodes[p]->MinBB.Y)
        Node->MinBB.Y = Node->SubNodes[p]->MinBB.Y;
      if (Node->MaxBB.Y < Node->SubNodes[p]->MaxBB.Y)
        Node->MaxBB.Y = Node->SubNodes[p]->MaxBB.Y;

      if (Node->MinBB.Z > Node->SubNodes[p]->MinBB.Z)
        Node->MinBB.Z = Node->SubNodes[p]->MinBB.Z;
      if (Node->MaxBB.Z < Node->SubNodes[p]->MaxBB.Z)
        Node->MaxBB.Z = Node->SubNodes[p]->MaxBB.Z;
    }
  */
  free(FilePrims);
  free(mem);
  return TRUE;
} /* End of 'TTP_GeomG3DMLoad' function */

/* Draw node primitives function.
 * ARGUMENTS:
 *   - pointer to node to draw:
 *       ttpGEOM_NODE *Node;
 * RETURNS: None.
 */
static VOID TTP_GeomDrawNode( ttpGEOM_NODE *Node )
{
  INT i;

  /* Recalculate transform matrix */
  if (Node->IsNeedRecalc)
  {
    MATR m = MatrIdentity();

    Node->IsNeedRecalc = TRUE;
    for (i = 0; i < Node->NumOfNodes; i++)
      Node->SubNodes[i].IsNeedRecalc = TRUE;
    if (Node->Parent != NULL)
      m = Node->Parent->GlobalTrans;
    Node->GlobalTrans = MatrMulMatr3(Node->LocalTrans, Node->ParentTrans, m);
  }

  /* Draw primitives */
  for (i = 0; i < Node->NumOfPrims; i++)
    // if (i == 85)
    if (Node->IsNeedTrans)
      Ttp->PrimDraw(Node->Prims[i], MatrMulMatr(Node->Prims[i]->Trans, Node->GlobalTrans));
    else
      Ttp->PrimDraw(Node->Prims[i], Node->GlobalTrans);
  /* Draw subnodes */
  for (i = 0; i < Node->NumOfNodes; i++)
    TTP_GeomDrawNode(&Node->SubNodes[i]);
} /* End of  'TTP_GeomDraw' function */

/* Draw all depended primitives function.
 * ARGUMENTS:
 *   - pointer to geom structure:
 *       ttpGEOM *G;
 * RETURNS: None.
 */
static VOID TTP_GeomDraw( ttpGEOM *G )
{
  if (G != NULL)
    TTP_GeomDrawNode(&G->Root);
} /* End of  'TTP_GeomDraw' function */

/* Set matrix for current node function.
 * ARGUMENTS:
 *   - main matrix:
 *       MATR M;
 * RETURNS: None.
 */
static VOID TTP_GeomSetMatrix( MATR M )
{
  TTP_GeomCurrentRoot->Current->LocalTrans = M;
} /* End of 'TTP_GeomSetMatrix' function */

/* Set multiplication matrix for current node function.
 * ARGUMENTS:
 *   - main matrix:
 *       MATR M;
 * RETURNS: None.
 */
static VOID TTP_GeomSetMulMatrix( MATR M )
{
  TTP_GeomCurrentRoot->Current->LocalTrans = MatrMulMatr(M, TTP_GeomCurrentRoot->Current->LocalTrans);
} /* End of 'TTP_GeomSetMulMatrix' function */

/* Set multiplication matrix for current node function.
 * ARGUMENTS:
 *   - number of primitive ii primitives array:
 *       INT NumberofPrim;
 *   - main matrix:
 *       MATR M;
 * RETURNS: None.
 */
static VOID TTP_GeomPrimSetMatrix( INT NumberofPrim, MATR M )
{
  if (TTP_GeomCurrentRoot->Current->Prims[NumberofPrim] != NULL)
    TTP_GeomCurrentRoot->Current->Prims[NumberofPrim]->Trans =  M;
} /* End of 'TTP_GeomSetMulMatrix' function */

/* Recalculate BB function.
 * ARGUMENTS:
 *   - number of primitive:
 *       INT NumberOfPrim;
 * RETURNS: None.
 */
static VEC3 TTP_GeomBB( INT NumberOfPrim )
{
  ttpPRIM *Pr = {0};
  VEC3 Center = Vec3Set1(0);

  if (TTP_GeomCurrentRoot->Current->Prims[NumberOfPrim] == NULL)
    return Vec3Set1(0);
  if (NumberOfPrim >= 0 && NumberOfPrim < TTP_GeomCurrentRoot->Current->MaxNumOfPrims)
    Pr = TTP_GeomCurrentRoot->Current->Prims[NumberOfPrim];

  if (Pr != NULL)
    Center = Vec3Set((Pr->MaxBB.X + Pr->MinBB.X) / 2, (Pr->MaxBB.Y + Pr->MinBB.Y) / 2, (Pr->MaxBB.Z + Pr->MinBB.Z) / 2);
  return Center;
} /* End of 'TTP_GeomBB' function */

/* Recalculate BB of primitives array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VEC3 TTP_GeomArrayBB( VOID )
{
  INT i;
  ttpPRIM *Pr = {0};
  VEC3 Center = Vec3Set1(0);

  for (i = 0; i < TTP_GeomCurrentRoot->Current->NumOfPrims; i++)
    Center = Vec3AddVec(Center, Ttp->GeomBB(i));
  Center = Vec3MulNum(Center, 1 / TTP_GeomCurrentRoot->Current->NumOfPrims);

  return Center;
} /* End of 'TTP_GeomArrayBB' function */

/* Change current function.
 * ARGUMENTS:
 *   - what current change:
 *       INT Num;
 *   - id of subnode:
 *       INT Id;
 * RETURNS: None.
 */
static VOID TTP_GeomChangeCurrent( INT Num, INT Id )
{
  if (Num == 1 && TTP_GeomCurrentRoot->Current->Parent != NULL)
    TTP_GeomCurrentRoot->Current = TTP_GeomCurrentRoot->Current->Parent;
  else if (Num == 0)
    TTP_GeomCurrentRoot->Current = &TTP_GeomCurrentRoot->Current->SubNodes[Id];
} /* End of 'TTP_GeomChangeCurrent' function */

/* Change need memory flag function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TTP_GeomChangeMemory( VOID )
{
  TTP_GeomCurrentRoot->Current->IsNeedMemory = !TTP_GeomCurrentRoot->Current->IsNeedMemory;
} /* End of 'TTP_GeomChangeMemory' function */

/* Geom draw function.
 * ARGUMENTS:
 *   - main geom node:
 *       ttpGEOM_NODE *MainNode;
 * RETURNS: None.
 */
/*
static VOID TTP_GeomSetActive( ttpGEOM *G )
{
  G->Root->IsOpen = TRUE;
}
 *//* End of  'TTP_GeomDraw' function */

/* Geometry hierarchy init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_GeomInit( VOID )
{
  Ttp->GeomSet = TTP_GeomSet;
  Ttp->GeomSetActive = TTP_GeomSetActive;
  Ttp->GeomAddG3DM = TTP_GeomAddG3DM;
  Ttp->GeomAddPrim = TTP_GeomAddPrim;
  Ttp->GeomOpenNode = TTP_GeomOpenNode;
  Ttp->GeomCloseNode = TTP_GeomCloseNode;
  Ttp->GeomDraw = TTP_GeomDraw;
  Ttp->GeomLevel = TTP_GeomLevel;
  Ttp->GeomSetMatrix = TTP_GeomSetMatrix;
  Ttp->GeomSetMulMatrix = TTP_GeomSetMulMatrix;
  Ttp->GeomChangeCurrent = TTP_GeomChangeCurrent;
  Ttp->GeomChangeMemory = TTP_GeomChangeMemory;
  Ttp->GeomBB = TTP_GeomBB;
  Ttp->GeomArrayBB = TTP_GeomArrayBB;
  Ttp->GeomPrimSetMatrix = TTP_GeomPrimSetMatrix;
} /* End of 'TTP_GeomInit' function */

/* Geometry hierarchy close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_GeomClose( VOID )
{
  Ttp->GeomSet = NULL;
  Ttp->GeomSetActive = NULL;
  Ttp->GeomAddG3DM = NULL;
  Ttp->GeomAddPrim = NULL;
  Ttp->GeomOpenNode = NULL;
  Ttp->GeomCloseNode = NULL;
  Ttp->GeomDraw = NULL;
  Ttp->GeomLevel = NULL;
  Ttp->GeomSetMatrix = NULL;
  Ttp->GeomSetMulMatrix = NULL;
  Ttp->GeomChangeCurrent = NULL;
  Ttp->GeomChangeMemory = NULL;
  Ttp->GeomBB = NULL;
  Ttp->GeomArrayBB = NULL;
  Ttp->GeomPrimSetMatrix = NULL;
} /* End of 'TTP_GeomClose' function */

/* END OF 'ttp_geom.c' FILE */
