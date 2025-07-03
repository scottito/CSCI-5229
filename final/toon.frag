//  Cartoon fragment shader
//  Adapted from Lighthouse3D
#version 120

varying vec3 N;
varying vec3 L;
varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform bool useTexture;
	
void main()
{
   float intensity = dot(normalize(N),normalize(L));
   intensity = 0.3+intensity*(0.7); // Adding ambient

   float f;
   if (intensity > 0.95)
      f= 1.0;
   else if (intensity > 0.5)
      f = 0.6;
   else if (intensity > 0.25)
      f = 0.4;
   else
      f = 0.2;

   if (useTexture)
   {
      // I got this line from ChatGPT
      vec4 texColor = texture2D(tex, texCoord);
      gl_FragColor = vec4(f * texColor * color);
   }
   else
      gl_FragColor = vec4(f * color);
}
