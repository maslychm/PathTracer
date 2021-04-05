# PathTracer
Path tracer using C++

![gallery](/images/thumbs.png)

## What is this
Lighting Path tracer implemented by following Peter Shirley's [Ray Tracing in One Weekend Book Series](https://raytracing.github.io/).  

Originally I wanted to use this code to render a high resolution avatar similar to one I made in Unity some time ago, but it turned out that I had great interest in the inner workings of a Ray Tracer, so ended up following a book and making my changes to the code. The avatar is one of the scenes in the image above.  
This repository is always work in progress. Sometimes when I have free time, I sit down to continue following the book and adding new features 🙂.

## Features

Custom changes: 
- adding `scene` class that contains initialization of `hittalbe` objects and sets appropriate image format depending on scene 
- thread pooling: minimize downtime by rendering lines concurrently
- during render, enter p to generate preview

From the book:
- Materials:
    - Lambertian, Metal, Dielectric, Diffuse-light, Isotropic
    - Textures: procedural, image mapping
- Hittables:
    - Sphere
    - Rectangle
    - Box (collection of 6 rectangles)
- Other:
    - Defocus Blur
    - Perlin noise (+ turbulence)
    - Threading

## External Resources
   For image loading (to use in textures) [stb](https://github.com/nothings/stb) was used.  
   For thread pooling implementation, I used [ThreadPool](https://github.com/progschj/ThreadPool) repository  
