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
        //update();
        oldPoint = newpoint;
    }
}
void RenderWidget::wheelEvent(QWheelEvent *event){
        eyeBall += (centerBall - eyeBall) * 0.001f * event->delta();
        view = glm::lookAt(eyeBall,centerBall, upBall);
        //update();
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_G)
    {
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry");
        program.link();
    }

    if(event->key() == Qt::Key_W)
    {
        wireframeON = !wireframeON;
    }

    if(event->key() == Qt::Key_Up)
    {
        numberOfTesselations++;
    }

    if(event->key() == Qt::Key_Down)
    {
        numberOfTesselations--;
    }

    if(event->key() == Qt::Key_A)
    {
        animation = !animation;
    }

    // Fazer poly mais complexa
    if(event->key() == Qt::Key_Space)
    {
        glm::vec3 newpoint =  polyline[polyline.size()-1];
        polyline.push_back(glm::vec3(newpoint.x + 20*glm::cos(newpoint.x),newpoint.y + 20*glm::sin(newpoint.y), newpoint.z -10));
    }

    if(event->key() == Qt::Key_1)
    {
        gpu = false;
        cpuAndGpu = false;
        tesselationOnly = false;
        program.removeAllShaders();
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertex");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry-cpu");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.link();
    }

    if(event->key() == Qt::Key_2)
    {
        gpu = false;
        cpuAndGpu = true;
        tesselationOnly = false;
        program.removeAllShaders();
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertexshader_tess.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry");
        program.link();
    }


    if(event->key() == Qt::Key_3)
    {
        gpu = true;
        cpuAndGpu = false;
        tesselationOnly = false;
        program.removeAllShaders();
        //Create the program
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertexshader_tess.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/Shaders/TEShader.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/Shaders/TCShader.glsl");
        program.link();
    }

    if(event->key() == Qt::Key_4)
    {
        gpu = false;
        cpuAndGpu = false;
        tesselationOnly = true;
        program.removeAllShaders();
        //Create the program
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertexshader_tess.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry-cpu");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/Shaders/TEShaderFull.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/Shaders/TCShaderFull.glsl");
        program.link();
    }

    if(event->key() == Qt::Key_0)
    {
        totalFrameCount = 0;
        frameTime.restart();
    }

    if(gpu)
    {
        setupGPU(polyline);
    }
    else if(cpuAndGpu){

        setupCPUAndGPU(polyline);
    }
    else if(tesselationOnly){

        setupGPUTesselationOnly(polyline);
    }
    else
    {
        setupCPU(polyline);
    }


}


