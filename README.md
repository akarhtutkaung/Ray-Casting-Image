# Ray Casting

@Author: Akar (Ace) Htut Kaung

## Overview

This program use ray casting to determine the appropriate color to write in each pixel of the output image and rendered the image in ascii PPM format as an output.

### Feature
- Phong illumination model
- Texture support
- Transparency
- Mirror reflection

## Usage
Inside the src/ folder:

### Compile
```
make all
```
### Run
```
./raytracer [config file] [output filename]
```
## Showcase
Example1

Example2

Example3

## Config File
What should be in the config file?

### Mandatory

The view origin, also variously referred to as the 'eye position', 'camera position' or 'center of projection' (a 3D point in space).
```
eye   eyex eyey eyez
```

The viewing direction (a 3D vector).
```
viewdir   vdirx  vdiry  vdirz
```

The 'up' direction (a 3D vector).
```
updir   upx  upy  upz
```

The vertical field of view (in degrees).
```
vfov   fovv
```
The size of the output image (in pixel units).
```
imsize   width  height
```

The ‘background’ color (in terms of r, g, b components defined on a scale from 0-1).
```
bkgcolor   r  g  b
```

A ‘material’ color (in terms of r, g, b components defined on a scale from 0-1). <br>
  - \[Odr, Odg, Odb]: Object diffuse color <br>
  - \[Osr, Osg, Osb]: Object specular highligh color<br>
  - \[ka]: reflectivity ambient coefficient <br>
  - \[kd]: reflectivity diffuse coefficient <br>
  - \[ks]: reflectivity specular coefficient  <br>
  - \[n]: reflectivity specular exponent <br>
  - \[α]: material's opacity <br>
  - \[η]: index of refraction <br>
```          
mtlcolor Odr Odg Odb Osr Osg Osb ka kd ks n α η
```

A point light source is defined by a 3D location in space (x, y, z, 1, r, g, b), and a directional light source is defined by a vector (x, y, z, 0, r, g, b). In terms of r, g, b components defined on a scale from 0-1.
```
light x y z w r g b
```

### Optional

A sequence of one or more objects.
```
sphere   cx  cy  cz  r
```

List of vertex positions to handle triangles.
```
v   x1 y1 z1
v   x2 y2 z2
v   x3 y3 z3
v   x4 y4 z4
. . .
```

List of triangle definitions, consisting of appropriate indices into the vertex array, starting at 1 (not 0).
```
f v1 v2  v3
f v1 v2  v4
. . .
```

List of vertex normal vectors.
```
vn   nx1 ny1 nz1
vn   nx2 ny2 nz2
. . .
```

For each smooth shaded triangle, indices into the array of vertex normal directions as well as into the array of vertex positions.
(In this example, v1, v2, v3 are indices into the array of vertex locations and vn1, vn2, vn3 are indices into the array of normal directions.)
```
f   v1//vn1    v2//vn2    v3//vn3
```

Texture mapping.
```
texture ppm
```

List of texture coordinates.
```
vt   u1 v1
vt   u2 v2
. . .
```

When smooth shading is not being applied, use this syntax:
```
f   v1/vt1   v2/vt2   v3/vt3
```

When smooth shading is being applied, use this syntax:
```
f   v1/vt1/vn1     v2/vt2/vn2   v3/vt3/vn3
```