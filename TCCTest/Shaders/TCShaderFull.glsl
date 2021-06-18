#version 400 core

layout( vertices = 4 ) out;
// Number of subdivions per bezier curve, maximun of 64. Higher values smoth the curve
uniform int numberOfTesselations;
uniform int numberOfPointsInCircle;
in vec3 tan[];

void main()
{

    if(gl_InvocationID == 0) {
        // levels only need to be set once per patch

        gl_TessLevelInner[0] = float( numberOfTesselations);
        gl_TessLevelInner[1] = float( numberOfPointsInCircle);// number of nested primitives to generate
        gl_TessLevelOuter[0] = float( numberOfPointsInCircle );
        gl_TessLevelOuter[1] = float( numberOfPointsInCircle );
        gl_TessLevelOuter[2] = float( numberOfPointsInCircle );
        gl_TessLevelOuter[3] = float( numberOfPointsInCircle );

    }

    gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
}
