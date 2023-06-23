/* eslint-disable no-undef */
import cowfrag from "./shaders/cow/frag.glsl";
import cowvert from "./shaders/cow/vert.glsl";
import gridfrag from "./shaders/grid/frag.glsl";
import gridvert from "./shaders/grid/vert.glsl";
import skyfrag from "./shaders/sky/frag.glsl";
import skyvert from "./shaders/sky/vert.glsl";
import treefrag from "./shaders/tree/frag.glsl";
import treevert from "./shaders/tree/vert.glsl";
import clothfrag from "./shaders/cloth/frag.glsl";
import clothvert from "./shaders/cloth/vert.glsl";

function loadshader(type, source) {
  const shader = gl.createShader(type);

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    console.log(`dont't load shader`);
  }
  return shader;
}
export function createshader(name) {
  let frag, vert;
  let updateShader = () => {};
  if (name === "cow") {
    frag = cowfrag;
    vert = cowvert;
    updateShader = (instance) => {
      let newvert = vert.split("\n");
      newvert[1] = `#define NUM_OF_MATRIX ${instance}`;
      vert = newvert.join("\n");
      const vertexShader = loadshader(gl.VERTEX_SHADER, vert);
      const fragmentShader = loadshader(gl.FRAGMENT_SHADER, frag);
      const shaderProgram = gl.createProgram();
      gl.attachShader(shaderProgram, vertexShader);
      gl.attachShader(shaderProgram, fragmentShader);
      gl.linkProgram(shaderProgram);
      if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        const Buf = gl.getProgramInfoLog(shaderProgram);
        alert(Buf);
      }
      return shaderProgram;
    };
  } else if (name === "grid") {
    frag = gridfrag;
    vert = gridvert;
  } else if (name === "sky") {
    frag = skyfrag;
    vert = skyvert;
  } else if (name === "tree") {
    frag = treefrag;
    vert = treevert;
  } else if (name === "cloth") {
    frag = clothfrag;
    vert = clothvert;
    updateShader = (count) => {
      let newvert = vert.split("\n");
      newvert[2] = `#define NUM_OF_CLOTH ${count}`;
      vert = newvert.join("\n");
      const vertexShader = loadshader(gl.VERTEX_SHADER, vert);
      const fragmentShader = loadshader(gl.FRAGMENT_SHADER, frag);
      const shaderProgram = gl.createProgram();
      gl.attachShader(shaderProgram, vertexShader);
      gl.attachShader(shaderProgram, fragmentShader);
      gl.linkProgram(shaderProgram);
      if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        const Buf = gl.getProgramInfoLog(shaderProgram);
        alert(Buf);
      }
      return shaderProgram;
    };
  } else {
    alert("I don't know shader name");
  }
  const vertexShader = loadshader(gl.VERTEX_SHADER, vert);
  const fragmentShader = loadshader(gl.FRAGMENT_SHADER, frag);
  const shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);
  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    const Buf = gl.getProgramInfoLog(shaderProgram);
    alert(Buf);
  }
  // Fill shader attributes info
  let shaderatr = {};
  shaderatr.attrs = {};
  shaderatr.uniforms = {};
  shaderatr.uniformBlocks = {};
  let countAttr = gl.getProgramParameter(shaderProgram, gl.ACTIVE_ATTRIBUTES);
  for (let i = 0; i < countAttr; i++) {
    const info = gl.getActiveAttrib(shaderProgram, i);
    shaderatr.attrs[info.name] = {
      name: info.name,
      type: info.type,
      size: info.size,
      loc: gl.getAttribLocation(shaderProgram, info.name),
    };
  }
  let countUniform = gl.getProgramParameter(shaderProgram, gl.ACTIVE_UNIFORMS);
  for (let i = 0; i < countUniform; i++) {
    const info = gl.getActiveUniform(shaderProgram, i);
    let Loc = gl.getUniformLocation(shaderProgram, info.name);
    shaderatr.uniforms[info.name] = {
      name: info.name,
      type: info.type,
      size: info.size,
      loc: Loc,
    };
  }

  // Fill shader uniform blocks info
  let countUniformBlocks = gl.getProgramParameter(
    shaderProgram,
    gl.ACTIVE_UNIFORM_BLOCKS
  );
  for (let i = 0; i < countUniformBlocks; i++) {
    const info = gl.getActiveUniformBlockName(shaderProgram, i);
    const idx = gl.getUniformBlockIndex(shaderProgram, info);
    const bind = gl.getActiveUniformBlockParameter(
      shaderProgram,
      idx,
      gl.UNIFORM_BLOCK_BINDING
    );
    shaderatr.uniformBlocks[info] = {
      name: info,
      index: idx,
      size: gl.getActiveUniformBlockParameter(
        shaderProgram,
        idx,
        gl.UNIFORM_BLOCK_DATA_SIZE
      ),
      bind: bind,
    };
  }

  /*
  gl.detachShader(shaderProgram, vertexShader);
  gl.deleteShader(vertexShader);
  gl.detachShader(shaderProgram, fragmentShader);
  gl.deleteShader(fragmentShader);
  */
  return [shaderProgram, shaderatr, updateShader];
}
