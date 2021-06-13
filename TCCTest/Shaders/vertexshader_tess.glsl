#version 400 core
layout( location = 0 ) in vec3 vertexPos;
layout( location = 1 ) in vec3 vertexNormal;

// receber qual é o endereco do array

//Coordenadas de textura
layout( location = 2 ) in vec2 vertexTexCoord;
layout( location = 3 ) in vec3 tangent;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 mv_ti; //inversa transposta da MV
//Posição e normal no espaço da câmera:
//Coordenadas de textura do fragmento
//out vec2 fragUV;

out vec4 tan;

void main()
{
    gl_Position = mvp * vec4(vertexPos, 1 );
    tan = mv * vec4(tangent, 1 );
}
