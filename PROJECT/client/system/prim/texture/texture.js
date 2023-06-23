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
export class cube_texture {
  constructor(name, loc) {
    this.loc = loc;
    this.id = gl.createTexture();
    this.value = 0;
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, this.id);
    const faceInfos = [
      {
        target: gl.TEXTURE_CUBE_MAP_POSITIVE_X,
        url: name[0],
      },
      {
        target: gl.TEXTURE_CUBE_MAP_NEGATIVE_X,
        url: name[1],
      },
      {
        target: gl.TEXTURE_CUBE_MAP_POSITIVE_Y,
        url: name[2],
      },
      {
        target: gl.TEXTURE_CUBE_MAP_NEGATIVE_Y,
        url: name[3],
      },
      {
        target: gl.TEXTURE_CUBE_MAP_POSITIVE_Z,
        url: name[4],
      },
      {
        target: gl.TEXTURE_CUBE_MAP_NEGATIVE_Z,
        url: name[5],
      },
    ];
    this.countTextures = 0;
    faceInfos.forEach((faceInfo) => {
      const { target, url } = faceInfo;
      // setup each face so it's immediately renderable
      gl.texImage2D(
        target,
        0,
        gl.RGBA,
        1,
        1,
        0,
        gl.RGBA,
        gl.UNSIGNED_BYTE,
        new Uint8Array([255, 255, 255, 0])
      );
      // Asynchronously load an image
      const image = new Image();
      image.src = url;
      this.countTextures++;
      image.onload = () => {
        this.countTextures--;
        gl.bindTexture(gl.TEXTURE_CUBE_MAP, this.id);
        gl.texImage2D(target, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
        //gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
        if (!this.countTextures) {
          gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
          this.value = 1;
        }
      };
    });
    //gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
    gl.texParameteri(
      gl.TEXTURE_CUBE_MAP,
      gl.TEXTURE_MIN_FILTER,
      gl.LINEAR_MIPMAP_LINEAR
    );
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.CLAMP_TO_EDGE);
  }
  apply(texUnit) {
    gl.activeTexture(gl.TEXTURE0 + texUnit);
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, this.id);
    gl.uniform1i(this.loc, texUnit);
  }
}
