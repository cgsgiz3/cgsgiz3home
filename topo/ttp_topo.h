/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_topo.h
 * PURPOSE     : Tough Third Planet project.
 *               Utilities topology declaration file.
 * PROGRAMMER  : CGSG'2022.
 *               Artem Polishchuk (AP2).
 * LAST UPDATE : 28.04.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_topo_h_
#define __ttp_topo_h_

#include "ttp_def.h"

/* Structure of vertexes */
typedef struct tagttpVERTEX
{
  VEC3 P;           /* Vertex position */
  VEC2 T;           /* Vertex texture coordinate */
  VEC3 N;           /* Vertex normal */
  VEC4 C;           /* Vertex color */
  VEC3 Tan;         /* Vertex tangent vector */
  VEC3 Bitan;       /* Vertex bitangent vector */
} ttpVERTEX;

/* Structure of topology types */
typedef enum tagttpTOPO_TYPE
{
  TTP_TOPO_TYPE_POINTS,        /* Array of points */
  TTP_TOPO_TYPE_GRID,          /* Array of grids */
  TTP_TOPO_TYPE_TRIMESH,       /* Triangle mesh - array of triangles */
} ttpTOPO_TYPE;

/* Structure of topology description */
typedef struct tagttpTOPO ttpTOPO;
struct tagttpTOPO
{ 
  ttpTOPO_TYPE Type;           /* Topology type */
  ttpVERTEX *V;                /* Vertex array */
  INT *I;                      /* Index array */
  INT NumOfV;                  /* Vertex array size */
  INT NumOfI;                  /* Index array size */
  INT W, H;                    /* Grid size (in vertices) */
  VEC3 MinBB;                  /* Minimum overall coordinates */
  VEC3 MaxBB;                  /* Maximum overall coordinates */
};

/* Topology system */
typedef struct tagttpSYSTEM_TOPO
{
  /* Create points function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *G;
   *   - number of vertexes:
   *       INT NumOfV;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*TopoCreatePoints)( ttpTOPO *G, INT NumOfV ); 

  /* Create grid function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *G;
   *   - number of vertexes and indexes:
   *       INT NumOfV, NumOfI;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*TopoCreateGrid)( ttpTOPO *G, INT W, INT H );

  /* Create trimesh function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *G;
   *   - number of vertexes and indexes:
   *       INT NumOfV, NumOfI;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*TopoCreateTrimesh)( ttpTOPO *G, INT NumOfV, INT NumOfI );

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
  ttpPRIM * (*TopoGridToPrim)( ttpTOPO *T, ttpMATERIAL *Mtl );

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
  ttpPRIM * (*TopoPointsToPrim)( ttpTOPO *T, ttpMATERIAL *Mtl );

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
  ttpPRIM * (*TopoTrimeshToPrim)( ttpTOPO *T, ttpMATERIAL *Mtl );

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
  ttpPRIM * (*TopoToPrim)( ttpTOPO *Topo, ttpMATERIAL *Mtl );

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
  BOOL (*TopoCreateSphere)( ttpTOPO *G, FLT R, INT W, INT H );

  /* Create torus topology function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *G;
   *   - outside radius:
   *       FLT R;
   *   - inside radius:
   *       FLT r;
   *   - grid size:
   *       INT W, H;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*TopoCreateTorus)( ttpTOPO *G, FLT R, FLT r, INT W, INT H );

  /* Create cube topology function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *G;
   *   - cube center point:
   *       VEC3 C;
   *   - length side of cubeXYZ:
   *       VEC3 L;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*TopoCreateCube)( ttpTOPO *T, VEC3 C, VEC3 L );

  /* Evaluate triangle tangent space function.
   * ARGUMENTS:
   *   - triangle vertices:
   *       ttpVERTEX *P0, *P1, *P2;
   * RETURNS: None.
   */
  VOID (*TopoTriangleTangentSpace)( ttpVERTEX *P0, ttpVERTEX *P1, ttpVERTEX *P2 );

  /* Make orhogonal basis TBN for vertex function.
   * ARGUMENTS:
   *   - vertex:
   *       ttpVERTEX *V;
   * RETURNS: None.
   */
  VOID (*TopoVertexTBNOrtho)( ttpVERTEX *V );

  /* Evaluation of bound box function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *T;
   * RETURNS: None.
   */
  VOID (*TopoEvalBB)( ttpTOPO *T );

  /* Evaluate normals function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *T;
   * RETURNS: None.
   */
  VOID (*TopoEvalNormals)( ttpTOPO *T );

  /* Evaluate tangent space function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *T;
   * RETURNS: None.
   */
  VOID (*TopoEvalTangentSpace)( ttpTOPO *T );

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
  BOOL (*TopoCreate)( ttpTOPO_TYPE Type, ttpTOPO *T, INT W, INT H, INT NumOfV, INT NumOfI );

  /* Free topology function.
   * ARGUMENTS:
   *   - topology data:
   *       ttpTOPO *T;
   * RETURNS: None.
   */
  VOID (*TopoFree)( ttpTOPO *T );
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
  ttpPRIM * (*TopoPrim)( ttpTOPO *T, ttpMATERIAL *Mtl, INT NumOfVertexes, INT *Ind, INT NumOfI );

} ttpSYSTEM_TOPO;

/* Init topology function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_TopoInit( VOID );

ttpPRIM * TE1_TTP_TopoPrim( ttpTOPO *T, ttpMATERIAL *Mtl, INT NumOfVertexes, INT *Ind, INT NumOfI );

#endif /* __ttp_topo_h_ */

/* END OF 'ttp_topo.h' FILE */
