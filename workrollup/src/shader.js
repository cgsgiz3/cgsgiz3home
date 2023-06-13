/* eslint-disable no-undef */
function loadshader(gl, type, source) {
  const shader = gl.createShader(type);

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    console.log(`dont't load shader`);
  }
  return shader;
}
export function createshader(gl, file) {
  let vs, fs;

  return new Promise((resolve, reject) => {
    if (file === "") {
      resolve(0);
      return;
    }
    const ft1 = fetch(`${file}/vert.glsl`)
      .then((res) => res.text()) // Стрелочная безымянная функция запустится после получение текста, из 'res' получаем текстовые данные
      .then((data) => {
        // Стрелочная безымянная функция запустится для обработки полученных на предыдущем этапе данных - 'data' это полученный текст
        vs = data; // Запоминаем полученный текст в глобальной переменной
      });
    const ft2 = fetch(`${file}/frag.glsl`)
      .then((res) => res.text())
      .then((data) => {
        fs = data;
      });
    const allData = Promise.all([ft1, ft2]);
    allData.then(() => {
      // Вызов произойдет, когда все запросы выполнятся
      // Выполняем то, что необходимо после получения файлов
      const vertexShader = loadshader(gl, gl.VERTEX_SHADER, vs);
      const fragmentShader = loadshader(gl, gl.FRAGMENT_SHADER, fs);
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
      shaderatr.uniforms = {};
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
      // Fill shader uniforms info
      let countUniform = gl.getProgramParameter(
        shaderProgram,
        gl.ACTIVE_UNIFORMS
      );
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
      resolve([shaderProgram, shaderatr]);
    });
  });
}
