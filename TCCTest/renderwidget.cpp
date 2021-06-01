#include "renderwidget.h"
#include "polygonalmoulder.h"
#include <QImage>
#include <QGLWidget>
#include <QMouseEvent>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Init
RenderWidget::RenderWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , program(nullptr)
{
}


RenderWidget::~RenderWidget()
{
    //Delete OpenGL resources
}

// ARC BALL
void RenderWidget::mousePressEvent(QMouseEvent *event){
    arcBallEnable = true;
    oldPoint = {event -> x(), event -> y()};

}
void RenderWidget::mouseReleaseEvent(QMouseEvent *event){
        qDebug() << "Released Event";
        if (arcBallEnable){
            arcBallEnable = false;
        }
}
void RenderWidget::mouseMoveEvent(QMouseEvent *event){
    if (arcBallEnable){
        glm::ivec2 newpoint =  {event -> x(), event -> y()};
        rotate(oldPoint,newpoint);
        update();
        oldPoint = newpoint;
    }

}
void RenderWidget::wheelEvent(QWheelEvent *event){
        eyeBall += (centerBall - eyeBall) * 0.001f * event->delta();
        view = glm::lookAt(eyeBall,centerBall, upBall);
        update();
}


// init gl ativa sempre (chamada automaticamente )
void RenderWidget::initializeGL()
{
    //Initialize ARCBALL
    int w  = width();
    int h = height();

    radius = std::max(h,w);
    center = {w * 0.5f, h * 0.5f};

    //Initializa as funções OpenGL
    initializeOpenGLFunctions();

    //Define a cor do fundo
    glClearColor(0,0,0,0);

    //Define a viewport
    glViewport(0,0, width(), height());

    //Compilar os shaders
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertex");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
    program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry");
    program.link();

    glm::vec3 eye(0,0,100);
    glm::vec3 center(0,0,0);
    glm::vec3 up(0,1,0);

    eyeBall = eye;
    centerBall = center;
    upBall = up;


    //Definir matriz view e projection
    float ratio = static_cast<float>(width())/height();
    view = glm::lookAt(eye, center, up);
    proj = glm::perspective<float>(glm::radians(45.0f), ratio, 0.1, 1000.0f);


    // QUAD DE PEDRAS OU BOLA DE GOLFE
    //Cena de esferas
    createPoly();

    //Criar VBO e VAO
    createVBO();
}


void RenderWidget::paintGL()
{
    //Habilita o teste de Z
    glEnable(GL_DEPTH_TEST);

    //Limpar a tela
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //Linkar o VAO
    glBindVertexArray(VAO);

    //Linkar o programa e passar as uniformes:
    //Matriz
    //Posição da luz
    //Ambiente, difusa, especular e brilho do material
    program.bind();
    QMatrix4x4 m(glm::value_ptr(glm::transpose(model)));
    QMatrix4x4 v(glm::value_ptr(glm::transpose(view)));
    QMatrix4x4 p(glm::value_ptr(glm::transpose(proj)));
    program.setUniformValue("viewer", QVector3D(eyeBall.x, eyeBall.y, eyeBall.z));
    //Passar as uniformes da luz e do material
    //BOLA

    program.setUniformValue("light.position", QVector3D(0,0,5) );
    program.setUniformValue("material.ambient", QVector3D(0.2f,0.2f,0.2f));
    program.setUniformValue("material.diffuse", QVector3D(0.6f,0.6f,0.6f));
    program.setUniformValue("material.specular", QVector3D(1.0f,1.0f,1.0f));
    program.setUniformValue("material.shininess", 100.0f);


    //Ativar e linkar a textura

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    program.setUniformValue("sampler", 0);

    QMatrix4x4 sphereModel;
    sphereModel.translate(0,0,0);
    //Passar as matrizes mv e mvp
    QMatrix4x4 mv = v * m * sphereModel;
    QMatrix4x4 mvp = p * mv;
    program.setUniformValue("mv", mv);
    program.setUniformValue("mv_ti", mv.inverted().transposed());
    program.setUniformValue("mvp", mvp);

    //Desenhar
    glDrawArrays(GL_LINES_ADJACENCY_EXT, 0, vertices.size());

}


