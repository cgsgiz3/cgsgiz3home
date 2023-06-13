/* eslint-disable no-undef */
class texture {
  constructor(name, type, loc) {
    this.type = type;
    this.id = gl.createTexture();
    gl.bindTexture(this.type, this.id);
    gl.texImage2D(
      this.type,
      0,
      gl.RGBA,
      1,
      1,
      0,
      gl.RGBA,
      gl.UNSIGNED_BYTE,
      new Uint8Array([255, 255, 255, 0])
    );
    this.img = new Image();
    this.img.src = name;
    this.loc = loc;
    this.value = 0;
  }
  apply(texUnit) {
    gl.activeTexture(gl.TEXTURE0 + texUnit);
    gl.bindTexture(this.type, this.id);
    gl.uniform1i(this.loc, texUnit);
  }
}
export class texture_2D extends texture {
  constructor(name, loc) {
    super(name, gl.TEXTURE_2D, loc);
    this.img.onload = () => {
      gl.texImage2D(
        gl.TEXTURE_2D,
        0,
        gl.RGBA,
        gl.RGBA,
        gl.UNSIGNED_BYTE,
        this.img
      );
      gl.generateMipmap(gl.TEXTURE_2D);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
      gl.texParameteri(
        gl.TEXTURE_2D,
        gl.TEXTURE_MIN_FILTER,
        gl.LINEAR_MIPMAP_LINEAR
      );
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
      this.value = 1;
    };
    gl.texImage2D(
      gl.TEXTURE_2D,
      0,
      gl.RGBA,
      gl.RGBA,
      gl.UNSIGNED_BYTE,
      this.img
    );
  }
  apply = super.apply;
}
export class create_texture {
  constructor(type, loc, width, height) {
    this.width = width;
    this.type = type;
    this.height = height;
    this.loc = loc;
    this.id = gl.createTexture();
    gl.bindTexture(this.type, this.id);
    gl.texImage2D(
      this.type,
      0, // mip level
      gl.RGBA, // internal format
      width, // width
      height, // height
      0, // border
      gl.RGBA, // format
      gl.UNSIGNED_BYTE, // type
      null // data
    );
    gl.generateMipmap(this.type);
    gl.texParameteri(this.type, gl.TEXTURE_WRAP_S, gl.REPEAT);
    gl.texParameteri(this.type, gl.TEXTURE_WRAP_T, gl.REPEAT);
    gl.texParameteri(this.type, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
    gl.texParameteri(this.type, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  }
}
export class create_texture_2D extends create_texture {
  constructor(loc, width, height) {
    super(gl.TEXTURE_2D, loc, width, height);
  }
}
let frameBuffer = 0;
export class create_frameBuffer extends create_texture_2D {
  constructor(loc, width, height) {
    super(loc, width, height);
    this.fb = gl.createFramebuffer();
  }
  turnon() {
    gl.bindFramebuffer(gl.FRAMEBUFFER, this.fb);
    gl.framebufferTexture2D(
      gl.FRAMEBUFFER,
      gl.COLOR_ATTACHMENT0 + frameBuffer,
      gl.TEXTURE_2D,
      this.id,
      0
    );
    frameBuffer++;
  }
  turnoff() {
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    frameBuffer--;
  }
  apply(texUnit) {
    gl.activeTexture(gl.TEXTURE0 + texUnit);
    gl.bindTexture(this.type, this.id);
    gl.uniform1i(this.loc, texUnit);
  }
}
