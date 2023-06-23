import { uniform_buffer, vertex_buffer, index_buffer } from "./buffer.js";
import { createshader } from "./shader.js";
import { cube_texture, texture_2D } from "./texture/texture.js";

function takemassiv(obj) {
  if (Array.isArray(obj)) {
    let massiv = [];
    obj.forEach((res) => {
      massiv.push(...takemassiv(res));
    });
    return massiv;
  } else if (typeof obj === "function") {
    return takemassiv(obj());
  } else if (typeof obj === "object") {
    let massiv = [];
    if (obj.type === "vec3") {
      massiv = [obj.x, obj.y, obj.z];
    } else {
      for (let property in obj) {
        massiv.push(...takemassiv(obj[property]));
      }
    }
    return massiv;
  } else if (typeof obj === "number") {
    return [obj];
  }
}
function gettype(type) {
  if (typeof type === "number") {
    return type;
  }
  if (type === "trimesh") {
    return gl.TRIANGLES;
  } else if (type === "tristrip") {
    return gl.TRIANGLE_STRIP;
  } else if (type === "linestrip") {
    return gl.LINE_STRIP;
  } else {
    return gl.POINTS;
  }
}
export function primitiveEvalBB(primitive, pos) {
  let massiv = [];
  if (pos === null || pos.length == 0) {
    return;
  }
  if (pos[0].type !== "vec3") {
    let j = 0;
    for (let i = 0; i < pos.length; i += 3) {
      massiv[j++] = sys.vec3(pos[i], pos[i + 1], pos[i + 2]);
    }
  } else {
    massiv = pos;
  }
  primitive.maxBB = sys.vec3(massiv[0]);
  primitive.minBB = sys.vec3(massiv[0]);
  for (let i = 0; i < massiv.length; i++) {
    if (primitive.minBB.x > massiv[i].x) {
      primitive.minBB.x = massiv[i].x;
    }
    if (primitive.maxBB.x < massiv[i].x) {
      primitive.maxBB.x = massiv[i].x;
    }

    if (primitive.minBB.y > massiv[i].y) {
      primitive.minBB.y = massiv[i].y;
    }
    if (primitive.maxBB.y < massiv[i].y) {
      primitive.maxBB.y = massiv[i].y;
    }

    if (primitive.minBB.z > massiv[i].z) {
      primitive.minBB.z = massiv[i].z;
    }
    if (primitive.maxBB.z < massiv[i].z) {
      primitive.maxBB.z = massiv[i].z;
    }
  }
}
export function createprimitive(primitive) {
  if (primitive.texture === undefined) {
    primitive.texture = {};
  }
  primitiveEvalBB(primitive, primitive.buffer.in_pos);
  if (!primitive.instance) {
    primitive.draw = (mtr = sys.mat4()) => {
      gl.useProgram(this.shaderProgram);
      if (primitive.frameBuffer != undefined) {
        primitive.frameBuffer.update(takemassiv([sys.vp, mtr]));
        primitive.frameBuffer.apply(this.shaderProgram);
      }
      for (let property in primitive.uniformBlocks) {
        primitive.uniformBlocks[property].update(
          takemassiv(primitive.uniform[property])
        );
        primitive.uniformBlocks[property].apply(this.shaderProgram);
      }
      let i = 0;
      for (let property in primitive.texture) {
        primitive.texture[property].apply(i++);
      }
      gl.bindVertexArray(this.vertexArray);
      primitive.buffer["index"].apply();
      gl.drawElements(
        primitive.type,
        primitive.numOfVerices,
        gl.UNSIGNED_SHORT,
        0
      );
    };
  } else {
    primitive.draw = (mtr = sys.mat4()) => {
      gl.useProgram(this.shaderProgram);
      if (primitive.frameBuffer != undefined) {
        primitive.frameBuffer.update(takemassiv([sys.vp, mtr]));
        primitive.frameBuffer.apply(this.shaderProgram);
      }
      for (let property in primitive.uniformBlocks) {
        primitive.uniformBlocks[property].update(
          takemassiv(primitive.uniform[property])
        );
        primitive.uniformBlocks[property].apply(this.shaderProgram);
      }
      let i = 0;
      for (let property in primitive.texture) {
        primitive.texture[property].apply(i++);
      }
      gl.bindVertexArray(this.vertexArray);
      primitive.buffer["index"].apply();
      gl.drawElementsInstanced(
        primitive.type,
        primitive.numOfVerices,
        gl.UNSIGNED_SHORT,
        0,
        primitive.instance
      );
    };
  }
  let res = createshader(primitive.file);
  primitive.updateShader = (number) => {
    this.shaderProgram = res[2](number);
  };
  primitive.type = gettype(primitive.type);
  this.vertexArray = gl.createVertexArray();
  gl.bindVertexArray(this.vertexArray);
  this.shaderProgram = res[0];
  if (primitive.buffer["index"] !== undefined) {
    primitive.buffer["index"] = new index_buffer(
      takemassiv(primitive.buffer["index"])
    );
    primitive.numOfVerices = primitive.buffer["index"].numOfIndices;
  } else {
    if (typeof primitive.numOfVerices !== "number") {
      if (primitive.buffer["in_pos"] === undefined) {
        console.log("don't know count of vertex point");
      } else {
        if (res[1].attrs["in_pos"] !== undefined) {
          let num = 1;
          if (res[1].attrs["in_pos"].type === 35666) {
            num = 4;
          } else if (res[1].attrs["in_pos"].type === 35665) {
            num = 3;
          } else if (res[1].attrs["in_pos"].type === 35664) {
            num = 2;
          }
          primitive.numOfVerices = primitive.buffer["in_pos"].length / num;
        } else {
          console.log("don't know count of vertex point");
        }
      }
    }
  }
  for (let property in res[1].attrs) {
    property = res[1].attrs[property];
    if (primitive.buffer[property.name] === undefined) {
      continue;
    }
    primitive.buffer[property.name] = new vertex_buffer(
      takemassiv(primitive.buffer[property.name])
    );
    if (property.type === 35666) {
      gl.vertexAttribPointer(property.loc, 4, gl.FLOAT, false, 0, 0);
    } else if (property.type === 35665) {
      gl.vertexAttribPointer(property.loc, 3, gl.FLOAT, false, 0, 0);
    } else if (property.type === 35664) {
      gl.vertexAttribPointer(property.loc, 2, gl.FLOAT, false, 0, 0);
    }
    gl.enableVertexAttribArray(property.loc);
  }
  let i = 0;
  primitive.uniformBlocks = {};
  for (let property in res[1].uniformBlocks) {
    property = res[1].uniformBlocks[property];
    if (property.name === "frameBuffer") {
      primitive.frameBuffer = {};
      primitive.frameBuffer = new uniform_buffer(
        property.name,
        property.index,
        i,
        property.size
      );
      i++;
      continue;
    }
    if (primitive.uniform[property.name] === undefined) {
      continue;
    }
    primitive.uniformBlocks[property.name] = new uniform_buffer(
      property.name,
      property.index,
      i,
      property.size
    );
    i++;
  }
  for (let property in primitive.texture) {
    if (res[1].uniforms[property] === undefined) {
      delete primitive.texture[property];
      continue;
    }
    if (res[1].uniforms[property].type === 35678) {
      primitive.texture[property] = new texture_2D(
        primitive.texture[property],
        res[1].uniforms[property].loc
      );
    } else if (res[1].uniforms[property].type === 35680) {
      primitive.texture[property] = new cube_texture(
        primitive.texture[property],
        res[1].uniforms[property].loc
      );
    }
  }
}
export function primitiveGridNoise(size, min, max) {
  let tabNoise = [],
    perm = [],
    i;
  for (i = 0; i < size; i++) tabNoise[i] = Math.random() * (max - min) + min;
  for (i = 0; i < size; i++) perm[i] = i;
  for (i = 0; i < size; i++) {
    let a = Math.floor(Math.random() * size),
      b = Math.floor(Math.random() * size),
      tmp = perm[a];

    perm[a] = perm[b];
    perm[b] = tmp;
  }
  function Noise1D(x) {
    let ix = Math.floor(x),
      ix1,
      fx;

    fx = x - ix;
    ix = ix % size;
    ix1 = (ix + 1) % size;
    return tabNoise[ix] * (1 - fx) + tabNoise[ix1] * fx;
  }
  return function noiseTurb1D(x, octaves) {
    x = perm[x];
    let i,
      frac = 1,
      val = 0;

    for (i = 0; i < octaves; i++) {
      val += Noise1D(x) / frac;
      x *= 2;
      frac *= 2;
    }
    return (val * (1 << (octaves - 1))) / ((1 << octaves) - 1);
  };
}
export function primitiveCreateGrid(
  w = 100,
  h = 100,
  HeightFieldSize = 100000.0
) {
  let primitive = {};
  let k = 0,
    I = [],
    V = [],
    N = [];
  primitive.file = "grid";
  primitive.type = "tristrip";
  primitive.buffer = {};
  primitive.uniform = {
    materialBuffer: sys.mtlGetDef(),
    cameraBuffer: () => {
      return [
        [...sys.loc.array(), 1],
        [...sys.dir.array(), 1],
        [...sys.right.array(), 1],
        [...sys.up.array(), 1],
        [sys.localTime, sys.globalDeltaTime, 1, 1],
      ];
    },
  };
  let noise = primitiveGridNoise(w * h, 0.0, 2.0);
  for (let i = 0; i < h; i++) {
    for (let j = 0; j < w; j++) {
      V[i * w + j] = sys.vec3(
        (HeightFieldSize * j) / (w - 1.0) - HeightFieldSize / 2,
        noise(i * w + j, 2),
        HeightFieldSize * (1 - i / (h - 1.0)) - HeightFieldSize / 2
      );
    }
  }
  primitive.getHieghtFromGrid = (v = sys.vec3()) => {
    if (
      v.x > HeightFieldSize / 2.0 ||
      v.x < -HeightFieldSize / 2.0 ||
      v.z > HeightFieldSize / 2.0 ||
      v.z < -HeightFieldSize / 2 / 0
    ) {
      return 0;
    }
    v.x += HeightFieldSize / 2;
    v.z += HeightFieldSize / 2;
    let x1 =
      (v.x - (v.x % (HeightFieldSize / (w - 1.0)))) /
      (HeightFieldSize / (w - 1.0));
    let x2 = x1 + 1;
    let y1 =
      (v.z - (v.z % (HeightFieldSize / (h - 1.0)))) /
      (HeightFieldSize / (h - 1.0));
    let y2 = y1 + 1;

    //let x1 =
    //  (v.x - (v.x % HeightFieldSize) / (w - 1.0)) /
    //  (v.x % HeightFieldSize) /
    //  (w - 1.0);
    //let x2 =
    //  (x1 + (v.x % HeightFieldSize) / (w - 1.0)) /
    //  (v.x % HeightFieldSize) /
    //  (w - 1.0);
    //let y1 =
    //  (v.z - (v.z % HeightFieldSize) / (h - 1.0)) /
    //  (v.z % HeightFieldSize) /
    //  (h - 1.0);
    //let y2 =
    //  (y1 + (v.z % HeightFieldSize) / (h - 1.0)) /
    //  (v.z % HeightFieldSize) /
    //  (h - 1.0);
    return (
      ((V[y1 * w + x1].y + V[y1 * w + x2].y + V[y2 * w + x1].y) / 3.0 +
        (V[y1 * w + x2].y + V[y2 * w + x1].y + V[y2 * w + x2].y) / 3.0) /
      2.0
    );
  };
  for (let i = 0; i < h - 1; i++) {
    for (let j = 0; j < w; j++) {
      I[k++] = (i + 1) * w + j;
      I[k++] = i * w + j;
    }
    if (i != h - 2) I[k++] = -1;
  }
  primitive.buffer.in_pos = V;
  primitive.buffer.index = I;
  primitive.buffer.in_normal = sys.autoNormals(V, I, "tristrip");
  new sys.createprimitive(primitive);
  return primitive;
}
export function primitiveCreateSky(file) {
  let primitive = {};
  primitive.file = "sky";
  primitive.type = "trimesh";
  primitive.texture = {
    cubemap: [
      file + "/PosX.png",
      file + "/NegX.png",
      file + "/PosY.png",
      file + "/NegY.png",
      file + "/PosZ.png",
      file + "/NegZ.png",
    ],
  };
  primitive.buffer = {
    in_pos: [-1, -1, 0, -1, 1, 0, 1, -1, 0, 1, 1, 0],
    index: [0, 1, 2, 1, 2, 3],
  };
  primitive.uniform = {
    materialBuffer: sys.mtlGetDef(),
    cameraBuffer: () => {
      return [
        [...sys.loc.array(), sys.projdist],
        [...sys.dir.array(), sys.projsize],
        [...sys.right.array(), sys.w],
        [...sys.up.array(), sys.h],
        [sys.localTime, primitive.texture.cubemap.value, sys.wp, sys.hp],
      ];
    },
  };
  new sys.createprimitive(primitive);
  return primitive;
}
export function primitiveCreateCloth(w, h) {
  let optimallength = 5,
    j,
    i,
    v,
    w1,
    w2,
    lenv,
    length,
    k;
  let primitive = {};
  this.V = [];
  this.I = [];
  primitive.file = "cloth";
  primitive.buffer = {};
  let width = ((w - 1.0) * optimallength) / 2.0;
  let hieght = ((h - 1.0) * optimallength) / 2.0;
  k = 4;
  this.V[0] = sys.vec3(-width, 10.0, -hieght);
  this.V[0].weight = Math.random();
  this.I[0] = [0, w];
  this.I[1] = [0, 1];
  for (j = 1; j < w - 3; j++) {
    this.V[j] = sys.vec3(j * optimallength - width, 10.0, -hieght);
    this.V[j].weight = Math.random();
    this.I[k++] = [j, w + j];
    this.I[k++] = [j, j + 1];
  }
  this.V[w - 2] = sys.vec3((w - 2) * optimallength - width, 10.0, -hieght);
  this.V[w - 2].weight = Math.random();
  this.I[k++] = [w - 2, w + w - 2];
  this.I[2] = [w - 2, w - 1];

  this.V[w - 1] = sys.vec3((w - 1) * optimallength - width, 10.0, -hieght);
  this.V[w - 1].weight = Math.random();
  this.I[3] = [w - 1, w + w - 1];
  for (i = 1; i < h - 1; i++) {
    for (j = 0; j < w; j++) {
      this.V[i * w + j] = sys.vec3(
        j * optimallength - width,
        10.0,
        i * optimallength - hieght
      );
      this.V[i * w + j].weight = Math.random();
      this.I[k++] = [i * w + j, (i + 1) * w + j];
      this.I[k++] = [i * w + j, i * w + j + 1];
    }
    k--;
  }
  i = h - 1;
  for (let j = 0; j < w - 1; j++) {
    this.V[i * w + j] = sys.vec3(
      j * optimallength - width,
      10.0,
      i * optimallength - hieght
    );
    this.V[i * w + j].weight = Math.random();
    this.I[k++] = [i * w + j, i * w + j + 1];
  }
  this.V[h * w - 1] = sys.vec3(
    (w - 1) * optimallength - width,
    10.0,
    (h - 1) * optimallength - hieght
  );
  this.V[h * w - 1].weight = Math.random();
  primitive.apply = () => {
    for (j = 1; j < w - 1; j++) {
      this.V[j].y -= this.V[j].weight;
    }
    for (i = 1; i < h; i++) {
      for (j = 0; j < w; j++) {
        this.V[i * w + j].y -= this.V[i * w + j].weight;
      }
    }
    for (k = 0; k < 2; k++) {
      v = sys.subVec(this.V[this.I[k][1]], this.V[this.I[k][0]]);
      length = v.length();
      lenv = length - optimallength;
      v.normalize();
      if (length < optimallength) {
        lenv = -lenv;
      }
      w1 = this.V[this.I[k][1]].weight;
      this.V[this.I[k][1]] = sys.addVec(this.V[this.I[k][1]], v.mul(-lenv));
      this.V[this.I[k][1]].weight = w1;
    }
    k = 2;
    v = sys.subVec(this.V[this.I[k][1]], this.V[this.I[k][0]]);
    length = v.length();
    lenv = length - optimallength;
    v.normalize();
    if (length < optimallength) {
      lenv = -lenv;
    }
    w1 = this.V[this.I[k][0]].weight;
    this.V[this.I[k][0]] = sys.addVec(this.V[this.I[k][0]], v.mul(lenv));
    this.V[this.I[k][0]].weight = w1;
    k = 3;
    v = sys.subVec(this.V[this.I[k][1]], this.V[this.I[k][0]]);
    length = v.length();
    lenv = length - optimallength;
    v.normalize();
    if (length < optimallength) {
      lenv = -lenv;
    }
    w1 = this.V[this.I[k][1]].weight;
    this.V[this.I[k][1]] = sys.addVec(this.V[this.I[k][1]], v.mul(-lenv));
    this.V[this.I[k][1]].weight = w1;

    for (let k = 4; k < this.I.length; k++) {
      v = sys.subVec(this.V[this.I[k][1]], this.V[this.I[k][0]]);
      length = v.length();
      lenv = (length - optimallength) / 2;
      v.normalize();
      if (length < optimallength) {
        lenv = -lenv;
      }
      w1 = this.V[this.I[k][0]].weight;
      w2 = this.V[this.I[k][1]].weight;
      this.V[this.I[k][0]] = sys.addVec(this.V[this.I[k][0]], v.mul(-lenv));
      this.V[this.I[k][1]] = sys.addVec(this.V[this.I[k][1]], v.mul(lenv));
      this.V[this.I[k][0]].weight = w1;
      this.V[this.I[k][1]].weight = w2;
    }
    //for (let i = 0; i < h - 1; i++) {
    //  for (let j = 0; j < w - 1; j++) {
    //    let v = sys.subVec(this.V[i * w + j + 1], this.V[i * w + j]);
    //    let length = v.length();
    //    let lenv = (length - optimallength) / 2;
    //    v.normalize();
    //    if (length < optimallength) {
    //      sys.addVec(this.V[i * w + j], v.mul(-lenv));
    //      sys.addVec(this.V[i * w + j + 1], v.mul(lenv));
    //    } else if (length > optimallength) {
    //      sys.addVec(this.V[i * w + j], v.mul(lenv));
    //      sys.addVec(this.V[i * w + j + 1], v.mul(-lenv));
    //    }
    //    v = sys.subVec(this.V[(i + 1) * w + j], this.V[i * w + j]);
    //    length = v.length();
    //    lenv = (length - optimallength) / 2;
    //    v.normalize();
    //    if (length < optimallength) {
    //      sys.addVec(this.V[i * w + j], v.mul(-lenv));
    //      sys.addVec(this.V[(i + 1) * w + j], v.mul(lenv));
    //    } else if (length > optimallength) {
    //      sys.addVec(this.V[i * w + j], v.mul(lenv));
    //      sys.addVec(this.V[(i + 1) * w + j], v.mul(-lenv));
    //    }
    //  }
    //  let j = w - 1;
    //  let v = sys.subVec(this.V[(i + 1) * w + j], this.V[i * w + j]);
    //  let length = v.length();
    //  let lenv = (length - optimallength) / 2;
    //  v.normalize();
    //  if (length < optimallength) {
    //    sys.addVec(this.V[i * w + j], v.mul(-lenv));
    //    sys.addVec(this.V[(i + 1) * w + j], v.mul(lenv));
    //  } else if (length > optimallength) {
    //    sys.addVec(this.V[i * w + j], v.mul(lenv));
    //    sys.addVec(this.V[(i + 1) * w + j], v.mul(-lenv));
    //  }
    //}
    //let i = h - 1;
    //for (let j = 0; j < w - 1; j++) {
    //  let v = sys.subVec(this.V[i * w + j + 1], this.V[i * w + j]);
    //  let length = v.length();
    //  let lenv = (length - optimallength) / 2;
    //  v.normalize();
    //  if (length < optimallength) {
    //    sys.addVec(this.V[i * w + j], v.mul(-lenv));
    //    sys.addVec(this.V[i * w + j + 1], v.mul(lenv));
    //  } else if (length > optimallength) {
    //    sys.addVec(this.V[i * w + j], v.mul(lenv));
    //    sys.addVec(this.V[i * w + j + 1], v.mul(-lenv));
    //  }
    //}
  };
  primitive.uniform = {
    materialBuffer: sys.mtlGetDef(),
    cameraBuffer: () => {
      return [
        [...sys.loc.array(), 1],
        [...sys.at.array(), 1],
        [...sys.dir.array(), 1],
        [...sys.right.array(), sys.startelevator],
        [...sys.up.array(), sys.startazimuth],
        [sys.localTime, sys.globalDeltaTime, sys.azimuth, sys.elevator],
      ];
    },
    clothBuffer: () => {
      let massiv = [];
      let k = 0;
      for (let i = 0; i < h - 1; i++) {
        for (let j = 0; j < w - 1; j++) {
          massiv[k++] = [
            this.V[i * w + j].x,
            this.V[i * w + j].y,
            this.V[i * w + j].z,
            1,
          ];
          massiv[k++] = [
            this.V[(i + 1) * w + j].x,
            this.V[(i + 1) * w + j].y,
            this.V[(i + 1) * w + j].z,
            1,
          ];
          massiv[k++] = [
            this.V[i * w + j + 1].x,
            this.V[i * w + j + 1].y,
            this.V[i * w + j + 1].z,
            1,
          ];
          massiv[k++] = [
            this.V[(i + 1) * w + j + 1].x,
            this.V[(i + 1) * w + j + 1].y,
            this.V[(i + 1) * w + j + 1].z,
            1,
          ];
        }
      }
      return massiv;
    },
  };
  let pos = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1];
  let index = [0, 1, 2, 3];
  primitive.buffer.in_pos = pos;
  primitive.buffer.index = index;
  primitive.instance = (w - 1) * (h - 1);
  sys.createprimitive(primitive);
  primitive.updateShader((h - 1) * (w - 1) * 4);
  return primitive;
}
