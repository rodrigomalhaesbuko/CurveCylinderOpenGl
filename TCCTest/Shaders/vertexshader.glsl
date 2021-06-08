#version 400 core
layout( location = 0 ) in vec3 vertexPos;
layout( location = 1 ) in vec3 vertexNormal;
// receber qual é o endereco do array

//Coordenadas de textura
layout( location = 2 ) in vec2 vertexTexCoord;
out vec3 fColor;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 mv_ti; //inversa transposta da MV
//Posição e normal no espaço da câmera:
out vec3 fragPos;
out vec3 fragNormal;
//Coordenadas de textura do fragmento
out vec2 fragUV;

void main()
{
    gl_Position = mvp * vec4( vertexPos, 1 );
//    int m = v.size(); // me e o tamanho da bezier interpolada
//    int nt = (int((m-1)/3) + 1) - 1;
//    for(int i=0; i< nt; i++){
//        int j  = 3*i;
//        vec3 v0 = v[j];
//        vec3 v1 = v[j + 1];
//        vec3 v2 = v[j + 2];
//        vec3 v3 = v[j + 3];
//        float taux = 0;
//        for(int t = 0; t < 4; t++){ // faz em 4 pedacos para cada ponto da curva interpolada
//            float u = 1-taux ;
//            glm::vec3 position = u*u*u*v0 + 3*u*u*taux*v1 + 3*u*taux*taux*v2 + taux*taux*taux*v3;
//            glm::vec3 velocity = -3*u*u*v0 + 3*(3*taux*taux - 4*taux + 1)*v1 + 3*(-3*taux*taux + 2*taux)*v2 + 3*taux*taux*v3;
//            // fazer unitario de velocity
//            pos->push_back(position);
//            tan->push_back(glm::normalize(velocity));
//            taux = taux + 1.0/float(4);
//        }

//    }
//    fragPos = ( mv * vec4( vertexPos, 1 ) ).xyz;
//    fragNormal = ( mv_ti * vec4( vertexNormal, 0 ) ).xyz;
//    fragUV = vertexTexCoord;

        fColor = vec3(1.0,1.0,1.0);
}
