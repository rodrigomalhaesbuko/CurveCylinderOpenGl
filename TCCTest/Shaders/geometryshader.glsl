#version 330 core
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 1000) out;

vec4 myValues[1000];

out vec3 fColor;

void drawCircle(vec4 actual_Point, vec4 nextPoint){
    float angle = 0;
    int numberOfPointsInCircle = 80;
    for(int i = 0; i < numberOfPointsInCircle; i++){
        float u = 10.0f*cos(angle);
        float v = 10.0f*sin(angle);
        float t = 0.f;
        myValues[i] =  actual_Point + vec4(u,v,t,1.0f);
        angle = angle + 2.0f*3.1415f/(float(numberOfPointsInCircle));
    }

    angle = 0;
    for(int i = numberOfPointsInCircle; i < (numberOfPointsInCircle) * 2; i++){
        float u = 10.0f*cos(angle);
        float v = 10.0f*sin(angle);
        float t = 0.f;
        myValues[i] =  nextPoint + vec4(u,v,t,1.0f);
        angle = angle + 2.0f*3.1415f/(float(numberOfPointsInCircle));
    }

    for(int i = 0; i< numberOfPointsInCircle; i++){

        int k = i;
        int k1 = (i+1)%numberOfPointsInCircle;
        gl_Position = myValues[k];
        fColor = vec3(1.0f, 0.0f, 0.0f);
        EmitVertex();

        gl_Position = myValues[k1];
        fColor = vec3(1.0f, 0.0f, 0.0f);
        EmitVertex();

        gl_Position = myValues[k1 + numberOfPointsInCircle];
        fColor = vec3(1.0f, 0.0f, 0.0f);
        EmitVertex();


        gl_Position = myValues[k];
        fColor = vec3(0.0f, 1.0f, 0.0f);
        EmitVertex();

        gl_Position = myValues[k1 + numberOfPointsInCircle];
        fColor = vec3(0.0f, 1.0f, 0.0f);
        EmitVertex();

        gl_Position = myValues[k + numberOfPointsInCircle];
        fColor = vec3(0.0f, 1.0f, 0.0f);
        EmitVertex();

        }

}


void main() {
    drawCircle(gl_in[1].gl_Position, gl_in[2].gl_Position );
}
