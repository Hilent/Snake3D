#version 330 core

out vec4 FragColor;

in vec2 texCoord;
uniform vec3 inColor;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

void main(){
    FragColor = vec4(inColor, 1.0) * mix(texture(myTexture, texCoord), texture(myTexture2, -texCoord), 0.5);
}