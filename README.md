# Project Leemur

A simulator based on Procedural generation of terrain, buildings, and plants. Procedural terrain uses Fractal Brownian motion with Perlin and Value noise for seeding; procedural plants uses L-Systems for generating plants utilizing grammar and seed placements; and procedural buildings following simple shape grammar rules to determine which model and their size based on seeding as well. 

Water effects include reflection and refraction of the skybox, with wave animations utilizing coroutines as to achieve asynchronous updates. Once all the calculations for that coroutine has finished, it will alert the World System and when all grids are done, the World System will alert each individual chunk to update the mesh data containing the vertices for the waves. This is done 60x a second. (Note: wave animations are only available in wave-anim-merge branch, as coroutines are an experimental C++ compiler option utilizing '/await' option). Water can be lowered as well, via adjusting the mesh once again similarly to how coroutine adjust for wave motion.

Low-poly graphics was used to implement the whole terrain utilizing a 0.5 unit length shift to create more jagged triangles in the terrain. If one looks closely near the edges, they look jagged; this is caused by the shift, and without it, the terrain would look grid-like. Toon shading was also added to further the characteristics of Low Poly.

## Click image to watch demonstration
[![Link to video demonstration](https://img.youtube.com/vi/bjaFW2GLRnM/0.jpg)](https://www.youtube.com/watch?v=bjaFW2GLRnM)
