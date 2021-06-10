#version 400 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec3 vLightDir[];
in vec3 vColor[];

out vec3 gNormal;
out vec3 gLightDir;
out vec3 wireframeUVW;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    gNormal = vNormal[0];
    gLightDir = vLightDir[0];
    wireframeUVW = vec3(1.0, 1.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gNormal = vNormal[1];
    gLightDir = vLightDir[1];
    wireframeUVW = vec3(0.0, 1.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gNormal = vNormal[2];
    gLightDir = vLightDir[2];
    wireframeUVW = vec3(1.0, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