void RenderWidget::ChangePoly(){
    // Animation 1
    for( unsigned int i = 0; i < polyline.size(); i++)
    {
        if(i%2==0)
            polyline[i].y = polyline[i].y + 0.5*glm::cos(angleAnimation);
//        else
//            polyline[i].x = polyline[i].x + -1*glm::cos(angleAnimation);
    }
    angleAnimation += 3.1415f/80.0f;

    // Animation 2
//    if(animationIndex > currentTest.size() - 3){
//        animationIndex = 4;
//        std::vector<glm::vec3> new_poly;
//        new_poly.push_back(currentTest[0]);
//        new_poly.push_back(currentTest[1]);
//        new_poly.push_back(currentTest[2]);
//        new_poly.push_back(currentTest[3]);

//        polyline = new_poly;
//    }
//    else {
//        polyline.push_back(currentTest[animationIndex]);
//        polyline.push_back(currentTest[animationIndex+1]);
//        polyline.push_back(currentTest[animationIndex+2]);
//        polyline.push_back(currentTest[animationIndex+3]);
//        animationIndex+=4;
//    }

     //Animation 3
//    if(animationIndex > polyline.size() - 1)
//    {
//        animationIndex = 0;
//    }else {
//        polyline[animationIndex].y = polyline[animationIndex].y - 10*glm::cos(angleAnimation);
//        animationIndex++;
//        polyline[animationIndex].y = polyline[animationIndex].y + 10*glm::cos(angleAnimation);
//        animationIndex++;
//    }
//    angleAnimation += 3.1415f/80.0f;
    // re build the curve
//    PolygonalMoulder polyMoulder = PolygonalMoulder();
//    std::vector<glm::vec3> tri;
//    std::vector<glm::vec3> controlPoints = polyMoulder.bezierInterpolation(polyline);
    if(gpu)
    {
        setupGPU(polyline);
    }
    else if(cpuAndGpu){

        setupCPUAndGPU(polyline);
    }
    else if(tesselationOnly){

        setupGPUTesselationOnly(polyline);
    }
    else
    {
        setupCPU(polyline);
    }

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


    // TESTES
    currentTest = tccTeste;
    //currentTest = teste0;
    //currentTest = teste1;
    //currentTest = teste2;



//    // Descobrir pontos de controle da Bézier
//    polyline.push_back(glm::vec3(0, 0, -100));
//    polyline.push_back(glm::vec3(0, 20, -150));
////    polyline.push_back(glm::vec3(0, 50, -187));
////    polyline.push_back(glm::vec3(0, 100, -200));
////    polyline.push_back(glm::vec3(10, 120, -180));
////    polyline.push_back(glm::vec3(20, 50, -160));
////    polyline.push_back(glm::vec3(30, 20, -200));
////    polyline.push_back(glm::vec3(40, 13, -230));

//    for( unsigned int i = 0; i < 20; i++)
//    {
//        glm::vec3 newpoint =  polyline[polyline.size()-1];
//        polyline.push_back(glm::vec3(newpoint.x + 10, newpoint.y + 30, newpoint.z));
//        newpoint =  polyline[polyline.size()-1];
//        polyline.push_back(glm::vec3(newpoint.x + 10, newpoint.y + 30, newpoint.z - 30));
//        newpoint =  polyline[polyline.size()-1];
//        polyline.push_back(glm::vec3(newpoint.x + 10, newpoint.y  - 30, newpoint.z - 30));
//        newpoint =  polyline[polyline.size()-1];
//        polyline.push_back(glm::vec3(newpoint.x + 10, newpoint.y  - 30, newpoint.z + 30));
//        newpoint =  polyline[polyline.size()-1];
//        polyline.push_back(glm::vec3(newpoint.x, newpoint.y, newpoint.z + 20));
//    }

//    PolygonalMoulder polyMoulder = PolygonalMoulder();
//    std::vector<glm::vec3> debugPointsBezier = polyMoulder.bezierInterpolation(polyline);

    polyline = currentTest;
    animationIndex = polyline.size();

//    std::cout << polyline.size() << std::endl;

    // Cylinder control
    numberOfPointsInCircle = 16;
    radiusCircle = 10;

    // default value of tesselation
    numberOfTesselations = 4;

    //CRIAR VBO
    createVBO();

    // CPU FLOW
    //setupCPU(polyline);
    // CPU + GEOMETRY FLOW
    //setupCPUAndGPU(polyline);
    // TESSELATION + GEOMETRY FLOW
    //setupGPU(polyline);
    // Only Tesselation
    setupGPUTesselationOnly(polyline);

    //wireframe
    wireframeON = true;

    //animation
    animation = false;

    glm::vec3 eye(0,0,100);
    glm::vec3 verticesMedia;
    for (int i=0;i< vertices.size() ; i++ ) {
        verticesMedia+=vertices[i];
    }

    verticesMedia = verticesMedia/vertices.size();
    glm::vec3 centerM = verticesMedia;
    glm::vec3 up(0,1,0);

    eyeBall = eye;
    centerBall = centerM;
    upBall = up;

    //Definir matriz view e projection
    float ratio = static_cast<float>(width())/height();
    view = glm::lookAt(eye, centerM, up);
    proj = glm::perspective<float>(glm::radians(45.0f), ratio, 0.1, 1000.0f);

    //Setup the wireframe texture
    createWireframeTexture();

    if(gpu)
    {
        //Create the program
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertexshader_tess.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/Shaders/TEShader.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/Shaders/TCShader.glsl");
        program.link();
    }
    else if(cpuAndGpu){
        //Create the program
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertexshader_tess.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry");
        program.link();
    }
    else if(tesselationOnly)
    {
        //Create the program
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertexshader_tess.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry-cpu");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/Shaders/TEShaderFull.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/Shaders/TCShaderFull.glsl");
    }
    else
    {
        //Create the program
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertex");
        program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/geometry-cpu");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment");
        program.link();
    }

    frameTime.start();
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

    program.setUniformValue("light.position", QVector3D(0,0,5) );
    program.setUniformValue("material.ambient", QVector3D(0.2f,0.2f,0.2f));
    program.setUniformValue("material.diffuse", QVector3D(0.6f,0.6f,0.6f));
    program.setUniformValue("material.specular", QVector3D(1.0f,1.0f,1.0f));
    program.setUniformValue("material.shininess", 100.0f);

    //tesselation
    program.setUniformValue("numberOfTesselations", numberOfTesselations);

    // wireFrame
    program.setUniformValue("wireframeON", wireframeON);

    //Cylinder COntrol
    program.setUniformValue("numberOfPointsInCircle", numberOfPointsInCircle);
    program.setUniformValue("radiusCircle", radiusCircle);

    //Ativar e linkar a textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    program.setUniformValue("sampler", 0);

    //Ativar e linkar a textura do wireframe
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture(GL_TEXTURE_1D, wireframeTextureId);
    program.setUniformValue("wireframe", 1);

    QMatrix4x4 sphereModel;
    sphereModel.translate(-centerBall.x,-centerBall.y, -centerBall.z);
    //Passar as matrizes mv e mvp
    QMatrix4x4 mv = v * m * sphereModel;
    QMatrix4x4 mvp = p * mv;
    program.setUniformValue("mv", mv);
    program.setUniformValue("mv_ti", mv.inverted().transposed());
    program.setUniformValue("mvp", mvp);
    program.setUniformValue("inv_p", p.inverted());

    //Desenhar
    if(gpu || tesselationOnly)
    {
        glPatchParameteri( GL_PATCH_VERTICES, 4);
        glDrawArrays(GL_PATCHES, 0, vertices.size());
    }
    else if(cpuAndGpu){
        glDrawArrays(GL_LINES, 0, vertices.size());
    }
    else
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    //Animation
    if(animation){
       ChangePoly();
    }


    if(frameTime.elapsed() > elapsedTime) {
       //fps = frameCount;// FPS;
       elapsedTime += 1000.0f;
       fps = frameCount;
       frameCount = 0;
    }
    if(frameTime.elapsed() > 1000.0f){
       averageFps = totalFrameCount / (float(frameTime.elapsed()) / 1000.0f);
    }

    // DISPLAY
    QPainter myText(this);
    myText.drawText(QPoint(10,50), QString("FPS: ") + QString::number(fps));
    myText.drawText(QPoint(10,70), QString("averageFPS: ") + QString::number(averageFps));
    myText.drawText(QPoint(10,90), QString("Tesselations: ") + QString::number(numberOfTesselations));
    if(gpu)
    {
        myText.drawText(QPoint(10,30),"Solução 3");
        myText.drawText(QPoint(10,110), QString("Triangles: ") + QString::number((vertices.size()/4*2*numberOfPointsInCircle)*numberOfTesselations));
    }
    else if(tesselationOnly){
        myText.drawText(QPoint(10,30),"Solução 4");
        myText.drawText(QPoint(10,110), QString("Triangles: ") + QString::number((vertices.size()/4*2*numberOfPointsInCircle)*numberOfTesselations));
    }
    else if(cpuAndGpu)
    {
        myText.drawText(QPoint(10,30),"Solução 2");
        myText.drawText(QPoint(10,110), QString("Triangles: ") + QString::number(vertices.size()/2*(2*numberOfPointsInCircle)));
    }
    else{
        myText.drawText(QPoint(10,30),"Solução Padrão");
        myText.drawText(QPoint(10,110), QString("Triangles: ") + QString::number(indices.size()/3));
    }
    frameCount++;
    totalFrameCount++;
    // END FPS COUNT

    // call paitGl every frame
    update();
}


