/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_geom.h
 * PURPOSE     : Tough Third Planet project.
 *               Geometry hierarchical modeling.
 * PROGRAMMER  : CGSG'2022.
 *               Egorov Timofey (TE1),
 *               Aleksey Zheleznyakov (AZ3).
 * LAST UPDATE : 17.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_geom_h_
#define __ttp_geom_h_

#include "ttp_def.h"

#define TTP_MAX_NODES 30  /* Maximum amount of Depended Geom Nodes */
#define TTP_MAX_STR 100   /* Maximum len of string */

/* Hierarhical geometry node representation type */
typedef struct tagttpGEOM_NODE ttpGEOM_NODE;
struct tagttpGEOM_NODE
{
  INT Id;                           /* Node Id */
  BOOL IsOpen;                      /* IsNode Open Flag */
  ttpGEOM_NODE *Parent;             /* Parent node */
  ttpPRIM **Prims;                  /* Prims array */
  ttpGEOM_NODE *SubNodes;           /* Deppended node sarray  */
  INT NumOfPrims, MaxNumOfPrims;    /* Num of depended object, maximum depended object */
  INT NumOfNodes, MaxNumOfNodes;    /* Num of depended nodes, maximum depended nodes */
  BOOL IsNeedRecalc;                /* Recalculate matrix flag */
  BOOL IsNeedMemory;                /* Memory flag */
  BOOL IsNeedTrans;
  MATR
    LocalTrans,  /* Local tranformation matrix */
    ParentTrans, /* Tranformation matrix from parent node */
    GlobalTrans; /* Accumalated global transformation matrix */
  INT ActiveNumber;                 /* Open nodes number */
};

/* Hierarhical geometry root representation type */
typedef struct tagttpGEOM
{
  ttpGEOM_NODE Root;       /* Root Node */
  ttpGEOM_NODE *Current;   /* Current node */
} ttpGEOM;

/* G3DM model file vertex representation type */
typedef struct tagttpG3DM_VERTEX
{
  VEC3 P;  /* position */
  VEC2 T;  /* texture cordinate */
  VEC3 N;  /* normal */
  VEC4 C;  /* color */
} ttpG3DM_VERTEX;

/* G3DM model file material representation type */
typedef struct tagttpG3DM_MATERIAL
{
  CHAR Name[300]; /* Material name */
  /* Illumination coefficients */
  VEC3 Ka, Kd, Ks;    /* Ambient, diffuse, specular coefficients */
  FLT Ph;             /* Phong power coefficient – shininess */
  FLT Trans;          /* Transparency factor */
  DWORD Tex[8];       /* Texture references 
                       * (8 time: texture number in G3DM file, -1 if no texture) */
  /* Shader information */
  CHAR ShaderString[300]; /* Additional shader information */
  DWORD Shader;       /* Shader number (uses after load into memory) */
} ttpG3DM_MATERIAL;

/* Geometry system interface data type */
typedef struct tagttpSYSTEM_GEOM
{
  /* Setup geometry structure (ttpGEOM *G) function.
   * ARGUMENTS:
   *   - pointer to geom structure:
   *       ttpGEOM *G;
   * RETURNS: None.
   */
  VOID (*GeomSet)( ttpGEOM *G );

  /* Setup geometry structure (ttpGEOM *G) function.
   * ARGUMENTS:
   *   - pointer to geom structure:
   *       ttpGEOM *G;
   * RETURNS: None.
   */
  VOID (*GeomSetActive)( ttpGEOM *G );

  /* Load array of primitives from .G3DM file to current geometry open node function.
   * ARGUMENTS:
   *   - material pattern:
   *       ttpMATERIAL_PATTERN *MtlPat;
   *   - file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*GeomAddG3DM)( ttpMATERIAL_PATTERN *MtlPat, CHAR *FileName );

  /* Add primitive to current geometry open node function.
   * ARGUMENTS:
   *   - primitive:
   *       ttpPRIM *Pr;
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*GeomAddPrim)( ttpPRIM *Pr );

  /* Geom open function.
   * ARGUMENTS:
   *   - open node Id:
   *       INT Id; 
   * RETURNS:
   *   (BOOL) succes of operation.
   */ 
  BOOL (*GeomOpenNode)( INT Id );

  /* Geom open function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (BOOL) succes of operation.
   */ 
  BOOL (*GeomCloseNode)( VOID );

  /* Geom draw function.
   * ARGUMENTS:
   *   - pointer to geom structure:
   *       ttpGEOM *G;
   * RETURNS: None.
   */
  VOID (*GeomDraw)( ttpGEOM *G );

  /* Trans primitive to another level function function.
   * ARGUMENTS:
   *   - num of primitive:
   *       INT NumOfPrim;
   *   - id of node in array of subnodes:
   *       INT NumOfNode;
   * RETURNS: None.
   */
  VOID (*GeomLevel)( INT NumOfPrim, INT NumOfNode );

  /* Set matrix for current node function.
   * ARGUMENTS:
   *   - main matrix:
   *       MATR M;
   * RETURNS: None.
   */
  VOID (*GeomSetMulMatrix)( MATR M );
  
  /* Set matrix for current node function.
   * ARGUMENTS:
   *   - main matrix:
   *       MATR M;
   * RETURNS: None.
   */
  VOID (*GeomSetMatrix)( MATR M );

  /* Change current function.
   * ARGUMENTS:
   *   - what current change:
   *       INT Num;
   *   - id of subnode:
   *       INT Id;
   * RETURNS: None.
   */
  VOID (*GeomChangeCurrent)( INT Num, INT Id );

  /* Change need memory flag function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*GeomChangeMemory)( VOID );

  /* Recalculate BB function.
   * ARGUMENTS:
   *   - number of primitive:
   *       INT NumberOfPrim;
   * RETURNS: None.
   */
  VEC3 (*GeomBB)( INT NumberOfPrim );

  /* Recalculate BB of primitives array function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VEC3 (*GeomArrayBB)( VOID );

  /* Set multiplication matrix for current node function.
   * ARGUMENTS:
   *   - main matrix:
   *       MATR M;
   * RETURNS: None.
   */
  VOID (*GeomPrimSetMatrix)( INT NumberofPrim, MATR M );

} ttpSYSTEM_GEOM;

/* Geometry hierarchy init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_GeomInit( VOID );

/* Geometry hierarchy close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_GeomClose( VOID );

#endif /* __ttp_geom_h_ */

/* END OF 'ttp_geom.h' FILE */
