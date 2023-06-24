/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_topo_points.c
 * PURPOSE     : Tough Third Planet project.
 *               Points sample.
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

/* Create points function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - number of vertexes:
 *       INT NumOfV;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TTP_TopoCreatePoints( ttpTOPO *G, INT NumOfV )
{
  memset(G, 0, sizeof(ttpTOPO));
  if ((G->V = malloc(sizeof(ttpVERTEX) * NumOfV)) == NULL)
    return FALSE;

  G->Type = TTP_TOPO_TYPE_POINTS;
  G->NumOfV = NumOfV;

  memset(G->V, 0, sizeof(ttpVERTEX) * NumOfV);
  
  return TRUE;
} /* End of 'TTP_TopoCreatePoints' function */

/* END OF 'ttp_topo_points.c' FILE */
