let arrayJumps = [];
let soarTime = 2.0;
let hiegthJump = 10.0;
let soarOneTimeHiegthJump = (hiegthJump * 2.0) / soarTime;
function newCameraJump() {
  let newJump = {};
  newJump.startJumpTime = sys.localTime;
  newJump.averageHeightOfJump = hiegthJump;
  newJump.finalHeightOfJump = hiegthJump;
  arrayJumps.push(newJump);
}
export function camera() {
  let first = true;
  this.cameraJump = () => {
    //if (!!arrayJumps.isJump)
    //{
    //  let jump;
    //  if (arrayJumps.averageHeightOfJump !== 0.0)
    //  {
    //    jump =
    //      (sys.localTime - arrayJumps.startJumpTime) * soarOneTimeHiegthJump;
    //    if (arrayJumps.averageHeightOfJump - jump < 0.0) {
    //      jump = arrayJumps.averageHeightOfJump;
    //    }
    //    arrayJumps.averageHeightOfJump -= jump;
    //  } else {
    //    jump =
    //      (arrayJumps.startJumpTime - sys.localTime) * soarOneTimeHiegthJump;
    //    if (arrayJumps.finalHeightOfJump + jump < 0.0) {
    //      jump = arrayJumps.finalHeightOfJump;
    //      arrayJumps.isJump = false;
    //    }
    //    arrayJumps.finalHeightOfJump += jump;
    //  }
    //  arrayJumps.startJumpTime = sys.localTime;
    //  sys.loc.y += jump;
    //  sys.at.y += jump;
    //}
    arrayJumps.forEach((res) => {
      let jump;
      if (res.averageHeightOfJump !== 0.0) {
        jump = (sys.localTime - res.startJumpTime) * soarOneTimeHiegthJump;
        if (res.averageHeightOfJump - jump <= 0.0) {
          jump = res.averageHeightOfJump;
        }
        res.averageHeightOfJump -= jump;
      } else {
        jump = (res.startJumpTime - sys.localTime) * soarOneTimeHiegthJump;
        if (res.finalHeightOfJump + jump <= 0.0) {
          jump = res.finalHeightOfJump;
          let index = arrayJumps.indexOf(res);
          arrayJumps.splice(index, 1);
        }
        res.finalHeightOfJump += jump;
      }
      res.startJumpTime = sys.localTime;
      sys.loc.y += jump;
      sys.at.y += jump;
    });
  };
  this.responsemouse = function (event) {
    if (first) {
      this.mx = event.clientX;
      this.my = event.clientY;
    }
    this.mdx = event.clientX - this.mx;
    this.mdy = event.clientY - this.my;
    this.mx = event.clientX;
    this.my = event.clientY;
  };
  this.responsemousewheel = function (event) {
    if (event.deltaY > 0.0) {
      this.mdz = 0.5;
    } else {
      this.mdz = -0.5;
    }
    this.mz += this.mdz;
  };
  this.responsemouseup = function (event) {
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
  this.responsemousedown = function (event) {
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
  this.keys = function (key) {
    return !!this.keyboard[key];
  };
  this.responsekeyup = function (event) {
    this.keyboard[event.code] = 0;
  };
  this.responsekeydown = function (event) {
    this.keyboard[event.code] = 1;
    if (event.code === "Space") {
      newCameraJump();
    }
  };
  this.responsecamera = function () {
    let Dist = sys.subVec(this.at, this.loc).length(),
      cosT = (this.loc.y - this.at.y) / Dist,
      sinT = Math.sqrt(1 - cosT * cosT),
      plen = Dist * sinT,
      cosP = (this.loc.z - this.at.z) / plen,
      sinP = (this.loc.x - this.at.x) / plen,
      Azimuth = sys.R2D(Math.atan2(sinP, cosP)),
      Elevator = sys.R2D(Math.atan2(sinT, cosT));
    Azimuth +=
      2.875 * 1.875 * (this.keys("ArrowLeft") - this.keys("ArrowRight"));
    Elevator += 2 * 1.875 * (this.keys("ArrowDown") - this.keys("ArrowUp"));
    if (Elevator < 0.08) Elevator = 0.08;
    if (Elevator > 178.9) Elevator = 178.9;
    sys.azimuth = Azimuth;
    sys.elevator = Elevator;
    Dist +=
      (1 + this.keys("ShiftLeft") * 28) *
      (2 * this.mdz + 0.125 * (this.keys("PageUp") - this.keys("PageDown")));
    if (Dist < 0.1) Dist = 0.1;
    let sx, sz;
    let dv = sys.vec3(0);
    sx = -(this.keys("KeyA") - this.keys("KeyD")) * 3.125;
    sz = (this.keys("KeyW") - this.keys("KeyS")) * 3.125;
    dv = sys.addVec(
      sys.vec3(this.right).normalize().mul(sx),
      sys
        .vec3(this.at.x - this.loc.x, 0.0, this.at.z - this.loc.z)
        .normalize()
        .mul(sz)
    );
    sys.cameraJump();
    this.at = sys.addVec(this.at, dv);
    this.loc = sys.addVec(this.loc, dv);
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
  this.set = function (
    loc = sys.vec3(8),
    at = sys.vec3(0),
    up = sys.vec3(0, 1, 0)
  ) {
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
  this.wp = this.hp = this.projsize = this.projdist = 0.1;
  this.projfarclip = 30000;
  this.mx = this.my = this.mz = this.mdz = this.wheel = this.mdx = this.mdy = 0;
  this.keyboard = [];
  this.keyboard["lmouse"] =
    this.keyboard["ArrowDown"] =
    this.keyboard["rmouse"] =
    this.keyboard["PageUp"] =
    this.keyboard["PageDown"] =
    this.keyboard["ShiftLeft"] =
    this.keyboard["ArrowRight"] =
    this.keyboard["ArrowLeft"] =
    this.keyboard["ArrowUp"] =
      0.0;
  this.resize = function (w, h) {
    this.w = w;
    this.h = h;
    this.wp = this.hp = this.projsize;
    if (w > h) {
      this.wp *= w / h;
    } else {
      this.hp *= h / w;
    }
    this.proj = sys.matrFrustum(
      -this.wp / 2,
      this.wp / 2,
      -this.hp / 2,
      this.hp / 2,
      this.projdist,
      this.projfarclip
    );
    let max = sys.sizeField / 2,
      min = -sys.sizeField / 2;
    let pos = sys.vec3(
      Math.random() * (max - min) + min,
      sys.primcow.maxBB.y + 2,
      Math.random() * (max - min) + min
    );
    //let height = sys.primgrid.getHieghtFromGrid(sys.vec3(pos.x, 0, pos.z));
    //let height = 13.18665599822998 - -8.576634407043457;
    //console.log(height);
    //    let v = sys.vec3(
    //      sys.primcow.maxBB.x,
    //      (sys.primcow.maxBB.y + sys.primcow.minBB.y) / 2.0,
    //      (sys.primcow.maxBB.z + sys.primcow.minBB.z) / 2.0
    //    );
    //    console.log(v.x, v.y, v.z);
    //    v = sys.vec3(
    //      sys.primcow.minBB.x,
    //      (sys.primcow.maxBB.y + sys.primcow.minBB.y) / 2.0,
    //      (sys.primcow.maxBB.z + sys.primcow.minBB.z) / 2.0
    //    );
    //    console.log(v.x, v.y, v.z);
    this.keyA = this.keyS = this.keyD = this.keyW = 0;
    this.set(
      sys.vec3(pos.x, pos.y, pos.z),
      sys.vec3(pos.x + 0.1, pos.y, pos.z)
    );
    sys.locationName = sys.vec3(sys.at.x + 11, sys.primcow.maxBB.y, sys.at.z);
    this.responsecamera();
    this.startazimuth = this.azimuth;
    this.startelevator = this.elevator;
    this.startvp = this.vp;
    pos = sys.vec3(
      Math.random() * (max - min) + min,
      sys.primcow.maxBB.y + 2,
      Math.random() * (max - min) + min
    );
    sys.autoCow = sys.primcow;
    sys.autoCow.pos = pos;
  };
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
