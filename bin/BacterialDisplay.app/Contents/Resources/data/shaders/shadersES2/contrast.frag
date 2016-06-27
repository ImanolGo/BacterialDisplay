
precision highp float;

uniform sampler2D tex0;
uniform vec4 range;

varying vec2 texCoordVarying;

float map(float i, float iMin, float iMax, float oMin, float oMax )
{
   
   float value = oMin + ((i - iMin) / (iMax - iMin) * (oMax - oMin));

   // if (value < oMin) {
   //  value = oMin;
   // }

   // if (value > oMax) {
   //  value = oMax;
   // }

   return value;
}


void main()
{
	vec3 colorIn = texture2D(tex0, texCoordVarying).rgb;

	fvec3 colorOut;

	colorOut.r = map(colorIn.r, range.r, range.g, range.b, range.a);
	colorOut.g = map(colorIn.g, range.r, range.g, range.b, range.a);
	colorOut.b = map(colorIn.b, range.r, range.g, range.b, range.a);

    gl_FragColor = vec4(colorOut , 1.0);
}