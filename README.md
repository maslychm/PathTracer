# PathTracer
Path tracer using C++

![part_one_final_scene](/images/image3.png)

## What is this
Path tracer implemented by following Peter Shirley's [Ray Tracing in One Weekend Book Series](https://raytracing.github.io/). This repository is work in progress. Sometimes when I have free time, I sit down to continue following the book and adding new features 🙂.

### Features covered:  
Book 1: 100%  
Book 2: 100%  
Book 3:  0%  

- Materials:
    - Lambertian
    - Metal
    - Dielectric
    - Procedural Texture
    - Image Texure
    - Diffuse-light
    - Isotropic
- Hittables:
    - Sphere
    - Rectangle
    - Box (collection of 6 rectangles)
- Other:
    - Defocus Blur
    - Perlin noise (+ turbulence)

### Some more screenshots
![perlin_noise_two_spheres](/images/perlin.png)  
![first_scene_checkered](/images/checker.png)  
![cornell_scene](/images/cornell.png)  


### Future goals
- Threading
- Text-based scene editor

## External Resources
   For image loading (to use in textures) [stb](https://github.com/nothings/stb) was used.