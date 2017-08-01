layout (location = 0) in vec3 coord;
layout (location = 1) in vec3 color; // the color variable has attribute position 1
layout (location = 2) in vec2 textureCoord; // the color variable has attribute position 1

out vec3 v_color;
out vec2 texCoordinates;

void main(){
    gl_Position = vec4(coord, 1);
//    gl_Position = transform * vec4(aPos.xy, 0, 1); //projection * view * model * vec4(aPos, 1.0);
    // gl_Position = projection * view * model * (transform * vec4(aPos, 1.0));

    texCoordinates = textureCoord;
//    // gl_Position.xyz = aPos;
//    gl_Position.x = 0.5f; // aPos.x;
//    gl_Position.yz = aPos.yz;
//    gl_Position.w = 1.0;

    v_color = color; // gl_Position * 0.5 + 0.5;
}
