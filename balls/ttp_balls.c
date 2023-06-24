/***************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ttp_balls.c
 * PURPOSE     : Tough Third Planet project.
 *               ...
 * PROGRAMMER  : CGSG'2022.
 *               Mihail Zalesskiy (MZ2).
 * LAST UPDATE : 24.04.2023
 * NOTE        : Module prefix 'ttp'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "ttp.h"

/* Add point of ball function.
 * ARGUMENTS:
 *   - Position:
 *       VEC3 Pos;
 *   - Velocity:
 *       VEC3 Vel;
 *   - Speed:
 *       DBL Speed;
 *   - Radius:
 *       FLT Rad;
 *   - Mass:
 *       FLT Mass;
 * RETURNS:
 *   - Created ball:
 *       ttpBALL.
 */
ttpBALL TTP_BallCreate( VEC3 Pos, VEC3 Vel, DBL Speed, FLT Rad, FLT Mass )
{
  ttpBALL Ball;
  
  Ball.Pos = Pos;
  Ball.Vel = Vec3MulNum(Vec3Normalize(Vel), Speed);
  Ball.Speed = Speed;
  Ball.Rad = Rad;
  
  if (Mass < 0)
    Ball.Mass = 4.0 / 3 * PI * Rad * Rad * Rad * 0.01;
  else
    Ball.Mass = Mass;

  return Ball;
}/* End of 'TTP_BallCreate' function */


/* Free balls function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_BallFree( ttpBALL_CONTROLLER *Cntrl )
{
  INT i;

  for (i = 0; i < Cntrl->NumOfBalls; i++)
      free(Cntrl->HitArr[i]);
  free(Cntrl->HitArr);  
  free(Cntrl->PointsX);
  free(Cntrl->Arr);
}/* End of 'TTP_BallFree' function */

/* Add point of ball function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ttpBALL_CONTROLLER *Cntrl;
 *   - id in PointX array:
 *       INT i;
 *   - Left or right:
 *       CHAR Flag;
 *   - Position:
 *       FLT PosX;
 *   - Id in Array of balls:
 *       INT BallId;
 * RETURNS: None.
 */
VOID TTP_BallAddPoint(ttpBALL_CONTROLLER *Cntrl, INT i, CHAR Flag, FLT PosX, INT BallId )
{
  Cntrl->PointsX[i].PosX = PosX;
  Cntrl->PointsX[i].Flag = Flag;
  Cntrl->PointsX[i].BallId = BallId;
}/* End of 'TTP_BallAddPoint' function */

/* Swap variables function.
 * ARGUMENTS:
 *   - pointer to first element:
 *       ttpBALL_POINT *A;
 *   - pointer to second element:
 *       ttpBALL_POINT *B;
 * RETURNS: None.
 */
VOID TTP_BallSwapVar( ttpBALL_POINT *A, ttpBALL_POINT *B )
{
  ttpBALL_POINT tmp = *A;

  *A = *B;
  *B = tmp;
}/* End of 'SwapVar' function */

/* Selection sort function.
 * ARGUMENTS:
 *   - array to sort:
 *       ttpBALL_POINT *a;
 *   - count of elements in array:
 *       INT n;
 * RETURNS: None.
 */
VOID TTP_BallSelectionSort( ttpBALL_POINT *a, INT n )
{
  INT j, i, min;

  for (j = 0; j < n; j++)
  {
    for (i = j + 1, min = j; i < n; i++)
      if (a[i].PosX < a[min].PosX)
        min = i;
    TTP_BallSwapVar(&a[j], &a[min]);
  }
}/* End of 'TTP_BallSelectionSort' function */

