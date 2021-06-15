#ifndef POLYGONALMOULDER_H
#define POLYGONALMOULDER_H

#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include <iterator>



class PolygonalMoulder
{
public:
    PolygonalMoulder();
    std::vector<glm::vec3> bezierInterpolation(std::vector<glm::vec3> poly);
    std::vector<glm::vec3> getOnlyPos(std::vector<glm::vec3> poly,std::vector<glm::vec3> *tangent , int ni);
    std::vector<glm::vec3> createShape(std::vector<glm::vec3> poly, std::vector<glm::vec3> *tri , float radius, int numberOfPoints, int ni);
    void rotaciona(glm::vec3 u0, glm::vec3 v0, glm::vec3 t0, glm::vec3 t1, glm::vec3 *u1, glm::vec3 *v1);
    void getPosAndTan( std::vector<glm::vec3> v, int ni, std::vector<glm::vec3> *pos, std::vector<glm::vec3> *tan);
    std::vector<glm::vec3> createVertices(std::vector<glm::vec3> pos,
                                          std::vector<glm::vec3> u,
                                          std::vector<glm::vec3> v,
                                          std::vector<glm::vec3> tan,
                                          float radius, int numberOfPoints);
    std::vector<glm::vec3> createCircle(float radius, int numberOfPoints);
    std::vector<glm::vec3> createTriangles(int pointsInCircle, int numberOfCircles);
    void printVector3(std::vector<glm::vec3> v);
    int numberOfPointsInCircle;
    float radiusCircle;
};

#endif // POLYGONALMOULDER_H


