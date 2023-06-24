/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_topo_grid.c
 * PURPOSE     : Tough Third Planet project.
 *               Grid sample.
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

/* Create grid function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TTP_TopoCreateGrid( ttpTOPO *G, INT W, INT H )
{
  INT x, y, k;

  memset(G, 0, sizeof(ttpTOPO));
  if ((G->V = malloc(sizeof(ttpVERTEX) * W * H)) == NULL)
    return FALSE;   

  G->Type = TTP_TOPO_TYPE_GRID;
  G->H = H;
  G->W = W;

  memset(G->V, 0, sizeof(ttpVERTEX) * W * H);

  for (y = 0, k = 0; y < G->H; y++)
    for (x = 0; x < G->W; x++, k++)
    {
      G->V[k].N = Vec3Set(0, 1, 0);
      G->V[k].C = Vec4Set(0.6, 0.3, 0.2, 1);
      G->V[k].T = Vec2Set(x / (G->W - 1.0), y / (G->H - 1.0));
    }
    
  return TRUE;
} /* End of 'TTP_TopoCreateGrid' function */

/* END OF 'ttp_topo_grid.c' FILE */
