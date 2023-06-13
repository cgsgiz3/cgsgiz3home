/* eslint-disable no-unused-vars */
/* eslint-disable no-undef */

export class buffer {
  constructor(type, size) {
    this.type = type;
    this.id = gl.createBuffer();
    gl.bindBuffer(this.type, this.id);
    gl.bufferData(this.type, size, gl.STATIC_DRAW);
  }
  update(data) {
    gl.bindBuffer(this.type, this.id);
    gl.bufferSubData(this.type, 0, data, 0);
  }
  apply() {
    gl.bindBuffer(this.type, this.id);
  }
}
export class uniform_buffer extends buffer {
  constructor(name, index, bind, size) {
    super(gl.UNIFORM_BUFFER, size);
    this.name = name;
    this.index = index;
    this.bind = bind;
  }
  update(data) {
    super.update(new Float32Array(data));
  }
  apply(shaderProgram) {
    super.apply();
    gl.uniformBlockBinding(shaderProgram, this.index, this.bind);
    gl.bindBufferBase(gl.UNIFORM_BUFFER, this.bind, this.id);
  }
}
export class vertex_buffer extends buffer {
  constructor(vertices) {
    super(gl.ARRAY_BUFFER, new Float32Array(vertices));
    this.numOfVertices = vertices.length;
  }
  update(data) {
    super.update(new Float32Array(data));
  }
  apply = super.apply;
}
export class index_buffer extends buffer {
  constructor(indices) {
    super(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices));
    this.numOfIndices = indices.length;
  }
  update(data) {
    super.update(new Uint32Array(data));
  }
  apply = super.apply;
}
