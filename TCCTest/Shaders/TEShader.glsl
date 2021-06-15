#version 400 core

layout( isolines, equal_spacing, ccw) in;

out vec3 fColor;
out vec4 tan;

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
    gl_Position = pos;


    vec4 velocity = -3*(1.-u)*(1.-u)*p0 + 3*(3*u*u - 4*u + 1)*p1 + 3*(-3*u*u + 2*u)*p2 + 3*u*u*p3;
    tan = normalize(velocity);

    fColor = vec3(1.0,1.0,1.0);
}
