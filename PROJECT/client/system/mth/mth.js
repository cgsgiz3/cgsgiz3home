const handleD2R = Math.PI / 180.0;
function GetN(P0 = sys.vec3(), P1 = sys.vec3(), P2 = sys.vec3()) {
  return sys.crossVec(sys.subVec(P1, P0), sys.subVec(P2, P0)).normalize();
}
class newvec3 {
  constructor(x, y, z) {
    try {
      this.x = x;
      this.y = y;
      this.z = z;
    } catch (err) {
      this.x = 1;
      this.y = 1;
      this.z = 1;
    }
  }
  type = "vec3";
  array = () => {
    return [this.x, this.y, this.z];
  };
  length = () => {
    let len = this.dot(this);

    if (len == 1.0 || len == 0.0) {
      return len;
    }
    return Math.sqrt(len);
  };
  negative = () => {
    this.x = -this.x;
    this.y = -this.y;
    this.z = -this.z;
    return this;
  };
  dot = (v = sys.vec3()) => {
    let t = v.x * this.x + v.y * this.y + v.z * this.z;
    return t;
  };
  div = (num = 1) => {
    this.x /= num;
    this.y /= num;
    this.z /= num;
    return this;
  };
  mul = (num = 1) => {
    this.x *= num;
    this.y *= num;
    this.z *= num;
    return this;
  };
  translete = () => {
    let mtr = sys.mat4();

    mtr.massiv[3] = [this.x, this.y, this.z, 1];
    return mtr;
  };
  normalize = () => {
    let len = this.length();
    if (len === 1 || len === 0) {
      return this;
    }
    this.div(len);
    return this;
  };
}
class newmat4 {
  constructor(v1, w1, v2, w2, v3, w3, v4, w4) {
    try {
      this.massiv = [
        [...v1.array(), w1],
        [...v2.array(), w2],
        [...v3.array(), w3],
        [...v4.array(), w4],
      ];
    } catch (err) {
      this.massiv = [
        [1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1],
      ];
    }
  }
}
export function mth() {
  this.addVec = function (v1 = this.vec3(), v2 = this.vec3()) {
    return this.vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
  };
  this.subVec = function (v1 = this.vec3(), v2 = this.vec3()) {
    return this.vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
  };
  this.crossVec = function (v1 = this.vec3(), v2 = this.vec3()) {
    return this.vec3(
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x
    );
  };
  this.autoNormals = function (pos = null, index = null, type = "trimesh") {
    const normal = [];
    if (pos === null) {
      return;
    }
    if (pos[0].type !== "vec3") {
      let massiv = [];
      let j = 0;
      for (let i = 0; i < pos.length; i += 3) {
        massiv[j++] = this.vec3(pos[i], pos[i + 1], pos[i + 2]);
      }
      pos = massiv;
    }
    for (let i = 0; i < pos.length; i++) {
      normal[i] = this.vec3(0);
    }
    if (index !== null) {
      let s = 3;
      if (type === "trimesh") {
        s = 3;
      } else if (type === "tristrip") {
        s = 1;
      }
      for (let i = 0; i < index.length; i += s) {
        let N = GetN(pos[index[i]], pos[index[i + 1]], pos[index[i + 2]]);
        normal[index[i]] = this.addVec(N, normal[index[i]]);
        normal[index[i + 1]] = this.addVec(N, normal[index[i + 1]]);
        normal[index[i + 2]] = this.addVec(N, normal[index[i + 2]]);
      }
    } else {
      let s = 3;
      if (type === "trimesh") {
        s = 3;
      } else if (type === "tristrip") {
        s = 1;
      }
      for (let i = 0; i < pos.length; i += s) {
        let N = GetN(pos[i], pos[i + 1], pos[i + 2]);
        normal[i] = this.addVec(N, normal[i]);
        normal[i + 1] = this.addVec(N, normal[i + 1]);
        normal[i + 2] = this.addVec(N, normal[i + 2]);
      }
    }
    for (let i = 0; i < pos.length; i++) {
      normal[i].normalize();
    }
    return normal;
  };
  this.matrOrtho = function (
    left = -10,
    right = 10,
    bottom = -10,
    top = 10,
    near = 1,
    far = 300
  ) {
    let mtr = this.mat4();

    mtr.massiv = [
      [2 / (right - left), 0, 0, 0],
      [0, 2 / (top - bottom), 0, 0],
      [0, 0, -2 / (far - near), 0],
      [
        -((right + left) / (right - left)),
        -((top + bottom) / (top - bottom)),
        -((far + near) / (far - near)),
        1,
      ],
    ];

    return mtr;
  };
  this.matrView = function (
    loc = this.vec3(5, 5, 5),
    at = this.vec3(0, 0, 0),
    up1 = this.vec3(0, 1, 0)
  ) {
    let dir = this.subVec(at, loc).normalize(),
      right = this.crossVec(dir, up1).normalize(),
      up = this.crossVec(right, dir),
      mtr = this.mat4();

    mtr.massiv = [
      [right.x, up.x, -dir.x, 0],
      [right.y, up.y, -dir.y, 0],
      [right.z, up.z, -dir.z, 0],
      [-loc.dot(right), -loc.dot(up), loc.dot(dir), 1],
    ];
    return mtr;
  };
  this.pointTransform = function (v = this.vec3(1), mtr = this.mat4()) {
    return this.vec3(
      v.x * mtr.massiv[0][0] +
        v.y * mtr.massiv[1][0] +
        v.z * mtr.massiv[2][0] +
        mtr.massiv[3][0],
      v.x * mtr.massiv[0][1] +
        v.y * mtr.massiv[1][1] +
        v.z * mtr.massiv[2][1] +
        mtr.massiv[3][1],
      v.x * mtr.massiv[0][2] +
        v.y * mtr.massiv[1][2] +
        v.z * mtr.massiv[2][2] +
        mtr.massiv[3][2]
    );
  };
  this.matrFrustum = function (
    l = -1 / 2,
    r = 1 / 2,
    b = -1 / 2,
    t = 1 / 2,
    n = 10,
    f = 300
  ) {
    let mtr = this.mat4();

    mtr.massiv = [
      [(2 * n) / (r - l), 0, 0, 0],
      [0, (2 * n) / (t - b), 0, 0],
      [(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1],
      [0, 0, -(2 * n * f) / (f - n), 0],
    ];
    return mtr;
  };
  this.D2R = function (A) {
    return A * handleD2R;
  };
  this.R2D = function (A) {
    return A / handleD2R;
  };
  this.matrRotateX = function (Angle = 90) {
    let a = this.D2R(Angle);
    let mtr = this.mat4();

    mtr.massiv = [
      [1, 0, 0, 0],
      [0, Math.cos(a), Math.sin(a), 0],
      [0, -Math.sin(a), Math.cos(a), 0],
      [0, 0, 0, 1],
    ];
    return mtr;
  };
  this.matrRotateY = function (Angle = 90) {
    let a = this.D2R(Angle);

    let mtr = this.mat4(0);
    mtr.massiv = [
      [Math.cos(a), 0, -Math.sin(a), 0],
      [0, 1, 0, 0],
      [Math.sin(a), 0, Math.cos(a), 0],
      [0, 0, 0, 1],
    ];
    return mtr;
  };
  this.matrRotateZ = function (Angle = 90) {
    let a = this.D2R(Angle);
    let mtr = this.mat4();

    mtr.massiv = [
      [Math.cos(a), Math.sin(a), 0, 0],
      [-Math.sin(a), Math.cos(a), 0, 0],
      [0, 0, 1, 0],
      [0, 0, 0, 1],
    ];
    return mtr;
  };
  this.matrScale = function (v = this.vec3(1)) {
    let mtr = this.mat4();

    mtr.massiv[0][0] = v.x;
    mtr.massiv[1][1] = v.y;
    mtr.massiv[2][2] = v.z;
    return mtr;
  };
  this.mulMatr = function (mtr1 = this.mat4(), mtr2 = this.mat4()) {
    let mtr = this.mat4(0);
    /*
    for (let i = 0; i < 4; i++) {
      for (let j = 0; j < 4; j++) {
        for (let k = 0; k < 4; k++) {
          mtr.massiv[i][j] += mtr1.massiv[i][k] * mtr2.massiv[k][j];
        }
      }
    }
    /**/
    mtr.massiv[0][0] += mtr1.massiv[0][0] * mtr2.massiv[0][0];
    mtr.massiv[0][0] += mtr1.massiv[0][1] * mtr2.massiv[1][0];
    mtr.massiv[0][0] += mtr1.massiv[0][2] * mtr2.massiv[2][0];
    mtr.massiv[0][0] += mtr1.massiv[0][3] * mtr2.massiv[3][0];
    mtr.massiv[0][1] += mtr1.massiv[0][0] * mtr2.massiv[0][1];
    mtr.massiv[0][1] += mtr1.massiv[0][1] * mtr2.massiv[1][1];
    mtr.massiv[0][1] += mtr1.massiv[0][2] * mtr2.massiv[2][1];
    mtr.massiv[0][1] += mtr1.massiv[0][3] * mtr2.massiv[3][1];
    mtr.massiv[0][2] += mtr1.massiv[0][0] * mtr2.massiv[0][2];
    mtr.massiv[0][2] += mtr1.massiv[0][1] * mtr2.massiv[1][2];
    mtr.massiv[0][2] += mtr1.massiv[0][2] * mtr2.massiv[2][2];
    mtr.massiv[0][2] += mtr1.massiv[0][3] * mtr2.massiv[3][2];
    mtr.massiv[0][3] += mtr1.massiv[0][0] * mtr2.massiv[0][3];
    mtr.massiv[0][3] += mtr1.massiv[0][1] * mtr2.massiv[1][3];
    mtr.massiv[0][3] += mtr1.massiv[0][2] * mtr2.massiv[2][3];
    mtr.massiv[0][3] += mtr1.massiv[0][3] * mtr2.massiv[3][3];

    mtr.massiv[1][0] += mtr1.massiv[1][0] * mtr2.massiv[0][0];
    mtr.massiv[1][0] += mtr1.massiv[1][1] * mtr2.massiv[1][0];
    mtr.massiv[1][0] += mtr1.massiv[1][2] * mtr2.massiv[2][0];
    mtr.massiv[1][0] += mtr1.massiv[1][3] * mtr2.massiv[3][0];
    mtr.massiv[1][1] += mtr1.massiv[1][0] * mtr2.massiv[0][1];
    mtr.massiv[1][1] += mtr1.massiv[1][1] * mtr2.massiv[1][1];
    mtr.massiv[1][1] += mtr1.massiv[1][2] * mtr2.massiv[2][1];
    mtr.massiv[1][1] += mtr1.massiv[1][3] * mtr2.massiv[3][1];
    mtr.massiv[1][2] += mtr1.massiv[1][0] * mtr2.massiv[0][2];
    mtr.massiv[1][2] += mtr1.massiv[1][1] * mtr2.massiv[1][2];
    mtr.massiv[1][2] += mtr1.massiv[1][2] * mtr2.massiv[2][2];
    mtr.massiv[1][2] += mtr1.massiv[1][3] * mtr2.massiv[3][2];
    mtr.massiv[1][3] += mtr1.massiv[1][0] * mtr2.massiv[0][3];
    mtr.massiv[1][3] += mtr1.massiv[1][1] * mtr2.massiv[1][3];
    mtr.massiv[1][3] += mtr1.massiv[1][2] * mtr2.massiv[2][3];
    mtr.massiv[1][3] += mtr1.massiv[1][3] * mtr2.massiv[3][3];

    mtr.massiv[2][0] += mtr1.massiv[2][0] * mtr2.massiv[0][0];
    mtr.massiv[2][0] += mtr1.massiv[2][1] * mtr2.massiv[1][0];
    mtr.massiv[2][0] += mtr1.massiv[2][2] * mtr2.massiv[2][0];
    mtr.massiv[2][0] += mtr1.massiv[2][3] * mtr2.massiv[3][0];
    mtr.massiv[2][1] += mtr1.massiv[2][0] * mtr2.massiv[0][1];
    mtr.massiv[2][1] += mtr1.massiv[2][1] * mtr2.massiv[1][1];
    mtr.massiv[2][1] += mtr1.massiv[2][2] * mtr2.massiv[2][1];
    mtr.massiv[2][1] += mtr1.massiv[2][3] * mtr2.massiv[3][1];
    mtr.massiv[2][2] += mtr1.massiv[2][0] * mtr2.massiv[0][2];
    mtr.massiv[2][2] += mtr1.massiv[2][1] * mtr2.massiv[1][2];
    mtr.massiv[2][2] += mtr1.massiv[2][2] * mtr2.massiv[2][2];
    mtr.massiv[2][2] += mtr1.massiv[2][3] * mtr2.massiv[3][2];
    mtr.massiv[2][3] += mtr1.massiv[2][0] * mtr2.massiv[0][3];
    mtr.massiv[2][3] += mtr1.massiv[2][1] * mtr2.massiv[1][3];
    mtr.massiv[2][3] += mtr1.massiv[2][2] * mtr2.massiv[2][3];
    mtr.massiv[2][3] += mtr1.massiv[2][3] * mtr2.massiv[3][3];

    mtr.massiv[3][0] += mtr1.massiv[3][0] * mtr2.massiv[0][0];
    mtr.massiv[3][0] += mtr1.massiv[3][1] * mtr2.massiv[1][0];
    mtr.massiv[3][0] += mtr1.massiv[3][2] * mtr2.massiv[2][0];
    mtr.massiv[3][0] += mtr1.massiv[3][3] * mtr2.massiv[3][0];
    mtr.massiv[3][1] += mtr1.massiv[3][0] * mtr2.massiv[0][1];
    mtr.massiv[3][1] += mtr1.massiv[3][1] * mtr2.massiv[1][1];
    mtr.massiv[3][1] += mtr1.massiv[3][2] * mtr2.massiv[2][1];
    mtr.massiv[3][1] += mtr1.massiv[3][3] * mtr2.massiv[3][1];
    mtr.massiv[3][2] += mtr1.massiv[3][0] * mtr2.massiv[0][2];
    mtr.massiv[3][2] += mtr1.massiv[3][1] * mtr2.massiv[1][2];
    mtr.massiv[3][2] += mtr1.massiv[3][2] * mtr2.massiv[2][2];
    mtr.massiv[3][2] += mtr1.massiv[3][3] * mtr2.massiv[3][2];
    mtr.massiv[3][3] += mtr1.massiv[3][0] * mtr2.massiv[0][3];
    mtr.massiv[3][3] += mtr1.massiv[3][1] * mtr2.massiv[1][3];
    mtr.massiv[3][3] += mtr1.massiv[3][2] * mtr2.massiv[2][3];
    mtr.massiv[3][3] += mtr1.massiv[3][3] * mtr2.massiv[3][3];
    return mtr;
  };
  this.pointTransform = function (v = this.vec3(), mtr = this.mat4()) {
    return this.vec3(
      v.x * mtr.massiv[0][0] +
        v.y * mtr.massiv[1][0] +
        v.z * mtr.massiv[2][0] +
        mtr.massiv[3][0],
      v.x * mtr.massiv[0][1] +
        v.y * mtr.massiv[1][1] +
        v.z * mtr.massiv[2][1] +
        mtr.massiv[3][1],
      v.x * mtr.massiv[0][2] +
        v.y * mtr.massiv[1][2] +
        v.z * mtr.massiv[2][2] +
        mtr.massiv[3][2]
    );
  };
  this.vecMulMatr = function (v = this.vec3(), mtr = this.mat4()) {
    let w =
      v.x * mtr.massiv[0][3] +
      v.y * mtr.massiv[1][3] +
      v.z * mtr.massiv[2][3] +
      mtr.massiv[3][3];

    return this.vec3(
      (v.x * mtr.massiv[0][0] +
        v.y * mtr.massiv[1][0] +
        v.z * mtr.massiv[2][0] +
        mtr.massiv[3][0]) /
        w,
      (v.x * mtr.massiv[0][1] +
        v.y * mtr.massiv[1][1] +
        v.z * mtr.massiv[2][1] +
        mtr.massiv[3][1]) /
        w,
      (v.x * mtr.massiv[0][2] +
        v.y * mtr.massiv[1][2] +
        v.z * mtr.massiv[2][2] +
        mtr.massiv[3][2]) /
        w
    );
  };

  this.mulMatr3 = function (
    mtr1 = this.mat4(),
    mtr2 = this.mat4(),
    mtr3 = this.mat4()
  ) {
    return this.mulMatr(this.mulMatr(mtr1, mtr2), mtr3);
  };
  this.mat4 = function (
    v1 = this.vec3(1, 0, 0),
    w1 = 0,
    v2 = this.vec3(0, 1, 0),
    w2 = 0,
    v3 = this.vec3(0, 0, 1),
    w3 = 0,
    v4 = this.vec3(0, 0, 0),
    w4 = 1
  ) {
    if (typeof v1 === "number") {
      w1 = w2 = w3 = w4 = v1;
      v1 = v2 = v3 = v4 = this.vec3(v1);
    }
    let matrix = new newmat4(v1, w1, v2, w2, v3, w3, v4, w4);
    return matrix;
  };
  this.vec3 = function (x = null, y = null, z = null) {
    if (typeof x === "number" && y === null) {
      y = z = x;
    } else if (x === null && y === null) {
      x = y = z = 1;
    } else if (typeof x === "string" && y === null) {
      x = parseFloat(x);
      if (typeof x === "number") {
        y = z = x;
      } else {
        return;
      }
    } else if (typeof x === "object" && y === null) {
      y = x.y;
      z = x.z;
      x = x.x;
    }
    return new newvec3(x, y, z);
  };
  return this;
}
