/* eslint-disable no-undef */
/* eslint-disable no-const-assign */
import {
  matrfrustum,
  matrview,
  vec3,
  pointtransform,
  mulmatr,
  mulmatr3,
  subvec,
  addvec,
  matrrotateX,
  matrrotateY,
  R2D,
} from "./math.js";
//import { Timer } from "./timer.js";

let Camera = new NewCamera();
export { Camera };
// Camera class constructor function
export function NewCamera() {
  this.responsemouse = (event = null) => {
    if (event === null) {
      this.mdx = 0;
      this.mdy = 0;
      return;
    }
    this.mdx = event.clientX - this.mx;
    this.mdy = event.clientY - this.my;
    this.mx = event.clientX;
    this.my = event.clientY;
  };
  this.responsemousewheel = (event = null) => {
    if (event === null) {
      this.mdz = 0;
      return;
    }
    if (event.deltaY > 0.0) {
      this.wheel = 1.125;
    } else {
      this.wheel = -1.125;
    }
    this.mz += this.wheel;
    this.mdz = this.wheel;
  };
  this.responsemouseclick = (event) => {
    let type;
    if (event.button === 2) {
      type = "rmouse";
    } else if (event.button === 0) {
      type = "lmouse";
    } else {
      return;
    }
    if (this.keyboard[`${type}`] === undefined) {
      this.keyboard[`${type}`] = 1;
    } else if (this.keyboard[`${type}`] === 1) {
      this.keyboard[`${type}`] = 0;
    } else {
      this.keyboard[`${type}`] = 1;
    }

    /*
    if (event.button === 0) {    
      if 
      this.keyboard["rmouse"] = 0;
    } else if (event.button === 2) {
      this.keyboard["lmouse"] = 1;                 
    } 
    */
  };
  this.keys = (key) => {
    if (this.keyboard[key] === undefined) {
      return 0;
    }
    return this.keyboard[key];
  };
  this.responsekeys = (event) => {
    if (this.keyboard[event.key] === 1.0) {
      this.keyboard[event.key] = 0;
    } else {
      this.keyboard[event.key] = 1.0;
    }
    //event.key - символ
    //event.code - куод
  };
  this.responsecamera = () => {
    if (this.keys("x")) {
      const time = 0.01;
      let Dist = subvec(this.at, this.loc).length(),
        cosT = (this.loc.y - this.at.y) / Dist,
        sinT = Math.sqrt(1 - cosT * cosT),
        plen = Dist * sinT,
        cosP = (this.loc.z - this.at.z) / plen,
        sinP = (this.loc.x - this.at.x) / plen,
        Azimuth = R2D(Math.atan2(sinP, cosP)),
        Elevator = R2D(Math.atan2(sinT, cosT));
      Azimuth +=
        time *
        3 *
        (-30 * this.keys("lmouse") * this.mdx +
          447 * (this.keys("d") - this.keys("a")));
      Elevator +=
        time *
        2 *
        (-300 * this.keys("lmouse") * this.mdy +
          447 * (this.keys("s") - this.keys("w")));
      if (Elevator < 0.08) Elevator = 0.08;
      if (Elevator > 178.9) Elevator = 178.9;
      Dist +=
        time *
        (1 + this.keys("Shift") * 208) *
        (1 * this.mdz + 80 * (this.keys("PageUp") - this.keys("PageDown")));
      if (Dist < 0.1) Dist = 0.1;
      if (this.keys("rmouse")) {
        let sx, sy;
        let dv = vec3(0);
        sx = (((-this.mdx * this.wp) / canvas.width) * Dist) / this.projdist;
        sy = (((this.mdy * this.hp) / canvas.height) * Dist) / this.projdist;
        dv = addvec(vec3(this.right).mul(sx), vec3(this.up).mul(sy));
        this.at = addvec(this.at, dv);
        this.loc = addvec(this.loc, dv);
      }
      this.set(
        pointtransform(
          vec3(0, Dist, 0),
          mulmatr3(
            matrrotateX(Elevator),
            matrrotateY(Azimuth),
            vec3(this.at).translete()
          )
        ),
        this.at,
        vec3(0, 1, 0)
      );
    }
  };

  this.set = (loc = vec3(8), at = vec3(0), up = vec3(0, 1, 0)) => {
    this.view = matrview(loc, at, up);

    this.vp = mulmatr(this.view, this.proj);
    this.dir = vec3(
      -this.view.massiv[0][2],
      -this.view.massiv[1][2],
      -this.view.massiv[2][2]
    );
    this.loc = loc;
    this.at = at;
    this.up = vec3(
      this.view.massiv[0][1],
      this.view.massiv[1][1],
      this.view.massiv[2][1]
    );

    this.right = vec3(
      this.view.massiv[0][0],
      this.view.massiv[1][0],
      this.view.massiv[2][0]
    );
  };
  this.hp = this.wp = this.projsize = this.projdist = 0.1;
  this.projfarclip = 30000;
  this.proj = matrfrustum(
    -this.wp / 2,
    this.wp / 2,
    -this.hp / 2,
    this.hp / 2,
    this.projdist,
    this.projfarclip
  );
  this.mx = this.my = this.mz = this.mdz = this.wheel = this.mdx = this.mdy = 0;
  this.keyboard = [];
  this.set(vec3(15, 15, 15));
  return this;
}
export function cameraResize() {
  Camera.hp = Camera.wp = Camera.projsize;
  if (canvas.width > canvas.height) {
    Camera.wp *= canvas.width / canvas.height;
  } else {
    Camera.hp *= canvas.height / canvas.width;
  }
}

