#version 120

uniform sampler2DRect tex0;
uniform vec2 size;

void main()
{

  vec2 uv = gl_TexCoord[0].xy;
  vec3 tc = vec3(1.0, 0.0, 0.0);

  float dx = size.x;
  float dy = size.y;
  vec2 coord = vec2(dx*floor(uv.x/dx), dy*floor(uv.y/dy));

  tc = texture2DRect(tex0, coord).rgb;
    	
  gl_FragColor = vec4(tc, 1.0);
  	
}