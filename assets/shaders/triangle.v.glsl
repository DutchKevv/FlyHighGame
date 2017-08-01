layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoordinates; // the texture variable has attribute position 2

uniform float xOffset; // the color variable has attribute position 1
uniform mat4 transform; // the transformation happening
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_color;
out vec2 texCoordinates;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
//    gl_Position = transform * vec4(aPos.xy, 0, 1); //projection * view * model * vec4(aPos, 1.0);
    // gl_Position = projection * view * model * (transform * vec4(aPos, 1.0));

    texCoordinates = aTexCoordinates;
//    // gl_Position.xyz = aPos;
//    gl_Position.x = 0.5f; // aPos.x;
//    gl_Position.yz = aPos.yz;
//    gl_Position.w = 1.0;

    v_color = aColor; // gl_Position * 0.5 + 0.5;
}