/* Set HitArray function.
 * ARGUMENTS:
 *   - Pointer to balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_FillHitArray( ttpBALL_CONTROLLER *Cntrl )
{
  INT cnt, i, j, k, flag;


  for (i = 0; i < Cntrl->NumOfBalls * 2; i++)
  {
    if (Cntrl->PointsX[i].Flag == 0)
    {
      cnt = 0;
      for (j = i + 1; Cntrl->PointsX[j].BallId != Cntrl->PointsX[i].BallId; j++)
      {
        flag = 1;
        for (k = 0; Cntrl->HitArr[Cntrl->PointsX[i].BallId][k] != -1; k++)
          if (Cntrl->PointsX[j].BallId == Cntrl->HitArr[Cntrl->PointsX[i].BallId][k])
            flag = 0;

        if (flag == 1)
        {
          Cntrl->HitArr[Cntrl->PointsX[i].BallId][cnt] = Cntrl->PointsX[j].BallId;
          cnt++;
        }
      }
    }
  }
}/* End of 'TTP_FillHitArray' function */

/* Check collision function.
 * ARGUMENTS:
 *   - Pointer to balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_BallCheckCollision( ttpBALL_CONTROLLER *Cntrl )
{
  INT i, j;
  VEC3 PosA, PosB;

  for (i = 0; i < Cntrl->NumOfBalls; i++)
  {
    for (j = 0; Cntrl->HitArr[i][j] != -1 && j < Cntrl->NumOfBalls; j++)
    {
      if (Vec3Len(Vec3SubVec(Cntrl->Arr[i].Pos, Cntrl->Arr[Cntrl->HitArr[i][j]].Pos)) <= Cntrl->Arr[i].Rad + Cntrl->Arr[Cntrl->HitArr[i][j]].Rad)
      {

        PosA = Cntrl->Arr[i].Pos;
        PosB = Cntrl->Arr[Cntrl->HitArr[i][j]].Pos;

        Cntrl->Arr[Cntrl->HitArr[i][j]].Speed = (2 * Cntrl->Arr[i].Mass * Cntrl->Arr[i].Speed - Cntrl->Arr[Cntrl->HitArr[i][j]].Speed * (Cntrl->Arr[i].Mass - Cntrl->Arr[Cntrl->HitArr[i][j]].Mass)) / (Cntrl->Arr[i].Mass + Cntrl->Arr[Cntrl->HitArr[i][j]].Mass);

        if (Cntrl->Arr[Cntrl->HitArr[i][j]].Speed < Cntrl->MinSpeed)
          Cntrl->Arr[Cntrl->HitArr[i][j]].Speed = Cntrl->MinSpeed;
        if (Cntrl->Arr[Cntrl->HitArr[i][j]].Speed > Cntrl->MaxSpeed)
          Cntrl->Arr[Cntrl->HitArr[i][j]].Speed = Cntrl->MaxSpeed;

        Cntrl->Arr[i].Vel = Vec3MulNum(Vec3Normalize(Vec3AddVec(Vec3SubVec(PosA, PosB), Cntrl->Arr[i].Vel)), Cntrl->Arr[i].Speed);
        Cntrl->Arr[Cntrl->HitArr[i][j]].Vel = Vec3MulNum(Vec3Normalize(Vec3AddVec(Vec3SubVec(PosB, PosA), Cntrl->Arr[Cntrl->HitArr[i][j]].Vel)), Cntrl->Arr[Cntrl->HitArr[i][j]].Speed);
      }
    }
  }
}/* End of 'TTP_BallCheckCollision' function */

/* Balls prepare function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 * RETURNS: None.
 */
