#version 330 core
in vec2 pass_TextureCoords;
in vec3 fragPos;
flat in vec3 pass_Normal;
out vec4 color;
uniform sampler2D tileTextures;
uniform vec3 light_pos;
void main(){
    float distanceToLight = distance(light_pos, fragPos);
    if(distanceToLight<64){
        color = 1.0f/(1.0f+0.045*distanceToLight+0.005*pow(distanceToLight,2))*dot(pass_Normal, /*normalize(vec3(1.0f,3.0f,2.0f))*/normalize(light_pos-fragPos))*texture(tileTextures, pass_TextureCoords*vec2(1.0f/16, 1.0f));
        color = vec4(pow(color.r, 1/2.2), pow(color.g, 1/2.2), pow(color.b,1/2.2), 1.0f);
    }else{
        color = vec4(0.0f,0.0f,0.0f,1.0f);
    }

}
