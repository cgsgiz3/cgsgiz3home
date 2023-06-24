/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_topo_trimesh.c
 * PURPOSE     : Tough Third Planet project.
 *               Trimesh sample.
 * PROGRAMMER  : CGSG'2022.
 *               Artem Polishchuk (AP2).
 * LAST UPDATE : 22.04.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"
#include "ttp_topo_def.h"

/* Create trimesh function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - number of vertexes and indexes:
 *       INT NumOfV, NumOfI;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TTP_TopoCreateTrimesh( ttpTOPO *G, INT NumOfV, INT NumOfI )
{
  memset(G, 0, sizeof(ttpTOPO));
  if ((G->V = malloc(sizeof(ttpVERTEX) * NumOfV)) == NULL)
    return FALSE;

  if ((G->I = malloc(sizeof(ttpVERTEX) * NumOfI)) == NULL)
    return FALSE;

  G->Type = TTP_TOPO_TYPE_TRIMESH;
  G->NumOfV = NumOfV;
  G->NumOfI = NumOfI;

  memset(G->V, 0, sizeof(ttpVERTEX) * NumOfV);
  memset(G->I, 0, sizeof(ttpVERTEX) * NumOfI);

  return TRUE;
} /* End of 'TTP_TopoCreateTrimesh' function */

/* END OF 'ttp_topo_trimesh.c' FILE */
