/* eslint-disable no-undef */
import frag from "./shaders/frag.glsl"
import vert from "./shaders/vert.glsl"
function loadshader(type, source) {
  const shader = gl.createShader(type);

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    console.log(`dont't load shader`);
  }
  return shader;
}
export function createshader() {
  const vertexShader = loadshader(gl.VERTEX_SHADER, vert);
  const fragmentShader = loadshader(gl.FRAGMENT_SHADER, frag);
  const shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);
  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    const Buf = gl.getProgramInfoLog(shaderProgram);
    alert(Buf);
    reject(null);
  }
  // Fill shader attributes info
  let shaderatr = {};
  shaderatr.attrs = {};
  shaderatr.uniformBlocks = {};
  let countAttr = gl.getProgramParameter(
    shaderProgram,
    gl.ACTIVE_ATTRIBUTES
  );
  for (let i = 0; i < countAttr; i++) {
    const info = gl.getActiveAttrib(shaderProgram, i);
    shaderatr.attrs[info.name] = {
      name: info.name,
      type: info.type,
      size: info.size,
      loc: gl.getAttribLocation(shaderProgram, info.name),
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
  return [shaderProgram, shaderatr];
}
