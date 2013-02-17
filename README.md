raytracer
=========

A ray tracer that supports various primitives, area lighting, and certain dielectrics.


Features
--------
* Supports many primitives: Boxes (with rotation, scaling, translation), triangles, planes, spheres
* Parallelogram area lights with a variable number of samples along each axis.
 * Soft shadowing is implemented using a stratified, stochastic approach (i.e., stratified Monte Carlo method); a grid is formed over the parallelogram using the number of samples given for each axis, and a random sample is then taken for each cell. Objects' diffuse/specular are still computed as if the area light were a point light, however. (This is more of a problem for specular highlights.)
* Refraction / dielectric media. By specifying an object with a refractive index != 0, the object is then considered to be a dense dielectric, and as such it transmits light (with its inside media having the refractive index given). Beer's law (to account for absorption) hasn't been implemented yet.
* Antialiasing via supersampling using a simple grid-based approach. The number of samples per row and column is defined in the scene file.
* OpenMP to parallelize rows of rays. Using a tile-based approach may works better (due to memory locality), but without an acceleration structure it probably isn't a huge concern.

Scene files
-----------
The following is a specification of the scene input files. See `scenes/` for examples. The first block are the initial features that I implemented; by writing `ENABLEEXTRAFEATURES` on a line, the second block of features opens for use. `float01` means a `float` that ranges from 0 to 1; similarly, `vector01` means a `vector` with components ranging from 0 to 1.

    NEAR <float: near plane>
    LEFT <float: left plane>
    RIGHT <float: right plane>
    BOTTOM <float: bottom plane>
    TOP <float: top plane>
    RES <int: x resolution> <int: y resolution>
    SPHERE <string: name> <vector: position> <vector: scale> <vector01: RGB color> <float01: ambient coefficient> <float01: diffuse coefficient> <float01: specular coefficient> <float01: reflectivity (if not dielectric)> <float: specular exponent>
    LIGHT <string: name> <vector: position> <vector01: RGB color>

    ENABLEEXTRAFEATURES
    AASAMPLES <integer: number of samples per pixel is AASAMPLES^2>
    MAXDEPTH <integer above 0: maximum recursion depth>
    REFRACTIVEINDEX <float: ambient refractive index>
    BIAS <float: bias (or epsilon) value used in ray tracing>
    BOX <string: name> <vector: position> <vector: scale> <vector: rotation, in degrees, along X, Y, Z> < ... material properties ... > <float: refractive index>
    SPHERE <... normal parameters ...> <refractive index>
    PLANE <string: name> <vector: normal> <float: distance> <... material properties (same as sphere, box, etc.) ... >
    TRIANGLE <name> <vector: vertex 1> <vector: vertex 2> <vector: vertex 3> <... material properties ...>
    AREALIGHT <string: name> <vector: position (center)> <vector: axis1> <vector: axis2> <float: axis1 length> <float: axis2 length> <int above 0: axis1 samples> <int above 0: axis2 samples> <vector01: RGB color>
