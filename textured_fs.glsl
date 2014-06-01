#version 130

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

in vec3 position;
in vec3 normal;
in vec2 uv;
in vec3 viewVec;
in vec3 lightVec1;
in vec3 lightVec2;

out vec4 fcolor;

uniform vec4 diffuseColor;
uniform vec4 specularColor;

const float ka = 0.2;
const vec3 light1Color = vec3(0.8, 0.76, 0.54);
const vec3 light2Color = vec3(0.8, 0.8, 0.8);

vec3 saturate(vec3 a)
{
    return clamp(a, 0, 1);
}

float saturate(float a)
{
    return clamp(a, 0, 1);
}

void main()
{
    vec3 ViewVec = normalize(viewVec);
    vec3 Normal = normalize(normal);
    vec3 LightVec = normalize(lightVec1);
    vec3 HalfVec = normalize(ViewVec + LightVec);

    vec4 DiffuseColor = texture2D( diffuseTex, uv);
    vec4 SpecularColor = vec4( texture2D( specularTex, uv).xyz , specularColor.a);

    vec3 Color = DiffuseColor.xyz*ka;
    Color += light1Color*DiffuseColor.xyz * saturate(dot(Normal, LightVec)) + light1Color*SpecularColor.xyz * pow(saturate(dot(Normal, HalfVec)), SpecularColor.a);

    LightVec = normalize(lightVec2);
    HalfVec = normalize(ViewVec + LightVec);
    Color += light2Color*DiffuseColor.xyz * saturate(dot(Normal, LightVec)) + light2Color*SpecularColor.xyz * pow(saturate(dot(Normal, HalfVec)), SpecularColor.a);

    fcolor = vec4(saturate(Color), diffuseColor.a);

    //fcolor.rgb = texture2D( diffuseTex, uv).rgb;
    //fcolor.rgb += texture2D( specularTex, uv).rgb;
    //fcolor.a = 1;
}
