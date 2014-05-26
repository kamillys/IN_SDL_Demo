#version 130
attribute vec3 in_vertices;
attribute vec3 in_normals;
attribute vec2 in_UVs;

out vec4 normal;
out vec2 uv;

//uniform mat4 UMV;
uniform mat4 UMVP;
//uniform mat4 UNormalMatrix;

void main(){
    //pos = in_vertices;
    gl_Position = UMVP * vec4(in_vertices, 1.0);
    //normals = UNormalMatrix * vec4(normalize(in_normals), 0.0);
    normal = vec4(in_normals, 0.0);
    uv = in_UVs;
}
