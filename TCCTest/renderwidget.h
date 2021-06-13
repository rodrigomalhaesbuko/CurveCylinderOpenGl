#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QMatrix4x4>

#include <vector>

#include "glm/glm.hpp"

class RenderWidget
        : public QOpenGLWidget
        , protected QOpenGLExtraFunctions
{
public:
    RenderWidget(QWidget* parent = nullptr);
    virtual ~RenderWidget();

private:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    bool gpu = false;
    bool cpuAndGpu = false;

    void setupCPU(const std::vector<glm::vec3> &polyline);
    void setupCPUAndGPU(const std::vector<glm::vec3> &polyline);
    void setupGPU(const std::vector<glm::vec3> &polyline);


    //ArcBall
    int radius;
    glm::ivec2 center;
    glm::ivec2 oldPoint;
    glm::quat pointToQuat(glm::ivec2 screenPoint);
    void rotate(glm::ivec2 p1, glm::ivec2 p2);
    bool arcBallEnable;

    glm::vec3 eyeBall;
    glm::vec3 centerBall;
    glm::vec3 upBall;

    // tesselation
    int numberOfTesselations;
    bool wireframeON;

    // Cylinder Control
    int numberOfPointsInCircle;
    float radiusCircle;


    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);


    void createCube();
    void createQuad();
    void createSphere();
    void createPoly();
    void createVBO();
    void createTexture(const std::string& imagePath);

    void computeNormals();
    void createWireframeTexture();
    void uptadeWireframeTexture(unsigned int textureId, float lineThickness);

    void keyPressEvent(QKeyEvent *event);

    QOpenGLShaderProgram program;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec2 > texCoords;
    std::vector< glm::vec3 > tangentes;
    std::vector< unsigned int > indices;

    glm::mat4x4 model;
    glm::mat4x4 view;
    glm::mat4x4 proj;

    unsigned int textureID;
    unsigned int textureID2;

    //Texture wireframe Id.
    unsigned int wireframeTextureId;
};

#endif // RENDERWIDGET_H