void RenderWidget::resizeGL(int w, int h)
{
    //Atualizar a viewport

    //Atualizar a matriz de projeção
}

glm::quat RenderWidget::pointToQuat(glm::ivec2 screenPoint)
{
    if(radius > 0){
        glm::vec3 v(glm::vec2(screenPoint - center)/radius, 0.0f);
        auto r = glm::length2(v);
        if(r>1){
            v *= (1.0f/glm::sqrt(r));
        }else {
            v.z = glm::sqrt(1-r);
        }

        return glm::quat(0.0f, v);
    }
    return glm::quat();
}

void RenderWidget::rotate(glm::ivec2 p1, glm::ivec2 p2)
{
    auto q0 = pointToQuat({p1.x,height()-p1.y});
    auto q1 = pointToQuat({p2.x,height()-p2.y});
    auto q  = q1*glm::conjugate(q0);

    auto rot = glm::mat4_cast(q);
    model = rot * model;
}


void RenderWidget::createCube()
{
    //Definir vértices, normais e índices
    vertices = {
        { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 },
        { +1, -1, -1 }, { +1, -1, -1 }, { +1, -1, -1 },
        { +1, -1, +1 }, { +1, -1, +1 }, { +1, -1, +1 },
        { -1, -1, +1 }, { -1, -1, +1 }, { -1, -1, +1 },
        { -1, +1, -1 }, { -1, +1, -1 }, { -1, +1, -1 },
        { +1, +1, -1 }, { +1, +1, -1 }, { +1, +1, -1 },
        { +1, +1, +1 }, { +1, +1, +1 }, { +1, +1, +1 },
        { -1, +1, +1 }, { -1, +1, +1 }, { -1, +1, +1 }
    };

    normals = {
        {  0, -1,  0 }, { -1,  0,  0 }, {  0,  0, -1 },
        {  0, -1,  0 }, { +1,  0,  0 }, {  0,  0, -1 },
        {  0, -1,  0 }, { +1,  0,  0 }, {  0,  0, +1 },
        {  0, -1,  0 }, { -1,  0,  0 }, {  0,  0, +1 },
        { -1,  0,  0 }, {  0,  0, -1 }, {  0, +1,  0 },
        { +1,  0,  0 }, {  0,  0, -1 }, {  0, +1,  0 },
        { +1,  0,  0 }, {  0,  0, +1 }, {  0, +1,  0 },
        { -1,  0,  0 }, {  0,  0, +1 }, {  0, +1,  0 }
    };

    texCoords = {
        {0.25, 0.50}, {0.25, 0.50}, {0.50, 0.75},
        {0.00, 0.50}, {1.00, 0.50}, {0.75, 0.75},
        {0.00, 0.25}, {1.00, 0.25}, {0.75, 0.00},
        {0.25, 0.25}, {0.25, 0.25}, {0.50, 0.00},
        {0.50, 0.50}, {0.50, 0.50}, {0.50, 0.50},
        {0.75, 0.50}, {0.75, 0.50}, {0.75, 0.50},
        {0.75, 0.25}, {0.75, 0.25}, {0.75, 0.25},
        {0.50, 0.25}, {0.50, 0.25}, {0.50, 0.25}
    };

    indices = {
        0,   3,  6, //normal: (  0, -1,  0 )
        0,   6,  9, //normal: (  0, -1,  0 )
        12,  1, 10, //normal: ( -1,  0,  0 )
        12, 10, 21, //normal: ( -1,  0,  0 )
        18,  7,  4, //normal: ( +1,  0,  0 )
        18,  4, 15, //normal: ( +1,  0,  0 )
        22, 11,  8, //normal: (  0,  0, +1 )
        22,  8, 19, //normal: (  0,  0, +1 )
        16,  5,  2, //normal: (  0,  0, -1 )
        16,  2, 13, //normal: (  0,  0, -1 )
        23, 20, 17, //normal: (  0, +1,  0 )
        23, 17, 14  //normal: (  0, +1,  0 )
    };
}

