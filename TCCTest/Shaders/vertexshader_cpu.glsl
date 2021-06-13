#version 400 core
layout( location = 0 ) in vec3 vertexPos;
layout( location = 1 ) in vec3 vertexNormal;
// receber qual é o endereco do array

//Coordenadas de textura
layout( location = 2 ) in vec2 vertexTexCoord;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 mv_ti; //inversa transposta da MV
//Posição e normal no espaço da câmera:
out vec3 fragPos;

out vec3 vNormal;
out vec3 vLightDir;
out vec3 vColor;

//Coordenadas de textura do fragmento
//out vec2 fragUV;

void main()
{
    vec3 peye = vec3(mv * vec4(vertexPos, 1.0f));

    vec3 lpos = vec3(0, 0, 0); //light in the camera
    vLightDir = normalize(lpos - peye);

    gl_Position = mvp * vec4(vertexPos, 1 );
    vNormal = ( mv_ti * vec4( vertexNormal, 0 ) ).xyz;

//    fragUV = vertexTexCoord;

    vColor = vec3(1.0,1.0,1.0);
}
