#version 130

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

in vec4 normal;
in vec2 uv;

out vec4 fcolor;

uniform vec4 diffuseColor;
uniform vec4 specularColor;

void main(){
//    vec3 lightDir = normalize(vec3(1,1,0));
//    vec3 N = normalize(normals.xyz);
//    float p = abs(dot(N,lightDir));
//    fcolor.rgb = p*vec3(0,1,0);
//    fcolor.a = 1;
    //fcolor.rgb = vec3(normals);
    fcolor.rgb = texture2D( diffuseTex, uv).rgb;
    //fcolor.rgb = vec3(uv, 0);
    fcolor.a = 1;
}
