
precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;

uniform float vx_offset;
uniform float rt_w; // GeeXLab built-in
uniform float rt_h; // GeeXLab built-in
uniform float pixel_w; // 15.0
uniform float pixel_h; // 10.0

void main()
{
    vec3 src = texture2D(tex0, texCoordVarying).rgb;
    float mask = texture2D(maskTex, texCoordVarying).r;
    gl_FragColor = vec4(src , mask);
}