void RenderWidget::resizeGL(int w, int h)
{
    //Atualizar a viewport

    //Atualizar a matriz de projeção
}



void RenderWidget::setupCPU(const std::vector<glm::vec3> &polyline)
{

    PolygonalMoulder polyMoulder = PolygonalMoulder();
    std::vector<glm::vec3> tri;
    vertices = polyMoulder.createShape(polyline, &tri, radiusCircle, numberOfPointsInCircle, numberOfTesselations);

    indices.clear();
    for(glm::vec3 t: tri)
    {
        indices.push_back(t.x);
        indices.push_back(t.y);
        indices.push_back(t.z);
    }

    computeNormals();

    //Not using. Erase.
    texCoords = std::vector<glm::vec2>(vertices.size());
    tangentes = std::vector<glm::vec3>(vertices.size());

    //Setup VBO and VAO.
    updateVBO();

    //Set gpu mode off
    gpu = false;
}



void RenderWidget::setupGPU(const std::vector<glm::vec3> &polyline)
{

    //Setup the model.
    //Wrong: vertices should be the bezier control points
    vertices = polyline;
    //vertices = polyline;
    // BÉzier points
    std::vector<glm::vec3> new_vertices;
    // Esse algoritimo pega os pontos da Bézier gerados pelo polygon moulder e cola uma bézier na outra
    for(unsigned int i = 0; i < vertices.size()-3; i = i + 3)
    {
        new_vertices.push_back(vertices[i + 0]);
        new_vertices.push_back(vertices[i + 1]);
        new_vertices.push_back(vertices[i + 2]);
        new_vertices.push_back(vertices[i + 3]);
    }

    vertices = new_vertices;

    //Should be calculated inside the shaders
    normals = std::vector<glm::vec3>(vertices.size());

    //Not using. Erase.
    texCoords = std::vector<glm::vec2>(vertices.size());
    tangentes = std::vector<glm::vec3>(vertices.size());

    //Setup VBO and VAO.
    updateVBO();

    //Set gpu mode on
    gpu = true;
}