void RenderWidget::createQuad()
{
    //Definir vértices, normais e índices
    vertices = {
        { -1, -1, 0 },
        { +1, -1, 0 },
        { +1, +1, 0 },
        { -1, +1, 0 },
        { -1, -1, 0 },
        { +1, -1, 0 }

    };

    normals = {
        {  0, 0,  1 },
        {  0, 0,  1 },
        {  0, 0,  1 },
        {  0, 0,  1 },
        {  0, 0,  1 },
        {  0, 0,  1 },
        {  0, 0,  1 },
    };

    texCoords = {
       {0,0},
       {1,0},
       {1,1},
       {0,1},
       {0,0},
       {1,0}
    };

    indices = {
        0,  1,  2, //normal: (  0, 0,  -1)
        0,   2,  3, //normal: (  0, 0,  -1 )
    };

    // CALCULO DA TANGENTE

    // positions
    glm::vec3 pos0(-1.0,  -1.0, 0.0);
    glm::vec3 pos1(+1.0, -1.0, 0.0);
    glm::vec3 pos2( +1.0, +1.0, 0.0);
    glm::vec3 pos3( -1.0,  +1.0, 0.0);
    glm::vec3 pos4( -1.0,  -1.0, 0.0);
    glm::vec3 pos5( +1.0,  -1.0, 0.0);
    // texture coordinates
    glm::vec2 uv0(0.0, 0.0);
    glm::vec2 uv1(1.0, 0.0);
    glm::vec2 uv2(1.0, 1.0);
    glm::vec2 uv3(0.0, 1.0);
    glm::vec2 uv4(0.0, 0.0);
    glm::vec2 uv5(1.0, 0.0);
    // normal vector
    glm::vec3 nm(0.0, 0.0, 1.0);

    glm::vec3 edge1 = pos1 - pos0;
    glm::vec3 edge2 = pos2 - pos0;
    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent1 = glm::vec3(0,0,0);
    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    edge1 = pos2 - pos0;
    edge2 = pos3 - pos0;
    deltaUV1 = uv2 - uv0;
    deltaUV2 = uv3 - uv0;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent2 = glm::vec3(0,0,0);
    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    tangentes.push_back(tangent1);
    tangentes.push_back(tangent1);
    tangentes.push_back(tangent1);
    tangentes.push_back(tangent2);
    tangentes.push_back(tangent2);
    tangentes.push_back(tangent2);

}


void RenderWidget::createSphere()
{
    const int n = 100;
    const int m = 100;

    const int numTriangles = 2 * n * m;

    for( unsigned int i = 0; i <= n; i++ )
    {
        for( unsigned int j = 0; j <= m; j++ )
        {
            //Atualizar as coordenadas de textura
            double s = static_cast<double>(i) / n;
            double t = static_cast<double>(j) / m;
            texCoords.push_back(glm::vec2(s,t));

            //Calcula os parâmetros
            double theta = 2 * s * M_PI;
            double phi = t * M_PI;
            double sinTheta = sin( theta );
            double cosTheta = cos( theta );
            double sinPhi = sin( phi );
            double cosPhi = cos( phi );

            //Calcula os vértices == equacao da esfera
            vertices.push_back( glm::vec3(cosTheta * sinPhi,
                                          cosPhi,
                                          sinTheta * sinPhi) );
        }
    }

    normals = vertices;

    for ( int i=0; i<vertices.size(); i+=3){

           // Shortcuts for vertices
           glm::vec3 v0 = vertices[i+0];
           glm::vec3 v1 = vertices[i+1];
           glm::vec3 v2 = vertices[i+2];

           // Shortcuts for UVs
           glm::vec2 uv0 = texCoords[i+0];
           glm::vec2 uv1 = texCoords[i+1];
           glm::vec2 uv2 = texCoords[i+2];

           // Edges of the triangle : postion delta
           glm::vec3 deltaPos1 = v1-v0;
           glm::vec3 deltaPos2 = v2-v0;

           // UV delta
           glm::vec2 deltaUV1 = uv1-uv0;
           glm::vec2 deltaUV2 = uv2-uv0;

           float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
           glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
           glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

           // Set the same tangent for all three vertices of the triangle.
           // They will be merged later, in vboindexer.cpp
           tangentes.push_back(tangent);
           tangentes.push_back(tangent);
           tangentes.push_back(tangent);
    }


    indices.resize(numTriangles*3);

    auto getIndex = [=]( unsigned int i, unsigned int j, unsigned int s )
    {
        return j + i * ( s + 1 );
    };

    //Preenche o vetor com a triangulação
    unsigned int k = 0;
    for( unsigned int i = 0; i < n; i++ )
    {
        for( unsigned int j = 0; j < m; j++ )
        {
            indices[ k++ ] = getIndex( i, j, n );
            indices[ k++ ] = getIndex( i + 1, j + 1, n );
            indices[ k++ ] = getIndex( i + 1, j, n );

            indices[ k++ ] = getIndex( i, j, n );
            indices[ k++ ] = getIndex( i, j + 1, n );
            indices[ k++ ] = getIndex( i + 1, j + 1, n );
        }
    }
}

