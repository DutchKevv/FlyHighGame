in vec3 v_color;
in vec2 texCoordinates;

out vec4 colorOut;

uniform sampler2D uTexture;

//void main() {
//    colorOut = vec4(0.1f, 0.1f, 1.0f, 0.5f); // texture(uTexture, texCoordinates);
//}

void main() {
    colorOut = texture(uTexture, texCoordinates);
}