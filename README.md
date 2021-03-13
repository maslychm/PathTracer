# PathTracer
Path tracer using C++

![part_one_final_scene](/images/image3.png)

## What is this
Lighting Path tracer implemented by following Peter Shirley's [Ray Tracing in One Weekend Book Series](https://raytracing.github.io/).  

Originally I wanted to use this code to render a high resolution avatar similar to one I made in Unity some time ago. It is included as the last screenshot.  

The repository is work in progress. Sometimes when I have free time, I sit down to continue following the book and adding new features 🙂.

So far my custom changes to the base code include: 
- adding `scene` class that contains initialization of `hittalbe` objects and sets appropriate image format depending on scene 
- simple threading 


### Features

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
    - Threading

### Some more screenshots
![perlin_noise_two_spheres](/images/perlin.png)  
![first_scene_checkered](/images/checker.png)  
![cornell_scene](/images/cornell.png)  
![mighty_avatar](/images/mighty.jpg)


### Future plans
The first threading implementation divides the image into `num_threads` (set to number of cores - 2) equal-sized tiles and renders them in parallel. Creating more threads does not make sense unless used with a thread pooler limiting the concurrent threads to `num_threads`. Thus a proper thread pool is the next goal.

The render threads have no downtime, so spawning more than the above reduces total performance, and I found that on my Windows 8-core machine 6 threads give the best performance. I'm interested in plotting the exact render times depending on number of threads in the future.

## External Resources
   For image loading (to use in textures) [stb](https://github.com/nothings/stb) was used.