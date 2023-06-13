const handleD2R = Math.PI / 180.0;
let GetN = (P0 = vec3(), P1 = vec3(), P2 = vec3()) => {
  return crossvec(subvec(P1, P0), subvec(P2, P0)).normalize();
};
export { GetN };
export class newvec3 {
  ///
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

    if (len == 1 || len == 0) return len;
    return Math.sqrt(len);
  };
  dot = (v = vec3()) => {
    return v.x * this.x + v.y * this.y + v.z * this.z;
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
    this.massiv = mat4();

    this.massiv[0] = [1, 0, 0, 0];
    this.massiv[1] = [0, 1, 0, 0];
    this.massiv[2] = [0, 0, 1, 0];
    this.massiv[3] = [this.x, this.y, this.z, 1];

    return this.massiv;
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
export function addvec(v1 = vec3(), v2 = vec3()) {
  return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
export function subvec(v1 = vec3(), v2 = vec3()) {
  return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
export function autonormals(pos = null, index = null, type = "trimesh") {
  const normal = [];
  if (pos === null || pos.length === 0) {
    return;
  }
  if (pos[0].type !== "vec3") {
    let massiv = [];
    let j = 0;
    for (let i = 0; i < pos.length; i += 3) {
      massiv[j++] = vec3(pos[i], pos[i + 1], pos[i + 2]);
    }
    pos = massiv;
  }
  for (let i = 0; i < pos.length; i++) {
    normal[i] = vec3(0);
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
      normal[index[i]] = addvec(N, normal[index[i]]);
      normal[index[i + 1]] = addvec(N, normal[index[i + 1]]);
      normal[index[i + 2]] = addvec(N, normal[index[i + 2]]);
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
      normal[i] = addvec(N, normal[i]);
      normal[i + 1] = addvec(N, normal[i + 1]);
      normal[i + 2] = addvec(N, normal[i + 2]);
    }
  }
  for (let i = 0; i < pos.length; i++) {
    normal[i].normalize();
  }
  return normal;
}
export function crossvec(v1 = vec3(), v2 = vec3()) {
  return vec3(
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x
  );
}
export function matrortho(
  left = -10,
  right = 10,
  bottom = -10,
  top = 10,
  near = 1,
  far = 300
) {
  let mtr = mat4();

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
}
export function matrview(
  loc = vec3(5, 5, 5),
  at = vec3(0, 0, 0),
  up1 = vec3(0, 1, 0)
) {
  let dir = subvec(at, loc).normalize(),
    right = crossvec(dir, up1).normalize(),
    up = crossvec(right, dir),
    mtr = mat4();

  mtr.massiv = [
    [right.x, up.x, -dir.x, 0],
    [right.y, up.y, -dir.y, 0],
    [right.z, up.z, -dir.z, 0],
    [-loc.dot(right), -loc.dot(up), loc.dot(dir), 1],
  ];
  return mtr;
}
export function pointtransform(vec = vec3(1), mtr = matridentity()) {
  return vec3(
    vec.x * mtr.massiv[0][0] +
      vec.y * mtr.massiv[1][0] +
      vec.z * mtr.massiv[2][0] +
      mtr.massiv[3][0],
    vec.x * mtr.massiv[0][1] +
      vec.y * mtr.massiv[1][1] +
      vec.z * mtr.massiv[2][1] +
      mtr.massiv[3][1],
    vec.x * mtr.massiv[0][2] +
      vec.y * mtr.massiv[1][2] +
      vec.z * mtr.massiv[2][2] +
      mtr.massiv[3][2]
  );
}
export function matrfrustum(
  l = -1 / 2,
  r = 1 / 2,
  b = -1 / 2,
  t = 1 / 2,
  n = 10,
  f = 300
) {
  let mtr = mat4();

  mtr.massiv = [
    [(2 * n) / (r - l), 0, 0, 0],
    [0, (2 * n) / (t - b), 0, 0],
    [(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1],
    [0, 0, -(2 * n * f) / (f - n), 0],
  ];
  return mtr;
}
export function matrrotateZ(Angle = 0) {
  let a = D2R(Angle);
  let mtr = mat4(0);

  mtr.massiv = [
    [Math.cos(a), Math.sin(a), 0, 0],
    [-Math.sin(a), Math.cos(a), 0, 0],
    [0, 0, 1, 0],
    [0, 0, 0, 1],
  ];
  return mtr;
}
export function matrrotateX(Angle = 0) {
  let a = D2R(Angle);
  let mtr = mat4(0);

  mtr.massiv = [
    [1, 0, 0, 0],
    [0, Math.cos(a), Math.sin(a), 0],
    [0, -Math.sin(a), Math.cos(a), 0],
    [0, 0, 0, 1],
  ];
  return mtr;
}
export function D2R(A) {
  return A * handleD2R;
}
export function R2D(A) {
  return A * (1 / handleD2R);
}
export function matrrotateY(Angle = 0) {
  let a = D2R(Angle);

  let mtr = mat4(0);
  mtr.massiv = [
    [Math.cos(a), 0, -Math.sin(a), 0],
    [0, 1, 0, 0],
    [Math.sin(a), 0, Math.cos(a), 0],
    [0, 0, 0, 1],
  ];
  return mtr;
}
export function matrScale(v = vec3(1)) {
  let mtr = matridentity();

  mtr.massiv[0][0] = v.x;
  mtr.massiv[1][1] = v.y;
  mtr.massiv[2][2] = v.z;
  return mtr;
}

export function mulmatr3(mtr1 = mat4(), mtr2 = mat4(), mtr3 = mat4()) {
  return mulmatr(mulmatr(mtr1, mtr2), mtr3);
}
export function mulmatr(mtr1 = mat4(), mtr2 = mat4()) {
  let mtr = mat4(0);
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
}
export class newmat4 {
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
export function matridentity() {
  let mtr = mat4(0);
  mtr.massiv[0][0] = 1;
  mtr.massiv[1][1] = 1;
  mtr.massiv[2][2] = 1;
  mtr.massiv[3][3] = 1;
  return mtr;
}
export function mat4(
  v1 = vec3(1, 0, 0),
  w1 = 0,
  v2 = vec3(0, 1, 0),
  w2 = 0,
  v3 = vec3(0, 0, 1),
  w3 = 0,
  v4 = vec3(0, 0, 0),
  w4 = 1
) {
  if (typeof v1 === "number") {
    w1 = w2 = w3 = w4 = v1;
    v1 = v2 = v3 = v4 = vec3(v1);
  }
  let matrix = new newmat4(v1, w1, v2, w2, v3, w3, v4, w4);
  return matrix;
}
export function vec3(x = null, y = null, z = null) {
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
}
