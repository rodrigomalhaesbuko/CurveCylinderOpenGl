#version 400 core

layout( vertices = 4 ) out;

void main()
{

    if(gl_InvocationID == 0) {
        // levels only need to be set once per patch
        gl_TessLevelOuter[0] = float( 1 );
        gl_TessLevelOuter[1] = float( 16 );
    }

    gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
}
