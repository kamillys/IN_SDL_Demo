#version 130
attribute vec3 in_vertices;
attribute vec3 in_normals;
attribute vec2 in_UVs;

out vec3 position;
out vec3 normal;
out vec2 uv;
out vec3 viewVec;
out vec3 lightVec1;
out vec3 lightVec2;

//uniform mat4 UMV;
uniform mat4 UMVP;
uniform mat4 UView;
uniform mat4 UNormalMatrix;
uniform mat4 UModelView;

const vec4 lightpos1 = vec4(0, 2.2, 0, 1.0);
const vec4 lightpos2 = vec4(-50.0, 100.0, 50.0, 1.0);


void main(){
    //pos = in_vertices;
    gl_Position = UMVP * vec4(in_vertices, 1.0);
    position = vec3(UModelView * vec4(in_vertices, 1.0));
    normal = vec3(normalize(UModelView * vec4(in_normals, 0.0)));
    uv = in_UVs;
    viewVec = normalize(-position.xyz);
    lightVec1 = normalize(vec3(UView * lightpos1) - position);
    lightVec2 = normalize(vec3(UView * lightpos2) - position);
}