void RenderWidget::setupGPUTesselationOnly(const std::vector<glm::vec3> &polyline)
{

    //Setup the model.
    //Wrong: vertices should be the bezier control points
    vertices = polyline;
    //vertices = polyline;
    // BÉzier points
    std::vector<glm::vec3> new_vertices;
    // Esse algoritimo pega os pontos da Bézier gerados pelo polygon moulder e cola uma bézier na outra
    for(unsigned int i = 0; i < vertices.size()-3; i = i + 3)
    {
        new_vertices.push_back(vertices[i + 0]);
        new_vertices.push_back(vertices[i + 1]);
        new_vertices.push_back(vertices[i + 2]);
        new_vertices.push_back(vertices[i + 3]);
    }

    vertices = new_vertices;

    //Should be calculated inside the shaders
    normals = std::vector<glm::vec3>(vertices.size());

    //Not using. Erase.
    texCoords = std::vector<glm::vec2>(vertices.size());
    tangentes = std::vector<glm::vec3>(vertices.size());

    //Setup VBO and VAO.
    updateVBO();

    //Set gpu mode on
    tesselationOnly = true;
}

void RenderWidget::setupCPUAndGPU(const std::vector<glm::vec3> &polyline)
{

    //Setup the model.
    //Wrong: vertices should be the bezier control points
    PolygonalMoulder polyMoulder = PolygonalMoulder();
    std::vector<glm::vec3> tan;
    vertices = polyMoulder.getOnlyPos(polyline, &tan, numberOfTesselations);

    //USING POS
    std::vector<glm::vec3> new_vertices;
    for(unsigned int i = 0; i < vertices.size() - 1; ++i)
    {
        new_vertices.push_back(vertices[i+0]);
        new_vertices.push_back(vertices[i+1]);
    }

    std::vector<glm::vec3> new_tan;
    for(unsigned int i = 0; i < vertices.size() - 1; ++i)
    {
        new_tan.push_back(tan[i+0]);
        new_tan.push_back(tan[i+1]);
    }

    vertices = new_vertices;

    //Should be calculated inside the shaders
    normals = std::vector<glm::vec3>(vertices.size());

    //Not using. Erase.
    texCoords = std::vector<glm::vec2>(vertices.size());
    tangentes = new_tan;

    //Setup VBO and VAO.
    updateVBO();

    //Set gpu mode on
    cpuAndGpu = true;
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


void RenderWidget::createVBO()
{
    //createvbo
    glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        struct vertex
        {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 tangent;
        };
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
    //
}

void RenderWidget::updateVBO()
{
    std::vector< vertex > vbo;
    vbo.reserve( vertices.size() );
    for( unsigned int i = 0; i < vertices.size(); i++ )
    {
        vbo.push_back({vertices[i], normals[i], texCoords[i], tangentes[i]});
    }
    //Criar VBO, linkar e copiar os dados
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vbo.size()*sizeof(vertex), vbo.data(), GL_STATIC_DRAW);
    //Criar EBO, linkar e copiar os dados
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
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



void RenderWidget::computeNormals()
{
    normals = std::vector<glm::vec3>(vertices.size(), {0.0f, 0.0f, 0.0f});

    auto numTriangles = indices.size() / 3;

    //Compute the normals of every triangle and accumulate it in the vertices
    for(unsigned int i = 0; i < numTriangles; ++i)
    {
        //The triangle vertex indices
        unsigned int i0 = indices[i * 3 + 0];
        unsigned int i1 = indices[i * 3 + 1];
        unsigned int i2 = indices[i * 3 + 2];

        //The coordinates of each vertex
        glm::vec3 v0 = vertices[i0];
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];

        //Compute the triangle normal
        glm::vec3 n = glm::cross(v1 - v0, v2 - v0);

        //Accumulate n in every vertex normal
        normals[i0] += n;
        normals[i1] += n;
        normals[i2] += n;
    }

    //Normalize the normals
    for(auto &n : normals)
    {
        n = glm::normalize(n);
    }
}