//static VOID IZ3_KeyboardResponse( iz3UNIT_CONTROL*Uni, iz3ANIM *Ani )
//{
//  if (Ani ->KeysClick['P'])
//    IZ3_AnimFlipFullScreen();
//  if (Ani ->KeysClick['S'])
//    Ani ->IsPause = !Ani ->IsPause;
//  if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['W'])
//  {
//    INT modes[2];
//
//    glGetIntegerv(GL_POLYGON_MODE, modes);
//    if (modes[0] == GL_LINE)
//      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    else
//      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  }
//  if (Ani ->Keys[VK_CONTROL])
//  {
//    /* Handle camera orientatiom */
//    FLT
//      Dist = VecLen(VecSubVec(IZ3_RndCamAt, IZ3_RndCamLoc)),
//      cosT = (IZ3_RndCamLoc.Y - IZ3_RndCamAt.Y) / Dist,
//      sinT = sqrt(1 - cosT * cosT),
//      plen = Dist * sinT,
//      cosP = (IZ3_RndCamLoc.Z - IZ3_RndCamAt.Z) / plen,
//      sinP = (IZ3_RndCamLoc.X - IZ3_RndCamAt.X) / plen,
//      Azimuth = R2D(atan2(sinP, cosP)),
//      Elevator = R2D(atan2(sinT, cosT));
//
//    Azimuth += Ani ->GlobalDeltaTime * 3 *
//      (-30 * Ani ->Keys[VK_LBUTTON] * Ani ->Mdx +
//      47 * (Ani ->Keys[VK_RIGHT] - Ani ->Keys[VK_LEFT]));
//
//    Elevator += Ani ->GlobalDeltaTime * 2 *
//      (-30 * Ani ->Keys[VK_LBUTTON] * Ani ->Mdy +
//      47 * (Ani ->Keys[VK_DOWN] - Ani ->Keys[VK_UP]));
//    if (Elevator < 0.08)
//      Elevator = 0.08;
//    if (Elevator > 178.90)
//      Elevator = 178.90;
//
//    Dist += Ani ->GlobalDeltaTime * (1 + Ani ->Keys[VK_SHIFT] * 28) *
//      (2 * Ani ->Mdz +
//      8 * (Ani ->Keys[VK_NEXT] - IZ3_Anim.Keys[VK_PRIOR]));
//    if (Dist < 0.1)
//      Dist = 0.1;
//    if (Ani ->Keys[VK_RBUTTON])
//    {
//      FLT Wp, Hp, sx, sy;
//      VEC dv;
//
//      Wp = IZ3_RndProjSize;
//      Hp = IZ3_RndProjSize;
//      if (Ani ->W > Ani ->H)
//        Wp *= (FLT)Ani ->W / Ani ->H;
//      else
//        Hp *= (FLT)Ani ->H / Ani ->W;
//      sx = -Ani ->Mdx * Wp / Ani ->W * Dist / IZ3_RndProjDist;
//      sy = Ani ->Mdy * Hp / Ani ->H * Dist / IZ3_RndProjDist;
//
//      dv = VecAddVec(VecMulNum(IZ3_RndCamRight, sx),
//                     VecMulNum(IZ3_RndCamUp, sy));
//
//      IZ3_RndCamAt = VecAddVec(IZ3_RndCamAt, dv);
//      IZ3_RndCamLoc = VecAddVec(IZ3_RndCamLoc, dv);
//    }
//    IZ3_RndCamSet(PointTransform(VecSet(0, Dist, 0),
//                                MatrMulMatr3(MatrRotateX(Elevator),
//                                             MatrRotateY(Azimuth),
//                                             MatrTranslate(IZ3_RndCamAt))),
//                  IZ3_RndCamAt,
//                  VecSet(0, 1, 0));
//  }
//}
//export function cameraresponse(cntrl)
//{
//}
//
