#version 450

//Sphere struct
struct Sphere {
    vec3 color;
    vec3 position;
    float radius;
};

struct RayHit {
    vec3 position;
    vec3 normal;
    float distance;
    bool hit;
    Sphere sphere;
};

const int RAY_SAMPLES = 80;
const int RAY_STEPS = 16;
const int RAY_BOUNCES = 8;

const Sphere NULL_SPHERE = Sphere(vec3(0.0), vec3(0.0), 0.0);
const RayHit NULL_HIT = RayHit(vec3(0.0), vec3(0.0), 0.0, false, NULL_SPHERE);

const int MAX_SPHERES = 3;

Sphere spheres[MAX_SPHERES] = {
Sphere(vec3(1.0, 0.0, 0.0) * 10.0, vec3(-3.0f, -1.0, 5.0), 1.0),
Sphere(vec3(0.0, 1.0, 0.0), vec3(1.0f, -3.0, 9.0), 1.0),
Sphere(vec3(0.0, 0.0, 1.0), vec3(1.0f, -0.0, 9.0), 1.0)
};

layout(location = 0) in vec3 FRAG_POSITION;
layout(location = 1) in vec2 FRAG_TEXCOORD;
layout(location = 2) in vec3 FRAG_COLOR;

layout(location = 0) out vec4 outColor;

float sdfToSphere(vec3 p, float radius) {
    return length(p) - radius;
}

float rand(vec2 co, float seed) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233)) * seed) * 43758.5453);
}

vec3 randColor(vec2 co, float seed) {
    return vec3(rand(co, seed), rand(co, seed + 1.0), rand(co, seed + 2.0)) * 2.0 - 1.0;
}

RayHit rayMarch(vec3 rayOrigin, vec3 rayDirection) {
    vec3 rayPosition = rayOrigin;
    RayHit hit = NULL_HIT;
    for (int i = 0; i < RAY_STEPS; i++) {
        float distance = 999999.0;
        Sphere sphere = NULL_SPHERE;
        for (int j = 0; j < MAX_SPHERES; j++) {
            if (spheres[j] == NULL_SPHERE) {
                continue;
            }
            float d = sdfToSphere(rayPosition - spheres[j].position, spheres[j].radius);
            if (d < distance) {
                distance = d;
                sphere = spheres[j];
            }
        }
        rayPosition += rayDirection * distance;
        if (distance < 0.01) {
            hit.position = rayPosition;
            hit.normal = normalize(rayPosition - sphere.position);
            hit.distance = length(rayOrigin - hit.position);
            hit.hit = true;
            hit.sphere = sphere;
            break;
        }
    }
    return hit;
}

vec3 shadeSphere(RayHit hit,int index) {
    vec3 color = vec3(0.0);

    RayHit shadowHit = rayMarch(hit.position + hit.normal * 0.02 + randColor(FRAG_TEXCOORD, float(index)) * 0.01, normalize(vec3(0.0, 1.0, 0.0)) + randColor(FRAG_TEXCOORD, float(index)) * 0.1);

    if (shadowHit.hit) {
        color -= vec3(1.0,1.0,1.0) * 0.5;
    }

    //Bounce
    for(int i = 0; i < RAY_BOUNCES; i++) {
        vec3 bounceDirection = reflect(hit.sphere.position - hit.position, hit.normal + randColor(FRAG_TEXCOORD, float(index)) * 0.4);
        RayHit bounceHit = rayMarch(hit.position + hit.normal * 0.02, bounceDirection);
        if (bounceHit.hit) {
            color += bounceHit.sphere.color * 0.3;
        }
    }

    color += hit.sphere.color * 0.5;

    return color;
}

void main() {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const float ASPECT = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);

    const float SAMPLE_OFFSET = 0.005;

    vec3 color = vec3(0.0);

    vec2 uv = FRAG_TEXCOORD;
    uv.x *= ASPECT;
    //Adjust the UV so that 0.5 is back in the center of the screen after the aspect ratio correction
    uv.x -= (ASPECT - 1.0) / 2.0;
    uv -= vec2(0.5, 0.5);
    uv.y = -uv.y;

    vec3 rayOrigin = vec3(0.0, 0.0, 0.0);
    vec3 rayOriginDirection = normalize(vec3(uv, 1.0));

    RayHit hits[RAY_SAMPLES];
    for(int i = 0; i < RAY_SAMPLES; i++) {
        hits[i] = NULL_HIT;
    }
    for(int r = 0; r < RAY_SAMPLES; r++) {
        vec3 rayPosition = rayOrigin;
        vec3 rayDirection = rayOriginDirection + vec3(rand(FRAG_TEXCOORD, float(r)) * SAMPLE_OFFSET, rand(FRAG_TEXCOORD, float(r + 1)) * SAMPLE_OFFSET, rand(FRAG_TEXCOORD, float(r + 2)) * SAMPLE_OFFSET);

        hits[r] = rayMarch(rayPosition, rayDirection);
    }

    vec3 sampleColors[RAY_SAMPLES];
    for(int i = 0; i < RAY_SAMPLES; i++) {
        sampleColors[i] = vec3(0.0);
    }
    for(int i = 0; i < RAY_SAMPLES; i++) {
        if(hits[i].hit) {
            sampleColors[i] = shadeSphere(hits[i],i);
        }
    }

    for(int i = 0; i < RAY_SAMPLES; i++) {
        color += sampleColors[i];
    }
    color /= float(RAY_SAMPLES);

    outColor = vec4(color, 1.0);
}