//  Vertex shader for toon and Gouraud
//  Adapted from Lighthouse3D
//  And then adapted by me
#version 120

varying vec3 N;
varying vec3 L;
varying vec4 color;
varying vec2 texCoord;
	
void main()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  L is the light vector
   L = vec3(gl_LightSource[0].position) - P;
   //  Normal
   N = gl_NormalMatrix * gl_Normal;
   //  Color (Claude)
   color = gl_Color;
   // TexCoord (ChatGPT)
   texCoord = gl_MultiTexCoord0.xy;
   //  Transformed vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
