import { uniform_buffer, vertex_buffer, index_buffer } from "./buffer.js";
import { createshader } from "./shader.js";

function takemassiv(obj) {
  if (Array.isArray(obj)) {
    let massiv = [];
    obj.forEach((res) => {
      if (Array.isArray(res)) {
        massiv.push(...res);
      } else {
        massiv.push(...takemassiv(res));
      }
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
  primitiveEvalBB(primitive, primitive.buffer.in_pos);
  primitive.draw = (mtr = sys.mat4()) => {
    gl.useProgram(this.shaderProgram);
    primitive.frameBuffer.update(takemassiv(sys.mulMatr(mtr, sys.vp)));
    primitive.frameBuffer.apply(this.shaderProgram);
    for (let property in primitive.uniformBlocks) {
      primitive.uniformBlocks[property].update(
        takemassiv(primitive.uniform[property])
      );
      primitive.uniformBlocks[property].apply(this.shaderProgram);
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
  let res = createshader(primitive.file);
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
  primitive.frameBuffer = {};
  primitive.uniformBlocks = {};
  for (let property in res[1].uniformBlocks) {
    property = res[1].uniformBlocks[property];
    if (property.name === "frameBuffer") {
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
}
export function primitiveCreateGrid(w = 100, h = 100) {
  let HeightFieldSize = 100000.0,
    primitive = {};
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
  for (let i = 0; i < h; i++) {
    for (let j = 0; j < w; j++) {
      V[i * w + j] = sys.vec3(
        (HeightFieldSize * j) / (w - 1.0) - HeightFieldSize / 2,
        0,
        HeightFieldSize * (1 - i / (h - 1.0)) - HeightFieldSize / 2
      );
      N[i * w + j] = sys.vec3(0, 1, 0);
    }
  }
  for (let i = 0; i < h - 1; i++) {
    for (let j = 0; j < w; j++) {
      I[k++] = (i + 1) * w + j;
      I[k++] = i * w + j;
    }
    if (i != h - 2) I[k++] = -1;
  }
  primitive.buffer.in_pos = V;
  primitive.buffer.index = I;
  primitive.buffer.in_normal = N;
  new sys.createprimitive(primitive);
  return primitive;
}