void RenderWidget::createWireframeTexture()
{
    //Create a new texture.
    glGenTextures( 1, &wireframeTextureId );

    //Update the texture with correct thickness.
    uptadeWireframeTexture(wireframeTextureId, 1.0f);
}



void RenderWidget::uptadeWireframeTexture(unsigned int textureId, float lineThickness)
{
    //Build texture vector.
    const int TAM = 1024;
    unsigned char wireframeTexture[TAM];
    memset( wireframeTexture, 0, TAM * sizeof (unsigned char ) );

    int idx = 0;
    while (lineThickness > 1.0f && idx < TAM)
    {
        wireframeTexture[TAM - 1 - idx] = 255;
        lineThickness = lineThickness - 1.0f;
        idx++;
    }

    if (idx < TAM)
    {
        wireframeTexture[TAM - 1 - idx] = static_cast<unsigned char>(255 * lineThickness);
    }

    //Turn the texture as current.
    glBindTexture( GL_TEXTURE_1D, textureId );

    //Build mipmap pyramid.
    int i = 0, j = TAM;
    for (i = 0, j = TAM; j > 4; j /= 2, i++)
    {
        glTexImage1D( GL_TEXTURE_1D, i, GL_RED, j, 0, GL_RED, GL_UNSIGNED_BYTE, wireframeTexture + ( TAM - j ) );
    }

    //Avoiding image saturation for rendering dense meshes.
    for (int s = 0; s <= idx; s++)
    {
        wireframeTexture[TAM - 1 - s] /= 3;
    }

    for (; j > 0; j /= 2, i++)
    {
        glTexImage1D( GL_TEXTURE_1D, i, GL_RED, j, 0, GL_RED, GL_UNSIGNED_BYTE, wireframeTexture + ( TAM - j ) );
    }

    //Define filters.
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
}
