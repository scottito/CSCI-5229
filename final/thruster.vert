//  Light up thruster shader
#version 120

varying vec3 N;
varying vec3 L0;
varying vec3 L1;
	
void main()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  L is the light vector
   L0 = vec3(gl_LightSource[0].position) - P;
   L1 = vec3(gl_LightSource[1].position) - P;
   //  Normal
   N = gl_NormalMatrix * gl_Normal;
   //  Transformed vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}