VOID TTP_BallPrepare( ttpBALL_CONTROLLER *Cntrl )
{
  INT i, k, j;

  for (i = 0; i < Cntrl->NumOfBalls; i++)
   for (j = 0; j < Cntrl->NumOfBalls; j++)
     Cntrl->HitArr[i][j] = -1;

  if (Cntrl->PointsX != NULL)
    memset(Cntrl->PointsX, 0, sizeof(Cntrl->PointsX));

  for (i = 0, k = 0; i < Cntrl->NumOfBalls; i++, k += 2)
  {
    TTP_BallAddPoint(Cntrl, k, 0, Cntrl->Arr[i].Pos.X - Cntrl->Arr[i].Rad, i);
    TTP_BallAddPoint(Cntrl, k + 1, 1, Cntrl->Arr[i].Pos.X + Cntrl->Arr[i].Rad, i);
  
    if (Cntrl->Arr[i].Pos.Y <= Cntrl->Plane.Pos.Y + Cntrl->Plane.Size.Y / 2 + Cntrl->Arr[i].Rad)
      Cntrl->Arr[i].Vel.Y = -Cntrl->Arr[i].Vel.Y;
    if (Cntrl->Arr[i].Pos.X <= Cntrl->Plane.Pos.X - Cntrl->Plane.Size.X / 2 + Cntrl->Arr[i].Rad || Cntrl->Arr[i].Pos.X >= Cntrl->Plane.Pos.X + Cntrl->Plane.Size.X / 2 - Cntrl->Arr[i].Rad)
       Cntrl->Arr[i].Vel.X = -Cntrl->Arr[i].Vel.X;
    if (Cntrl->Arr[i].Pos.Z <= Cntrl->Plane.Pos.Z - Cntrl->Plane.Size.Z / 2 + Cntrl->Arr[i].Rad || Cntrl->Arr[i].Pos.Z >= Cntrl->Plane.Pos.Z + Cntrl->Plane.Size.Z / 2 - Cntrl->Arr[i].Rad)
      Cntrl->Arr[i].Vel.Z = -Cntrl->Arr[i].Vel.Z;

    if (Cntrl->Arr[i].Pos.Y < Cntrl->Plane.Pos.Y + Cntrl->Plane.Size.Y / 2 + Cntrl->Arr[i].Rad - Cntrl->Arr[i].Rad / 4)
      Cntrl->Arr[i].Pos.Y += Cntrl->Arr[i].Rad + 10;
    if (Cntrl->Arr[i].Pos.X < Cntrl->Plane.Pos.X - Cntrl->Plane.Size.X / 2 + Cntrl->Arr[i].Rad - Cntrl->Arr[i].Rad / 4)
      Cntrl->Arr[i].Pos.X += Cntrl->Arr[i].Rad + 10;
    if (Cntrl->Arr[i].Pos.X > Cntrl->Plane.Pos.X + Cntrl->Plane.Size.X / 2 - Cntrl->Arr[i].Rad + Cntrl->Arr[i].Rad / 4)
      Cntrl->Arr[i].Pos.X -= Cntrl->Arr[i].Rad + 10;
    if (Cntrl->Arr[i].Pos.Z < Cntrl->Plane.Pos.Z - Cntrl->Plane.Size.Z / 2 + Cntrl->Arr[i].Rad - Cntrl->Arr[i].Rad / 4)
      Cntrl->Arr[i].Pos.Z += Cntrl->Arr[i].Rad + 10;
    if (Cntrl->Arr[i].Pos.Z > Cntrl->Plane.Pos.Z + Cntrl->Plane.Size.Z / 2 - Cntrl->Arr[i].Rad + Cntrl->Arr[i].Rad / 4)
      Cntrl->Arr[i].Pos.Z -= Cntrl->Arr[i].Rad + 10;

    Cntrl->Arr[i].Pos = Vec3AddVec(Vec3AddVec(Cntrl->Arr[i].Pos, Vec3MulNum(Cntrl->Arr[i].Vel, Ttp->DeltaTime)), Vec3MulNum(Cntrl->Grav, Ttp->DeltaTime * Ttp->DeltaTime / 2));
    Cntrl->Arr[i].Vel = Vec3AddVec(Cntrl->Arr[i].Vel, Vec3MulNum(Cntrl->Grav, Ttp->DeltaTime * 5));

  }

  TTP_BallSelectionSort(Cntrl->PointsX, Cntrl->NumOfBalls * 2);
}/* End of 'TTP_BallPrepare' function */

