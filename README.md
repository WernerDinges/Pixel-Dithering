# OpenGL Fog Dithering

This is an example of a raymarching rendering that implements fog pixelization (dithering).

![Screenshot 2025-04-03 000033](https://github.com/user-attachments/assets/caf00d28-e547-411d-81ab-6b5259eaad35)

![Screenshot 2025-04-03 000957](https://github.com/user-attachments/assets/b7ac1179-ed44-4b44-9718-32acdce8184b)

![Screenshot 2025-04-03 001008](https://github.com/user-attachments/assets/3c90696a-15df-4a0e-8c3d-03ab39d0cc70)

![Screenshot 2025-04-03 001046](https://github.com/user-attachments/assets/e45762a6-1848-4cb4-9372-862478417bc4)

## How does it work

To see the full code, see the [shader](https://github.com/WernerDinges/Pixel-fog/blob/main/resources/shaders/test.fs).
It is assumed that the reader is already familiar with the principle of raymarching.

By taking steps, the beam accumulates fog factor.
For each pixel in the Bayer matrix, a threshold is calculated, above which the clouding coefficient turns the pixel completely dark, otherwise - completely light.

Additionally, subtle white noise was applied for decorative purposes.
