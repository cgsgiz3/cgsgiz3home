export function camera()
{
  let first = true;
  this.responsemouse = function(event)
  {
    if (first)
    {
      this.mx = event.clientX;
      this.my = event.clientY;
    }
    this.mdx = event.clientX - this.mx;
    this.mdy = event.clientY - this.my;
    this.mx = event.clientX;
    this.my = event.clientY;
  };
  this.responsemousewheel = function(event) {
    if (event.deltaY > 0.0) {
      this.mdz = 0.5;
    } else {
      this.mdz = -0.5;
    }
    this.mz += this.mdz;
  };
  this.responsemouseup = function(event)
  {
    let type;
    if (event.which === 1 && event.button === 0) {
      type = "lmouse";
    } else if (event.button === 2 && event.which === 3) {
      type = "rmouse";
    } else {
      return;
    }
    this.keyboard[type] = 0;
  };
  this.responsemousedown = function(event)
  {
    let type;
    if (event.which === 1 && event.button === 0) {
      type = "lmouse";
    } else if (event.button === 2 && event.which === 3) {
      type = "rmouse";
    } else {
      return;
    }
    this.keyboard[type] = 1;
  };
  this.keys = function(key) 
  {
    return !!this.keyboard[key];
  };
  this.responsekeyup = function(event) {
    this.keyboard[event.code] = 0;

  }
  this.responsekeydown = function(event)
  {
    this.keyboard[event.code] = 1;
  };
  this.responsecamera = function()
  {
    let Dist = sys.subVec(this.at, this.loc).length(),
      cosT = (this.loc.y - this.at.y) / Dist,
      sinT = Math.sqrt(1 - cosT * cosT),
      plen = Dist * sinT,
      cosP = (this.loc.z - this.at.z) / plen,
      sinP = (this.loc.x - this.at.x) / plen,
      Azimuth = sys.R2D(Math.atan2(sinP, cosP)),
      Elevator = sys.R2D(Math.atan2(sinT, cosT));
    Azimuth += 3 * 1.875 * (this.keys("ArrowRight") - this.keys("ArrowLeft"));
    Elevator += 2 * 1.875 * (this.keys("ArrowDown") - this.keys("ArrowUp"));
    if (Elevator < 0.08) Elevator = 0.08;
    if (Elevator > 178.9) Elevator = 178.9;
    Dist += (1 + this.keys("ShiftLeft") * 28) * (2 * this.mdz + 0.125 * (this.keys("PageUp") - this.keys("PageDown")));
    if (Dist < 0.1) Dist = 0.1;
    if (this.keys("rmouse")) 
    {
      let sx, sy;
      let dv = sys.vec3(0);
      sx = (((-this.mdx * this.wp) / canvas.width) * Dist) / this.projdist;
      sy = (((this.mdy * this.hp) / canvas.height) * Dist) / this.projdist;
      dv = sys.addVec(vec3(this.right).mul(sx), sys.vec3(this.up).mul(sy));
      this.at = sys.addVec(this.at, dv);
      this.loc = sys.addVec(this.loc, dv);
    }
    this.set(
      sys.pointTransform(
        sys.vec3(0, Dist, 0),
        sys.mulMatr3(
          sys.matrRotateX(Elevator),
          sys.matrRotateY(Azimuth),
          this.at.translete()
        )
      ),
      this.at,
      sys.vec3(0, 1, 0)
    );
    this.mdz = this.mdx = this.mdy = 0.0;
  };
  this.set = function(loc = sys.vec3(8), at = sys.vec3(0), up = sys.vec3(0, 1, 0))
  {
    this.view = sys.matrView(loc, at, up);

    this.vp = sys.mulMatr(this.view, this.proj);
    this.dir = sys.vec3(
      -this.view.massiv[0][2],
      -this.view.massiv[1][2],
      -this.view.massiv[2][2]
    );
    this.loc = loc;
    this.at = at;
    this.up = sys.vec3(
      this.view.massiv[0][1],
      this.view.massiv[1][1],
      this.view.massiv[2][1]
    );

    this.right = sys.vec3(
      this.view.massiv[0][0],
      this.view.massiv[1][0],
      this.view.massiv[2][0]
    );
  };
  this.projsize = this.projdist = 0.1;
  this.projfarclip = 30000;
  this.mx = this.my = this.mz = this.mdz = this.wheel = this.mdx = this.mdy = 0;
  this.keyboard = [];
  this.keyboard["lmouse"] = this.keyboard["ArrowDown"] = this.keyboard["rmouse"] = this.keyboard["PageUp"] = this.keyboard["PageDown"] = this.keyboard["ShiftLeft"] = this.keyboard["ArrowRight"] = this.keyboard["ArrowLeft"] = this.keyboard["ArrowUp"] = 0.0;
  this.resize = function(w, h)
  {
    let rx = this.projsize, ry = this.projsize;
    if (w > h) {
      rx *= w / h;
    }
    else {
      ry *= h / w;
    }
    this.proj = sys.matrFrustum(
      -rx / 2,
      rx / 2,
      -ry / 2,
      ry / 2,
      this.projdist,
      this.projfarclip
    );
    this.set();
  }
  return this;
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