/* Add a ball function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 *   - Position:
 *       VEC3 Pos;
 *   - Velosity:
 *       VEC3 Vel;
 *   - Speed:
 *       DBL Speed;
 *   - Radius:
 *       FLT Rad;
 *   - Mass:
 *       FLT Mass;
 * RETURNS: None.
 */
VOID TTP_BallAdd( ttpBALL_CONTROLLER *Cntrl, VEC3 Pos, VEC3 Vel, DBL Speed, FLT Rad, FLT Mass )
{
  INT i;

  if (Cntrl->NumOfBalls % Cntrl->BlockOfBalls == 0)
  {
    ttpBALL *NewArr = malloc(sizeof(ttpBALL) * (Cntrl->NumOfBalls + Cntrl->BlockOfBalls));
    
    if (Cntrl->Arr != NULL)
      memcpy(NewArr, Cntrl->Arr, sizeof(ttpBALL) * Cntrl->NumOfBalls);

    free(Cntrl->Arr);
    Cntrl->Arr = NewArr;
  }

  Cntrl->Arr[Cntrl->NumOfBalls] = TTP_BallCreate(Pos, Vel, Speed, Rad, Mass);

  for (i = 0; i < Cntrl->NumOfBalls; i++)
    free(Cntrl->HitArr[i]);
  free(Cntrl->HitArr);

  Cntrl->NumOfBalls++;

  free(Cntrl->PointsX);
  Cntrl->PointsX = malloc(sizeof(ttpBALL_POINT)  * Cntrl->NumOfBalls * 2);

  Cntrl->HitArr = malloc(sizeof(INT *) * Cntrl->NumOfBalls);
  for (i = 0; i < Cntrl->NumOfBalls; i++)
    Cntrl->HitArr[i] = malloc(sizeof(INT) * Cntrl->NumOfBalls);

}/* End of 'TTP_BallAdd' function */

/* Init ball system function.
 * ARGUMENTS:
 *   - Pointer to Balls controller:
 *       ttpBALL_CONTROLLER *Cntrl;
 *   - Count of balls in one block:
 *       INT BlockOfBalls;
 *   - Position of plane:
 *       VEC3 Pos;
 *   - Size:
 *       VEC3 Size;
 *   - Max speed:
 *       DBL MaxSpeed;
 *   - Min speed:
 *       DBL MinSpeed;
 * RETURNS: None.
 */
VOID TTP_BallInit( ttpBALL_CONTROLLER *Cntrl, INT BlockOfBalls, VEC3 Pos, VEC3 Size, DBL MaxSpeed, DBL MinSpeed )
{
  Cntrl->Plane.Pos = Pos;
  Cntrl->Plane.Size = Size;

  Cntrl->Arr = NULL;
  Cntrl->HitArr = NULL;
  Cntrl->PointsX = NULL;
  Cntrl->NumOfBalls = 0;
  Cntrl->BlockOfBalls = BlockOfBalls;
  Cntrl->MaxSpeed = MaxSpeed;
  Cntrl->MinSpeed = MinSpeed;

  Cntrl->Grav = Vec3Set(0, -9.806, 0);
}/* End of 'TTP_BallInit' function */

/* System init ball system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TTP_BallsInit( VOID )
{
  Ttp->Balls.BallAdd = TTP_BallAdd;
  Ttp->Balls.BallAddPoint = TTP_BallAddPoint;
  Ttp->Balls.BallCheckCollision = TTP_BallCheckCollision;
  Ttp->Balls.BallCreate = TTP_BallCreate;
  Ttp->Balls.BallFree = TTP_BallFree;
  Ttp->Balls.BallInit = TTP_BallInit;
  Ttp->Balls.BallPrepare = TTP_BallPrepare;
  Ttp->Balls.FillHitArray = TTP_FillHitArray;
}/* End of 'TTP_SystemBallsInit' function */

/* END OF 'ttp_balls.c' FILE */
