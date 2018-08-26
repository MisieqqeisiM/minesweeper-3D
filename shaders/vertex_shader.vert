#version 330 core
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TextureCoords;
out vec2 pass_TextureCoords;
flat out vec3 pass_Normal;
out vec3 fragPos;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
void main(){
    pass_TextureCoords = in_TextureCoords;
    pass_Normal = (model_matrix*vec4(in_Normal,0.0f)).xyz;
    gl_Position = projection_matrix*(view_matrix*(model_matrix*vec4(in_Position, 1.0f)));
    fragPos = (model_matrix*vec4(in_Position, 1.0f)).xyz;
}
