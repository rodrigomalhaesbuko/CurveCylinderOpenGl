#version 400 core

layout(quads, equal_spacing, ccw) in;


uniform float radiusCircle;
out vec3 fColor;
out vec4 tan;

out vec3 vNormal;
out vec3 vLightDir;
uniform mat4 inv_p;

float pi = 3.14159f;

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    float u = gl_TessCoord.x;

    // float u = 1-taux ;
    // glm::vec3 position = u*u*u*v0 + 3*u*u*taux*v1 + 3*u*taux*taux*v2 + taux*taux*taux*v3;
    // glm::vec3 velocity = -3*u*u*v0 + 3*(3*taux*taux - 4*taux + 1)*v1 + 3*(-3*taux*taux + 2*taux)*v2 + 3*taux*taux*v3;

    float b0 = (1.-u) * (1.-u) * (1.-u);
    float b1 = 3. * u * (1.-u) * (1.-u);
    float b2 = 3. * u * u * (1.-u);
    float b3 = u * u * u;
    vec4 pos = b0*p0 + b1*p1 + b2*p2+ b3*p3;

    vec4 velocity = -3*(1.-u)*(1.-u)*p0 + 3*(3*u*u - 4*u + 1)*p1 + 3*(-3*u*u + 2*u)*p2 + 3*u*u*p3;
    tan = normalize(velocity);

    // pontos do circulo
    float angle = gl_TessCoord.y;
    float ua = 2*radiusCircle*cos(2.0*pi*angle);
    float va = 2*radiusCircle*sin(2.0*pi*angle);
    float ta = 0.f;

    //rotaciona
//    vec3 u_ = vec3(1, 0 ,0 );
//    vec3 t0 = vec3(tan);
//    vec3 v0 = normalize(cross(t0,u_));
//    vec3 u0 = cross(v0,t0);

//    vec3 aux = ua*u0 + va*v0 + ta*t0;
//    vec4 pos2 =  pos + vec4(aux, 1.0f);

    //sem rotacao
    vec4 pos2 =  pos + vec4(ua,va,ta, 1.0f);

    // passa a infromacao para o ponto no patch
    gl_Position = pos2;

    // light pos
    vec3 peye = vec3( inv_p * pos);
    vLightDir = normalize(-peye); // luz na posicao do olho
    vNormal = normalize(vec3(pos2 - pos));

    fColor = vec3(1.0,1.0,1.0);
}
