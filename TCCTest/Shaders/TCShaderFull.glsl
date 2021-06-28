#version 400 core

layout( vertices = 4 ) out;
in vec3 tan[];

// Number of subdivions per bezier curve, maximun of 64. Higher values smoth the curve
uniform int numberOfTesselations;
uniform int numberOfPointsInCircle;
void main()
{

    if(gl_InvocationID == 0) {
        gl_TessLevelInner[0] = float( numberOfTesselations);
        gl_TessLevelInner[1] = float( numberOfPointsInCircle);
        gl_TessLevelOuter[0] = float( numberOfPointsInCircle );
        gl_TessLevelOuter[1] = float( numberOfTesselations );
        gl_TessLevelOuter[2] = float( numberOfPointsInCircle );
        gl_TessLevelOuter[3] = float( numberOfTesselations );
    }

    gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
}
