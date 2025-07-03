//  Light up thruster shader
#version 120

varying vec3 N;
varying vec3 L0;
varying vec3 L1;

void main()
{
   vec3 normal = normalize(N);

   // Normalize each light vector
   vec3 l0 = normalize(L0);
   vec3 l1 = normalize(L1);

   // Compute intensity from each light (clamped to non-negative)
   float i0 = max(dot(normal, l0), 0.0);
   float i1 = max(dot(normal, l1), 0.0);

   // Total intensity
   float intensity = normalize(i0 + i1);

   // Final color: white light modulated by total intensity
   gl_FragColor = intensity*vec4(1, 1, 1, 0.5);
}
