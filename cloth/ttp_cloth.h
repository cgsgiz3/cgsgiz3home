/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_cloth.h
 * PURPOSE     : Tough Third Planet project.
 *               Cloth simulation.
 * PROGRAMMER  : CGSG'2022.
 *               Sofrygin Luka (LS4),
 * LAST UPDATE : 17.05.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __ttp_cloth_h_
#define __ttp_cloth_h_

#include "ttp_def.h"

/* Amount of steps to try to satisfy all constraints */ 
#define TTP_CLOTH_NUM_OF_ITERATIONS 3

/* Cloth multipoint strict constraint representation type */
typedef struct 
{
  INT ParticleA, ParticleB; /* Indexes of constraints in particles array */
  FLOAT RestLength;         /* Distance between particles in stable state */
  FLT Damper;                 /* Max allowed distance volatility */
  VEC3 Stretch;                /* Amount by what is constraint stretched */
} ttpCONSTRAINT;

typedef struct tagttpCLOTH ttpCLOTH; 
/* Cloth structure representation type */
struct tagttpCLOTH
{
  INT W;                                        /* Cloth width in particles */
  INT H;                                        /* Cloth height */
  FLT Weight;                                   /* Gravity force - same for all particles */
  FLT Friction;                                 /* Amount at which to reduce steps - like 0.999 */
  INT NumOfConstraints;                         /* Amount of constraints */
  FLT Stiffness;                                /* Amount by what to strech cloth (1 or 0.8 are good) */
  /* Customiseable collision handling function
   * ARGUMENTS:
   *   - Self-pointer to cloth:
   *       ttpCLOTH *Cloth;
   * RETURNS: None.
   */
  VOID (*HandleCollisions)( ttpCLOTH *Cloth );
  /* Customisable forces handling function
   * ARGUMENTS:
   *   - Self-pointer to cloth:
   *       ttpCLOTH *Cloth;
   * RETURNS: None.
   */
  VOID (*AccumulateForces)( ttpCLOTH *Cloth );
    /* Customisable hard constraints handling function
   * ARGUMENTS:
   *   - Self-pointer to cloth:
   *       ttpCLOTH *Cloth;
   * RETURNS: None.
   */
  VOID (*HandleHardConstraints)( ttpCLOTH *Cloth );
  VEC3 *P;                                      /* Array of particle positions */
  VEC3 *OldP;                                   /* Array of previous particles positions */
  VEC3 *Forces;                                 /* Array of force accumulations */
  VEC3 *Normals;                                /* Array of normals to the surface */
  ttpCONSTRAINT *Constraints;                   /* Array of (optional) constraints */
};

/* Cloth system interface data type */
typedef struct tagttpSYSTEM_CLOTH
{
  /* Cloth creation function.
   * ARGUMENTS:
   *   - Cloth object to initialize:
   *       ttpCLOTH *Cloth;
   *   - Height and width of cloth in particles:
   *       INT W, INT H;
   *   - Amount of constraints to be allocated:
   *        INT ConstrainCount;
   *   - Weight of cloth
   *        FLOAT Weight;
   * RETURNS: None.
   */
  VOID (*ClothCreate)( ttpCLOTH *Cloth, INT W, INT H, INT ConstrainCount, FLT Weight, FLT Stiffness, FLT Friction );

  /* Cloth freeing function.
   * ARGUMENTS:
   *   - Cloth object to free:
   *       ttpCLOTH *Cloth;
   * RETURNS: None.
   */
  VOID (*ClothFree)( ttpCLOTH *Cloth );

  /* Default flat cloth creation function.
   * ARGUMENTS:
   *   - Pointer to cloth object:
   *       ttpCLOTH *Cloth;
   *   - Cloth width:
   *       INT W;
   *   - Cloth height:
   *       INT H;
   *   - Cloth height above ground:
   *       FLT Height;
   *   - Stiffness level:
   *       INT MaxBr;
   *   - Cloth particle weight:
   *       FLT Weight;
   * RETURNS: None.
   */
  VOID (*ClothCreateDefault)( ttpCLOTH *Cloth, INT W, INT H, FLT Height, INT MaxBr );

  /* System unit inter frame events handle function.
   * ARGUMENTS:
   *   - Pointer to cloth object:
   *       ttpCLOTH *Cloth;
   *   - Speed (Amount of iterations)
   *       INT Speed;
   * RETURNS: None.
   */
  VOID (*ClothResponse)( ttpCLOTH *Cloth, INT Speed );

  /* System unit render function.
   * ARGUMENTS:
   *   - Pointer to a cloth to draw:
   *       ttpCLOTH *Cloth;
   * RETURNS: None.
   */
  VOID (*ClothDraw)( ttpCLOTH *Cloth );
} ttpSYSTEM_CLOTH;

/* Cloth hierarchy init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_ClothInit( VOID );

#endif /* __ttp_geom_h_ */

/* END OF 'ttp_cloth.h' FILE */
