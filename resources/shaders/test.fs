#version 330 core
out vec4 FragColor;

// Camera uniforms
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform float pitch;
uniform float yaw;
uniform float fov;

// -----------------------------------------------------------------

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

// -----------------------------------------------------------------

float sdBox(vec3 p, vec3 b) {
    vec3 d = abs(p) - b;
    return length(max(d, vec3(0.0))) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float sceneSDF(vec3 p) {
    // Wall 1: vertical wall at z = 5, spanning x in [-2,2] and y in [0,3]
    // Centered at (0,1.5,5) with half-extents (2, 1.5, 0.05)
    float d1 = sdBox(p - vec3(0.0, 1.5, 5.0), vec3(2.0, 1.5, 0.05));
    
    // Wall 2: vertical wall at x = 5, spanning z in [-2,2] and y in [0,3]
    // Centered at (5,1.5,0) with half-extents (0.05, 1.5, 2)
    float d2 = sdBox(p - vec3(5.0, 1.5, 0.0), vec3(0.05, 1.5, 2.0));
    
    // Return the minimum distance from the two walls.
    return min(d1, d2);
}

// Returns a Bayer matrix threshold based on fragment coordinate.
// The matrix is 4x4 with values normalized in [0,1].
float bayerDither(vec2 coord) {
    // 4x4 Bayer matrix normalized between 0 and 1.
    float bayer[16] = float[16](
        0.0/16.0,  8.0/16.0,  2.0/16.0, 10.0/16.0,
        12.0/16.0, 4.0/16.0, 14.0/16.0,  6.0/16.0,
         3.0/16.0, 11.0/16.0,  1.0/16.0,  9.0/16.0,
        15.0/16.0, 7.0/16.0, 13.0/16.0,  5.0/16.0
    );
    // Compute index from the pixel coordinate modulo 4.
    int x = int(mod(coord.x, 4.0));
    int y = int(mod(coord.y, 4.0));
    int index = x + y * 4;
    return bayer[index];
}

void main() {
    float d = 0.05 * (hash(gl_FragCoord.xy) - 0.5);
    vec3 lightColor = vec3(0.8+d, 0.75+d, 0.7+d);
    vec3 darkColor = vec3(0.1, 0.1, 0.1);
    vec3 color = darkColor;

    vec2 uv = (gl_FragCoord.xy / resolution) * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    // Camera basis vectors
    // Compute forward vector from pitch and yaw
    vec3 forward = vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
    // Right vector perpendicular to forward (assuming world up is (0,1,0))
    vec3 right = vec3(sin(yaw - 1.5708), 0.0, cos(yaw - 1.5708));
    // Up vector by cross product
    vec3 up = cross(right, forward);

    // Compute ray direction with FOV adjustment
    float tanFov = tan(radians(fov) * 0.5);
    vec3 rayDir = normalize(forward + uv.x * right * tanFov + uv.y * up * tanFov);

    // Raymarching variables
    float t = 0.0;
    const float tMax = 100.0;
    const int maxSteps = 100;
    const float hitThreshold = 0.001;
    bool hit = false;
    vec3 p;

    // Raymarch loop
    for (int i = 0; i < maxSteps; i++) {
        p = cameraPos + t * rayDir;
        float d = sceneSDF(p);
        if (d < hitThreshold) {
            hit = true;
            break;
        }
        if (t > tMax) {
            break;
        }
        t += d;
    }

    float tUsed = hit ? t : tMax;
    const float fogStart = 10.0;
    const float fogEnd = 30.0;
    float fogFactor = clamp((tUsed - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

    if (hit) {
        float ditherThreshold = bayerDither(gl_FragCoord.xy);
        color = (fogFactor > ditherThreshold) ? darkColor : lightColor;
    }

    FragColor = vec4(color, 1.0);
}