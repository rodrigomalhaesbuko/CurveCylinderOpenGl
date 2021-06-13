#version 400 core
layout (lines) in;
layout (triangle_strip, max_vertices = 1000) out;

vec4 myValues[1000];
int lastPrimitiveIn;

bool rotatation = false;

vec3 k[2];
vec3 uEixo[2];
in vec4 tan[];

out vec3 fColor;
out vec3 wireframeUVW;

uniform int numberOfPointsInCircle;
uniform float radiusCircle;

vec4 quat_from_axis_angle(vec3 axis, float angle)
{
  vec4 qr;
  float half_angle = (angle * 0.5) * 3.14159 / 180.0;
  qr.x = axis.x * sin(half_angle);
  qr.y = axis.y * sin(half_angle);
  qr.z = axis.z * sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}

vec4 quat_conj(vec4 q)
{
  return vec4(-q.x, -q.y, -q.z, q.w);
}

vec4 quat_mult(vec4 q1, vec4 q2)
{
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{
  vec4 qr = quat_from_axis_angle(axis, angle);
  vec4 qr_conj = quat_conj(qr);
  vec4 q_pos = vec4(position.x, position.y, position.z, 0);

  vec4 q_tmp = quat_mult(qr, q_pos);
  qr = quat_mult(q_tmp, qr_conj);

  return vec3(qr.x, qr.y, qr.z);
}


void drawCircle(vec4 actual_Point, vec4 nextPoint, float radius){
    float angle = 0;
    int numberOfPointsInCircle = numberOfPointsInCircle;
    float raio = radius;
    //        std::vector<glm::vec3> R;
    //        R.push_back(glm::vec3(ui[0],ui[1],ui[2]));
    //        R.push_back(glm::vec3(vi[0],vi[1],vi[2]));
    //        R.push_back(glm::vec3(ti[0],ti[1],ti[2]));
    //        glm::vec3 point = posi + circleJ[0]*R[0]+circleJ[1]*R[1]+circleJ[2]*R[2];
    for(int i = 0; i < numberOfPointsInCircle; i++){
        float u = raio*cos(angle);
        float v = raio*sin(angle);
        float t = 0.f;
        myValues[i] =  actual_Point + vec4(u,v,t,1.0f);
        angle = angle + 2.0f*3.14159f/(float(numberOfPointsInCircle));
        // Rotation
        if(rotatation){
            vec3 aux = u*uEixo[0] + v*k[0] + t*vec3(tan[0]);
            myValues[i] = actual_Point + vec4(aux, 1.0f);
        }

    }

    angle = 0;
    for(int i = numberOfPointsInCircle; i < (numberOfPointsInCircle) * 2; i++){
        float u = raio*cos(angle);
        float v = raio*sin(angle);
        float t = 0.f;
        myValues[i] =  nextPoint + vec4(u,v,t,1.0f);
        angle = angle + 2.0f*3.14159f/(float(numberOfPointsInCircle));
        //rotation
        if(rotatation){
            vec3 aux = u*uEixo[1] + v*k[1] + t*vec3(tan[1]);
            myValues[i] = nextPoint + vec4(aux, 1.0f);
        }


    }

    for(int i = 0; i< numberOfPointsInCircle; i++){
        int k = i;
        int k1 = (i + 1)%numberOfPointsInCircle;
        gl_Position = myValues[k];
        fColor = vec3(1.0f, 1.0f, 0.0f);
        wireframeUVW = vec3(1.0, 1.0, 0.0);
        EmitVertex();

        gl_Position = myValues[k1];
        fColor = vec3(1.0f, 1.0f, 0.0f);
        wireframeUVW = vec3(0.0, 1.0, 1.0);
        EmitVertex();

        gl_Position = myValues[k1 + numberOfPointsInCircle];
        fColor = vec3(1.0f, 1.0f, 0.0f);
        wireframeUVW = vec3(1.0, 0.0, 1.0);
        EmitVertex();
        EndPrimitive();

        gl_Position = myValues[k];
        fColor = vec3(1.0f, 0.0f, 0.0f);
        wireframeUVW = vec3(1.0, 1.0, 0.0);
        EmitVertex();

        gl_Position = myValues[k1 + numberOfPointsInCircle];
        fColor = vec3(1.0f, 0.0f, 0.0f);
        wireframeUVW = vec3(0.0, 1.0, 1.0);
        EmitVertex();

        gl_Position = myValues[k + numberOfPointsInCircle];
        fColor = vec3(1.0f, 0.0f, 0.0f);
        wireframeUVW = vec3(1.0, 0.0, 1.0);
        EmitVertex();
        EndPrimitive();

//        for(int j = 0; j< numberOfCircles-1; j++){
//            for(int i = 0; i< pointsInCircle; i++){
//                int k = j*10 + i;
//                int k1 = j*10 + (i+1)%10;
//                tri.push_back(glm::vec3(k,k1,k1+10));
//                tri.push_back(glm::vec3(k,k1+10,k+10));
//            }
//        }

        }

}


void main() {
    // Rotation
    //float angle = glm::acos(t0[0]*t1[0]+t0[1]*t1[1]+t0[2]*t1[2]);
//    glm::quat quat_rot =  glm::quat(glm::cos(angle/2), glm::sin(angle/2)*eixo);
//    glm::quat quat_u0 = glm::quat(0.0f, u0);
//    glm::quat quat_v0 = glm::quat(0.0f, v0);
//    glm::quat quat_u1  = quat_rot*quat_u0*glm::conjugate(quat_rot);
//    *u1 = glm::vec3(quat_u1.x, quat_u1.y, quat_u1.z);
//    glm::quat quat_v1  = quat_rot*quat_v0*glm::conjugate(quat_rot);
//    *v1 = glm::vec3(quat_v1.x, quat_v1.y, quat_v1.z);

    vec3 u_ = vec3(1, 0 ,0 );
    vec3 t0 = vec3(tan[0]);
    vec3 t1 = vec3(tan[1]);
    vec3 v0 = normalize(cross(t0,u_));
    vec3 u0 = cross(v0,t0);
    vec3 eixo = cross(t0,t1);
    eixo = normalize(eixo);
    uEixo[0] = u0;
    k[0] = v0;

    float angle = acos(t0.x*t1.x+t0.y*t1.y+t0.z*t1.z);

//    vec4 quat_rot = quat_from_axis_angle(sin(angle/2)*eixo, cos(angle/2));
//    vec4 quat_u0 = quat_from_axis_angle(u0, 0.0f);
//    vec4 quat_v0 = quat_from_axis_angle(v0, 0.0f);
//    vec4 quat_u1 = quat_rot*quat_u0*quat_conj(quat_rot);
//    uEixo[1] = vec3(quat_u1.x,quat_u1.y,quat_u1.z);
//    vec4 quat_v1 = quat_rot*quat_v0*quat_conj(quat_rot);
//    k[1] = vec3(quat_v1.x,quat_v1.y,quat_v1.z);

    uEixo[1] = rotate_vertex_position(uEixo[0], eixo, angle);
    k[1] = rotate_vertex_position(k[0], eixo, angle);

    drawCircle(gl_in[0].gl_Position, gl_in[1].gl_Position, radiusCircle);
}


