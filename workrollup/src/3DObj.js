import { autonormals } from "./math.js";
import { takeprimitive, handlecreateprimitive } from "./prim.js";
import { unitHandle } from "./units.js";
export function objLoadSync(fileName = "cow.obj", primitive = null) {
  if (primitive === null) {
    primitive = takeprimitive();
  }
  let V = [],
    I = [];
  unitHandle(
    new Promise((resolve) => {
      fetch(fileName)
        .then((res) => res.text())
        .then((data) => {
          let text = data.split("\n");
          let nv = 0,
            nf = 0;
          for (let i = 4; i < text.length; i++) {
            let massiv = text[i].split(" ");
            if (massiv[0] === "v") {
              for (let j = 1; j < massiv.length; j++) {
                let number = parseFloat(massiv[j]);
                V.push(number);
              }
              nv++;
            } else if (massiv[0] === "f") {
              let n = 0,
                c0 = 0,
                c = 0,
                c1 = 0;
              for (let j = 1; j < massiv.length; j++) {
                c = massiv[j].split("//")[0];
                if (c < 0) {
                  c = nv + c;
                } else {
                  c--;
                }
                if (n == 0) {
                  c0 = c;
                } else if (n == 1) {
                  c1 = c;
                } else {
                  I[nf++] = c0;
                  I[nf++] = c1;
                  I[nf++] = c;
                  c1 = c;
                }
                n++;
              }
            }
          }
          //primitive.type = "tristrip";
          primitive.buffer.in_pos = V;
          primitive.buffer.index = I;
          primitive.buffer.in_normal = autonormals(V, I, "trimesh");
          handlecreateprimitive(primitive).then(() => {
            resolve(0);
          });
        });
    })
  );
  return primitive;
}
//  nf = 0;
//  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
//  {
//    if (Buf[0] == 'v' && Buf[1] == ' ')
//    {
//      DBL x, y, z;
//      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
//      V[nv++].P = VecSet(x, y, z);
//    }
//    else if (Buf[0] == 'f' && Buf[1] == ' ')
//    {
//      int n = 0, c0 = 0, c = 0, c1 = 0;
//      CHAR *ptr = Buf + 2, Old = ' ';
//
//      while (*ptr != 0)
//      {
//        if (*ptr != ' ' && Old == ' ')
//        {
//          sscanf(ptr, "%d", &c);
//          if (c < 0)
//            c = nv + c;
//          else
//            c--;
//          if (n == 0)
//            c0 = c;
//          else if (n == 1)
//            c1 = c;
//          else
//          {
//            I[nf++] = c0;
//            I[nf++] = c1;
//            I[nf++] = c;
//            c1 = c;
//          }
//          n++;
//        }
//        Old = *ptr++;
//      }
//    }
//  }
//  for (i = 0; i < nv; i++)
//    V[i].N = VecSet(0, 0, 0);
//  for (i = 0; i < nf; i += 3)
//  {
//    VEC
//      p0 = V[I[i]].P,
//      p1 = V[I[i + 1]].P,
//      p2 = V[I[i + 2]].P,
//      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));
//    V[I[i]].N = VecAddVec(V[I[i]].N, N);
//    V[I[i + 1]].N = VecAddVec(V[I[i + 1]].N, N);
//    V[I[i + 2]].N = VecAddVec(V[I[i + 2]].N, N);
//  }
//  if (!IZ3_RndPrimCreate(Pr, IZ3_RND_PRIM_TRIMESH, nv, nf, V, I))
//  {
//    fclose(F);
//    return FALSE;
//  }
//  IZ3_RndPrimEvalBB(Pr, V, nv);
//  free(V);
//  fclose(F);
//  return TRUE;
//}
