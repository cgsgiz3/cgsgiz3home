/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_rigid_body_def.h
 * PURPOSE     : Tough Third Planet project.
 *               Rigid body simulation declaration file.
 *               Def rigit body module.
 * PROGRAMMER  : CGSG'2022.
 *               Timur Minazhetdinov (TM2).
 * LAST UPDATE : 19.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_rigid_body_def_h_
#define __ttp_rigid_body_def_h_

#include "ttp_def.h"

/* Create rigid cube function.
 * ARGUMENTS:
 *   - rigid body total mass:
 *       DBL M;
 *   - rigid body position:
 *       VEC Pos;
 *   - rigid cube sizes:
 *       VEC3 Size;
 * RETURNS:
 *   (ttpRIGID_BODY *) pointer to filled with parameters cube.
 */
ttpRIGID_BODY * TTP_CreateRigidCube( DBL M, VEC3 Pos, VEC3 Size );

#endif /* __ttp_rigid_body_def_h_ */

/* END OF 'ttp_rigid_body_def.h' FILE */
