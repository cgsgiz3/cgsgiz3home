/* eslint-disable no-undef */
/*
/* eslint-disable no-undef */
//import * as mth from "./math.js";
// eslint-disable-next-line no-unused-vars
//import "./main.js";
import {
  texture_2D,
  create_texture_2D,
  create_frameBuffer,
} from "./texture.js";
import { Camera } from "./camera.js";
import { unitHandle } from "./units.js";
import { uniform_buffer, vertex_buffer, index_buffer } from "./buffer.js";
import { createshader } from "./shader.js";
import { mtlGetDef } from "./material.js";
import { Timer } from "./timer.js";

function newprim() {
  return this;
}
export function takeprimitive() {
  let prim = new newprim();
  prim.idCanvas = "glCanvas";
  prim.file = "./icosahedron";
  prim.type = "trimesh";
  prim.buffer = {};
  prim.uniform = {
    materialBuffer: mtlGetDef(),
    frameBuffer: () => {
      return Camera.vp.massiv;
    },
    cameraBuffer: () => {
      return [
        [...Camera.loc.array(), 1],
        [...Camera.dir.array(), 1],
        [...Camera.right.array(), 1],
        [...Camera.up.array(), 1],
        [Timer.localTime, Timer.globalDeltaTime, 1, 1],
      ];
    },
  };
  return prim;
}
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
function gettype(gl, type) {
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
function newprimitive(primitive = null) {
  //-------------------------------------------------------------------------------------------------------------------------------------------------
  if (primitive === null) {
    console.log("You take me null primitive");
    return;
  }
  if (primitive.file === undefined) {
    console.log("Primitive's file absent");
    return;
  }
  if (primitive.uniform === undefined) {
    primitive.uniform = {};
  }
  if (primitive.buffer === undefined) {
    primitive.buffer = {};
  }
  if (primitive.texture === undefined) {
    primitive.texture = {};
  }
  //if (primitive.buffer === undefined) {primitive.buffer = {};}

  //-------------------------------------------------------------------------------------------------------------------------------------------------
  let drawprimitive = () => {
    gl = primitive.idCanvas;
    gl.useProgram(this.shaderProgram);
    const primUniform = () => {
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
    };
    primUniform();
    gl.bindVertexArray(this.vertexArray);

    if (primitive.inctance < 2) {
      if (primitive.buffer["index"] !== undefined) {
        primitive.buffer["index"].apply();
        gl.drawElements(
          primitive.type,
          primitive.numOfVerices,
          gl.UNSIGNED_SHORT,
          0
        );
      } else {
        gl.drawArrays(primitive.type, 0, primitive.numOfVerices);
      }
    } else {
      if (primitive.buffer["index"] !== undefined) {
        primitive.buffer["index"].apply();
        gl.drawElementsInstanced(
          primitive.type,
          primitive.numOfVerices,
          gl.UNSIGNED_SHORT,
          0,
          primitive.inctance
        );
      } else {
        gl.drawArraysInstanced(
          primitive.type,
          0,
          primitive.numOfVerices,
          primitive.inctance
        );
      }
    }
  };
  primitive.draw = drawprimitive;
  return createshader(gl, primitive.file).then((res) => {
    if (typeof primitive.inctance !== "number") {
      primitive.inctance = 1;
    }
    gl = primitive.idCanvas;
    primitive.type = gettype(gl, primitive.type);
    gl = primitive.idCanvas;
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
        let texturemassiv = primitive.texture[property].split("(");
        if (texturemassiv[0] === "create_frameBuffer") {
          let w = parseFloat(texturemassiv[1]);
          let h = parseFloat(texturemassiv[1]);
          if (typeof w === "number" && h === "number") {
            primitive.texture[property] = new create_frameBuffer(
              res[1].uniforms[property].loc,
              w,
              h
            );
          }
        } else if (texturemassiv[0] === "create_texture_2D") {
          let w = parseFloat(texturemassiv[1]);
          let h = parseFloat(texturemassiv[1]);
          if (typeof w === "number" && h === "number") {
            primitive.texture[property] = new create_texture_2D(
              res[1].uniforms[property].loc,
              w,
              h
            );
          }
        }
        primitive.texture[property] = new texture_2D(
          primitive.texture[property],
          res[1].uniforms[property].loc
        );
      }
    }
    //gl.bindVertexArray(null);
  });
}
export function createprimitive(primitive) {
  unitHandle(new newprimitive(primitive));
}
export function handlecreateprimitive(primitive) {
  return new newprimitive(primitive);
}
///if (typeof material === "object" && material !== null) {
///  let materialBuffer = gl.createBuffer();
///  let frameUniformBufferMaterialIndex = 2;
///  gl.bindBuffer(gl.UNIFORM_BUFFER, materialBuffer);
///  const materialelements = ["Ka", "Trans", "Kd", "Ks", "Ph"];
///  buf = [];
///  for (let i = 0; i < materialelements.length; i++) {
///    let property = materialelements[i];
///
///    if (primitive.material[property] !== undefined) {
///      if (primitive.material[property].length === 1) {
///        buf.push(
///          primitive.material[property],
///          primitive.material[property],
///          primitive.material[property]
///        );
///      } else if (primitive.material[property].length === 2) {
///        buf.push(...primitive.material[property], 1);
///      } else if (primitive.material[property].length === 3) {
///        buf.push(...primitive.material[property]);
///      } else if (typeof primitive.material[property] === "number") {
///        buf.push(primitive.material[property]);
///      }
///      if (i === 2) {
///        buf.push(1);
///      }
///    }
///  }
///  gl.bufferData(
///    gl.UNIFORM_BUFFER,
///    new Float32Array(buf),
///    gl.STATIC_DRAW
///  );
///  gl.bufferSubData(
///    gl.UNIFORM_BUFFER,
///    new Float32Array(buf),
///    gl.STATIC_DRAW
///  );
///  gl.uniformBlockBinding(
///    primitive.shaderProgram,
///    gl.getUniformBlockIndex(
///      primitive.shaderProgram,
///      "materialBuffer"
///    ),
///    frameUniformBufferMaterialIndex
///  );
///  gl.bindBufferBase(
///    gl.UNIFORM_BUFFER,
///    frameUniformBufferMaterialIndex,
///    primitive.materialBuffer
///  );
///}
///if (typeof primitive.uniform === "object" && primitive.uniform !== null) {
///  for (let property in uniform) {
///    let outuniform;
///    if (typeof primitive.uniform[property] === "function") {
///      outuniform = primitive.uniform[property]();
///    } else if (typeof primitive.uniform[property] === "number") {
///      outuniform = primitive.uniform[property];
///    }
///    let informLoc = gl.getUniformLocation(
///      primitive.shaderProgram,
///      property
///    );
///    gl.uniform1f(informLoc, outuniform);
///  }
///}
///
