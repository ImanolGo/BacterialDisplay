
precision highp float;

uniform sampler2D tex0;
uniform vec2 size;

varying vec2 texCoordVarying;


void main()
{
	float dx = size.x;
	float dy = size.y;
	vec2 coord = vec2(dx*floor(texCoordVarying.x/dx), dy*floor(texCoordVarying.y/dy));

    vec3 tc = texture2D(tex0, coord).rgb;
    gl_FragColor = vec4(tc , 1.0);
}