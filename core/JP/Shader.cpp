const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color; // Varying
uniform float uTime;
void main() 
{
   Color = aColor; //Pass-through
   vec3 pos = aPos;
   pos.y += sin(uTime + pos.x) / 4.0;	
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); 
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec4 Color;
uniform float uTime = 1.0;
uniform vec4 uColor = vec4(1.0);
void main() 
{
   FragColor = Color * (sin(uTime) * 0.5 + 0.5);
}
)";