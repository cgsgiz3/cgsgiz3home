/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_topo.h
 * PURPOSE     : Tough Third Planet project.
 *               Utilities topology default declaration file.
 * PROGRAMMER  : CGSG'2022.
 *               Artem Polishchuk (AP2).
 * LAST UPDATE : 10.04.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_topo_def_h_
#define __ttp_topo_def_h_

#include "ttp_def.h"

/* Create points function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - number of vertexes:
 *       INT NumOfV;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TTP_TopoCreatePoints( ttpTOPO *G, INT NumOfV );

/* Create grid function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TTP_TopoCreateGrid( ttpTOPO *G, INT W, INT H );

/* Create trimesh function.
 * ARGUMENTS:
 *   - topology data:
 *       ttpTOPO *G;
 *   - number of vertexes and indexes:
 *       INT NumOfV, NumOfI;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TTP_TopoCreateTrimesh( ttpTOPO *G, INT NumOfV, INT NumOfI );

#endif /* __ttp_topo_def_h_ */

/* END OF 'ttp_topo_def.h' FILE */