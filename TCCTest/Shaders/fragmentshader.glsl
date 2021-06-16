#version 400 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position; //No espaço da câmera
};

uniform Light light;
uniform Material material;
uniform bool wireframeON;

in vec3 fColor;
in vec3 FragColor;

in vec3 gNormal;
in vec3 gLightDir;
in vec2 fragUV;
uniform vec3 viewer;
uniform sampler2D sampler;
out vec3 finalColor;
uniform int numberOfTesselations;


//Wireframe data
uniform sampler1D wireframe; //The texture sampler
in vec3 wireframeUVW;        //The texture coordinates


void main()
{
    vec3 color = vec3(1.0, 0, 0);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    vec3 ambient = material.ambient;


    if(numberOfTesselations < 5){
//        vec3 N = normalize(gNormal);
//        vec3 L = normalize(gLightDir);

//        float iDif = abs(dot(N, L));
//        float iSpec = 0;
//        vec3 R = vec3(0.0,0.0,0.0);

//        if(iDif > 0)
//        {
//            diffuse = iDif * material.diffuse;
//            //R = normalize(reflect(-L, N));
//            //iSpec = pow(max(dot(viewer,R),0.0), material.shininess);
//            //@todo: Compute specular
//        }

//        specular = iSpec * material.specular;

//        color = (ambient + diffuse + specular)*color;

//        //Uncomment to render the normal color
//        //color = N;
    }

    //Add wireframe combining the final color to the wireframe texture
    vec3 wireframeColor = vec3(1, 1, 1);

    float alpha1 = texture( wireframe, wireframeUVW.x ).r;
    float alpha2 = texture( wireframe, wireframeUVW.y ).r;
    float alpha3 = texture( wireframe, wireframeUVW.z ).r;

    finalColor = color;
    if(wireframeON){
       finalColor = mix( finalColor, wireframeColor, alpha1 );
       finalColor = mix( finalColor, wireframeColor, alpha2 );
       finalColor = mix( finalColor, wireframeColor, alpha3 );
    }

}
