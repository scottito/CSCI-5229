// Gouraud Fragment Shader
#version 120

varying vec3 N;
varying vec3 L;
varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform bool useTexture;
	
void main()
{   // Funny enough, I had to add intensity myself
    float intensity = dot(normalize(N),normalize(L));
    intensity = 0.3+intensity*(0.7); // Adding ambient
    // I got the fact that it was just FragColor Claude

    if (useTexture)
    {
      // I got this line from ChatGPT
      vec4 texColor = texture2D(tex, texCoord);
      gl_FragColor = vec4(intensity * texColor * color);
    }
   else
      gl_FragColor = vec4(intensity * color); 
}