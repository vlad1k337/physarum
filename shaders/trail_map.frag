#version 460 core
// props to https://www.shadertoy.com/user/_samuelcho_ for cool color jiggling functions

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D tex;

vec3 colors[] = vec3[](
        vec3(0.0, 0.0, 0.0),
        vec3(0.64, 0.49, 0.51),
        vec3(0.64, 0.32, 0.51),
        vec3(0.56, 0.1, 0.3)
    );

vec3 palette(in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d)
{
    return a + b * cos(6.28318 * (c * t + d));
}

vec3 RAMP(float x) {
    x *= float(colors.length() - 1);
    return mix(colors[int(x)], colors[int(x + 1)], smoothstep(0.0, 1.0, fract(x)));
}

void main()
{
    vec3 texCol = texture(tex, TexCoords).rgb;

    const float gamma = 2.2;
    texCol.rgb = pow(RAMP(texCol.r), vec3(1.0 / gamma));
    //texCol.rgb = pow(texCol.rgb, vec3(1.0 / gamma));

    FragColor = vec4(texCol.rgb, 1.0);
}