void RenderWidget::createPoly()
{
    //TESTAR CODIGOS EM PYTHON
    PolygonalMoulder polyMoulder = PolygonalMoulder();
    //[0,0,0],[0,0,-100], [0,13,-150], [0,50, -187], [0,100,-200]
    std::vector<glm::vec3> poly;
    poly.push_back(glm::vec3(0, 0, 0));
    poly.push_back(glm::vec3(0, 0, -100));
    poly.push_back(glm::vec3(0, 13, -150));
    poly.push_back(glm::vec3(0, 50, -187));
    poly.push_back(glm::vec3(0, 100, -200));

    std::vector<glm::vec3> tri;
    vertices = polyMoulder.createShape(poly, &tri);

    for(glm::vec3 t: tri){
        indices.push_back(t.x);
        indices.push_back(t.y);
        indices.push_back(t.z);
    }


    std::vector<glm::vec3> new_vertices;
    for(unsigned int i = 0; i < vertices.size() - 3; ++i)
    {
        new_vertices.push_back(vertices[i+0]);
        new_vertices.push_back(vertices[i+1]);
        new_vertices.push_back(vertices[i+2]);
        new_vertices.push_back(vertices[i+3]);
    }

    vertices = new_vertices;

    normals = std::vector<glm::vec3>(vertices.size());
    texCoords =  std::vector<glm::vec2>(vertices.size());
    tangentes =  std::vector<glm::vec3>(vertices.size());
}


void RenderWidget::createVBO()
{
    //Construir vetor do vbo
    //OBS: Os dados já poderiam estar sendo armazenados assim na classe.
    struct vertex
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 tangent;
    };

    std::vector< vertex > vbo;
    vbo.reserve( vertices.size() );
    for( unsigned int i = 0; i < vertices.size(); i++ )
    {
        vbo.push_back({vertices[i], normals[i], texCoords[i], tangentes[i]});
    }

    //Criar VBO, linkar e copiar os dados
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vbo.size()*sizeof(vertex), vbo.data(), GL_STATIC_DRAW);
    //Criar EBO, linkar e copiar os dados
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    //Criar VAO, linkar e definir layouts
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //Habilitar, linkar e definir o layout dos buffers
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE,
                           sizeof(vertex),
                           (void*)0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE,
                           sizeof(vertex),
                           (void*)sizeof(glm::vec3) );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE,
                           sizeof(vertex),
                           (void*)(2*sizeof(glm::vec3)) );
    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE,
                           sizeof(vertex),
                           (void*)(sizeof(glm::vec3)) );
    glEnableVertexAttribArray( 4 );
    glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE,
                           sizeof(vertex),
                           (void*)(sizeof(glm::vec3)) );
    //Linkar o EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}


void RenderWidget::createTexture(const std::string& imagePath)
{

    //Criar a textura
    glGenTextures(1, &textureID);

    //Linkar (bind) a textura criada//Linkar (bind) a textura criada
    glBindTexture(GL_TEXTURE_2D, textureID);

    //Abrir arquivo de imagem com o Qt
    QImage texImage = QGLWidget::convertToGLFormat(QImage(imagePath.c_str()));

    //Enviar a imagem para o OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 texImage.width(), texImage.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //Definir parametros de filtro e gerar mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
}
