#include "polygonalmoulder.h"
#include "glm/glm.hpp"
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include<math.h>
#include <Eigen/Dense>
 #include <QStyle>

using namespace Eigen;

PolygonalMoulder::PolygonalMoulder()
{

}


// Funcao que cria o molde a partir de uma poligonal
// Recebe a poligonal
// Retorna uma lista de triangulos para serem renderizados
std::vector<glm::vec3> PolygonalMoulder::createShape(std::vector<glm::vec3> poly, std::vector<glm::vec3> *tri ){

    // se interpola a curva por Bezier
    std::vector<glm::vec3> polyInterpolated = bezierInterpolation(poly);
    std::cout << "BIINTERPOLADO VALORES DE V" << std::endl;
    printVector3(polyInterpolated);

    // com a curva interpolada precisamos das posicoes e das tangentes
    std::vector<glm::vec3> pos, tan;
    int ni = 4;
    getPosAndTan(polyInterpolated, ni, &pos, &tan);
    std::cout << "pos" << std::endl;
    printVector3(pos);

    std::cout << "tan" << std::endl;
    printVector3(tan);

    // com as informações gera o conjunto de vertices da estrutura
    std::vector<glm::vec3> vertices;
    int numberOfPointsInCircle = 10;
    float radiusCircle = 10;
    // descobrir u
    std::vector<glm::vec3> u;
    // descobrir v
    std::vector<glm::vec3> v;
    // caso base de u e v
    glm::vec3 u_ = glm::vec3(1, 0 ,0 );
    glm::vec3 t0 = tan[0];


    glm::vec3 v0 = glm::normalize(glm::cross(t0,u_));

    glm::vec3 u0 = glm::cross(v0,t0);

    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);
//    u.push_back(u0);

    v.push_back(v0);
//    v.push_back(glm::vec3( 0,    -1.   ,  0.015));
//    v.push_back(glm::vec3( 0,    -1.   ,  0.005));
//    v.push_back(glm::vec3( 0,    -1.   , -0.015));
//    v.push_back(glm::vec3( 0,    -0.999, -0.054));
//    v.push_back(glm::vec3( 0,    -0.991, -0.13 ));
//    v.push_back(glm::vec3( 0,    -0.967, -0.255));
//    v.push_back(glm::vec3( 0,    -0.914, -0.406));
//    v.push_back(glm::vec3( 0,    -0.849, -0.528));
//    v.push_back(glm::vec3( 0,    -0.79 , -0.613));
//    v.push_back(glm::vec3( 0,    -0.718, -0.696));
//    v.push_back(glm::vec3( 0,    -0.624, -0.781));
//    v.push_back(glm::vec3( 0,    -0.489, -0.872));
//    v.push_back(glm::vec3( 0,    -0.337, -0.942));
//    v.push_back(glm::vec3( 0,    -0.22 , -0.975));
//    v.push_back(glm::vec3( 0,    -0.15 , -0.989));
//    v.push_back(glm::vec3( 0,    -0.126, -0.992));



    /*u_ = np.array([1,0,0])
    t0 = t[0]
    v0 = unitario(np.cross(t0, u_))
    u0 = np.cross(v0,t0)*/

     //Apenas com o vetor tangente descobrir os eixos u e v proximos
    for(int i = 1; i < (int)tan.size(); i++){
        glm::vec3 vi;
        glm::vec3 ui;
        rotaciona(u[i-1], v[i-1], tan[i-1], tan[i], &ui, &vi);
        u.push_back(ui);
        v.push_back(vi);
    }

    std::cout << "U" << std::endl;
    printVector3(u);

    std::cout << "V" << std::endl;
    printVector3(v);

    vertices = createVertices(pos, u, v, tan, radiusCircle, numberOfPointsInCircle);

    // realiza o mapeamento para a malha de triangulos
    *tri = createTriangles(numberOfPointsInCircle, tan.size());

    std::cout << "Vector" << std::endl;
    printVector3(vertices);

    std::cout << "Triangulo" << std::endl;
    printVector3(*tri);

    return polyInterpolated;
    //return pos;

}

void PolygonalMoulder::printVector3(std::vector<glm::vec3> v){
    int tam = v.size();
    for(int i=0; i < tam; i++){
        std::cout << "[ " << v[i].x << ", " << v[i].y << ", " <<  v[i].z <<  "]," << std::endl;
    }

}

//Interpola a curva
//Recebe a polygonal
std::vector<glm::vec3> PolygonalMoulder::bezierInterpolation(std::vector<glm::vec3> poly){

  std::vector<glm::vec3> v;
  int n = poly.size(); // numero de pontos
  int nt = n-1; // numero de tramos
  int m = 3*nt + 1; // numero de pontos de interpolaçao da bezier
  MatrixXd A = MatrixXd(m,m);
  MatrixXd B =  MatrixXd(m,3);
  A = MatrixXd::Zero(m,m);
  B =  MatrixXd::Zero(m,3);

  // caso base
  int row = 0;
  A(row,0) = 1;
  B(row,0) = poly[0].x;
  B(row,1) = poly[0].y;
  B(row,2) = poly[0].z;

  // segundo passo tem os valore para A de [1, -2, 1]
  row++;
  A(row,0) = 1;
  A(row,1) = -2;
  A(row,2) = 1;

  for(int i = 1; i<nt; i++){
      int j = 3*i;
      row++;
      A(row,j-1) = 1;
      A(row,j) = -2;
      A(row,j+1) = 1;

      row++;
      B(row,0) = poly[i].x;
      B(row,1) = poly[i].y;
      B(row,2) = poly[i].z;
      A(row,j) = 1;

      row++;
      A(row,j-2) = 1;
      A(row,j-1) = -2;
      A(row,j) = 0;
      A(row,j+1) = 2;
      A(row,j+2) = -1;
  }

  // final
  row++;
  A(row,m-3) = 1;
  A(row,m-2) = -2;
  A(row,m-1) = 1;

  row++;
  A(row, m-1) = 1;
  B(row,0) = poly[n-1].x;
  B(row,1) = poly[n-1].y;
  B(row,2) = poly[n-1].z;



  // RESOLVER O SISTEMA DE AX = B => Ax0 = B0, Ax1 = B1, Ax2 = B2. Usando o eigen

  VectorXd b0 = VectorXd(m);
  VectorXd b1 = VectorXd(m);
  VectorXd b2 = VectorXd(m);

  // separa B
  for(int i=0; i < m; i++){
      b0(i) = B(i,0);
      b1(i) = B(i,1);
      b2(i) = B(i,2);
  }

  VectorXd x0 = VectorXd(m);
  VectorXd x1 = VectorXd(m);
  VectorXd x2 = VectorXd(m);

  x0 = A.fullPivLu().solve(b0);
  x1 = A.fullPivLu().solve(b1);
  x2 = A.fullPivLu().solve(b2);

  for(int i=0; i < m; i++){
      v.push_back(glm::vec3(x0(i),x1(i),x2(i)));
  }

 return v;

    /* n = p.shape[0]
        nt = n-1
        m = 3*nt + 1
        A = np.zeros(shape=(m,m),dtype= np.float64)
        B = np.zeros(shape=(m,3),dtype= np.float64)

        row=0
        B[row,:] = p[0]
        A[row,0] = 1
        row+=1
        B[row,:] = [0,0,0]
        A[row,0:3] = [1, -2, 1]

        for i in range(1, nt):
            j=3*i
            row+=1
            B[row,:] = [0,0,0]
            A[row,j-1:j+2] = [1, -2, 1]

            row+=1
            B[row,:] = p[i]
            A[row,j] = 1

            row+=1
            A[row, j-2:j+3] = [1,-2,0,2,-1]
            B[row, :] = [0,0,0]
        row+=1
        A[row, m-3:m]= [1, -2, 1]
        B[row,:] = [0,0,0]
        row+=1
        A[row, m-1]= 1
        B[row,:] = p[n-1]
        print(A, "\n", B)
        #resolve
        v = la.solve(A,B)
    */
}


void PolygonalMoulder::rotaciona(glm::vec3 u0, glm::vec3 v0, glm::vec3 t0, glm::vec3 t1, glm::vec3 *u1, glm::vec3 *v1){
    glm::vec3 eixo = glm::cross(t0,t1);
    if(glm::length(eixo) >= 0.000000001){
        eixo = glm::normalize(eixo);
        float angle = glm::acos(t0[0]*t1[0]+t0[1]*t1[1]+t0[2]*t1[2]);
        //rotacionar o quartenio entorno do eixo
        glm::quat quat_rot =  glm::quat(glm::cos(angle/2), glm::sin(angle/2)*eixo);
        glm::quat quat_u0 = glm::quat(0.0f, u0);
        glm::quat quat_v0 = glm::quat(0.0f, v0);
        glm::quat quat_u1  = quat_rot*quat_u0*glm::conjugate(quat_rot);
        *u1 = glm::vec3(quat_u1.x, quat_u1.y, quat_u1.z);
        glm::quat quat_v1  = quat_rot*quat_v0*glm::conjugate(quat_rot);
        *v1 = glm::vec3(quat_v1.x, quat_v1.y, quat_v1.z);

    }
    /*def rot_quaternio(u0,v0,t0,t1):
        eixo = np.cross(t0,t1)
        if norma(eixo)>EPS:
            eixo = unitario(eixo)
            angulo = math.acos(dot(t0,t1))
            quat = Quaternion(axis=eixo, angle=angulo)
            u1 = quat.rotate(u0)
            v1 = quat.rotate(v0)
        else:
            u1=u0
            v1=v0

        return u1,v1*/
}

void PolygonalMoulder::getPosAndTan( std::vector<glm::vec3> v, int ni, std::vector<glm::vec3> *pos, std::vector<glm::vec3> *tan){

        int m = v.size();
        int nt = (int((m-1)/3) + 1) - 1;
        for(int i=0; i< nt; i++){
            int j  = 3*i;
            glm::vec3 v0 = v[j];
            glm::vec3 v1 = v[j + 1];
            glm::vec3 v2 = v[j + 2];
            glm::vec3 v3 = v[j + 3];
            float taux = 0;
            for(int t = 0; t < ni; t++){
                float u = 1-taux ;
                glm::vec3 position = u*u*u*v0 + 3*u*u*taux*v1 + 3*u*taux*taux*v2 + taux*taux*taux*v3;
                glm::vec3 velocity = -3*u*u*v0 + 3*(3*taux*taux - 4*taux + 1)*v1 + 3*(-3*taux*taux + 2*taux)*v2 + 3*taux*taux*v3;
                // fazer unitario de velocity
                pos->push_back(position);
                tan->push_back(glm::normalize(velocity));
                taux = taux + 1.0/float(ni);
            }

        }

        // adicionando posição e velocidade finais
        pos->push_back(v[m-1]);
        tan->push_back(glm::normalize(v[m-1] - v[m-2]));


    /*m = vertices.shape[0]
        n = (m-1)//3+1
        nt = n - 1
        pos = []
        tan = []
        for trecho in range(nt):
            j = 3*trecho
            v0 = v[j]
            v1 = v[j + 1]
            v2 = v[j + 2]
            v3 = v[j + 3]
            for t in np.linspace(0,1,ni,endpoint=False):
                u = 1-t
    #             pt = ((1-t)**3)*v0 + 3*((1-t)**2)*t*v1 + 3*(1-t)*(t**2)*v2 + t*t*t*v3
                pt = u*u*u*v0 + 3*u*u*t*v1 + 3*u*t*t*v2 + t*t*t*v3
                pt_d = -3*u*u*v0 + 3*(3*t*t - 4*t + 1)*v1 + 3*(-3*t*t + 2*t)*v2 + 3*t*t*v3
                pt_d = unitario(pt_d)
                pos.append(pt)
                tan.append(pt_d)

        #adding last positions
        pos.append(v[m-1])
        tan.append(unitario(v[m-1] - v[m-2]))

        return np.array(pos), np.array(tan)
        */
}

std::vector<glm::vec3> PolygonalMoulder::createCircle(float radius, int numberOfPoints){
    std::vector<glm::vec3> circlePoints;
    float angle = 0;
    for(int i = 0; i < numberOfPoints; i++){
        float u = radius*glm::cos(angle);
        float v = radius*glm::sin(angle);
        float t = 0;
        circlePoints.push_back(glm::vec3(u,v,t));
        angle = angle + 2*M_PI/numberOfPoints;

    }

    return circlePoints;
    /*for angle in np.linspace(0,2*np.pi,nptos, endpoint=False):
        u = raio*math.cos(angle)
        v = raio*math.sin(angle)
        t = 0
        p_.append([u,v,t])
    */

}

std::vector<glm::vec3> PolygonalMoulder::createVertices(std::vector<glm::vec3>  pos, std::vector<glm::vec3>  u,std::vector<glm::vec3>  v, std::vector<glm::vec3>  tan, float radius, int numberOfPoints){
     std::vector<glm::vec3> vertices;
     std::vector<glm::vec3> circle = createCircle(radius, numberOfPoints);

     for(int i=0; i<int(pos.size()); i++){
         glm::vec3 posi = pos[i];
         glm::vec3 ui = u[i];
         glm::vec3 vi = v[i];
         glm::vec3 ti = tan[i];

         std::vector<glm::vec3> R;
         R.push_back(glm::vec3(ui[0],ui[1],ui[2]));
         R.push_back(glm::vec3(vi[0],vi[1],vi[2]));
         R.push_back(glm::vec3(ti[0],ti[1],ti[2]));

         // juntar com os pontos do circulo
         for(int j=0; j<int(circle.size()); j++){
             glm::vec3 circleJ = circle[j];
             glm::vec3 point = posi + circleJ[0]*R[0]+circleJ[1]*R[1]+circleJ[2]*R[2];
             vertices.push_back(point);
         }
     }

     return vertices;

     // ALINHAR OS

     /*
       nsec = o.shape[0]
         for i in range(nsec):
             oi = o[i,:]
             ui = u[i,:]
             vi = v[i,:]
             ti = t[i,:]

             R = np.array([[ui[0], ui[1], ui[2]]
                           ,[vi[0], vi[1], vi[2]],
                            [ti[0], ti[1], ti[2]]])
             for j in range(p_.shape[0]):
                 p_j = p_[j,:]
                 point = oi + dot(p_j, R)
                 vert.append(point)

         return np.array(vert)
     */
}




std::vector<glm::vec3> PolygonalMoulder::createTriangles(int pointsInCircle, int numberOfCircles){
   std::vector<glm::vec3> tri;
   for(int j = 0; j< numberOfCircles-1; j++){
       for(int i = 0; i< pointsInCircle; i++){
           int k = j*10 + i;
           int k1 = j*10 + (i+1)%10;
           tri.push_back(glm::vec3(k,k1,k1+10));
           tri.push_back(glm::vec3(k,k1+10,k+10));
       }
   }
    /*
    for j in range(ns-1):
            for i in range(m):
                k = j*m + i
                k1 = j*m + (i + 1)%m
                tri.append([k, k1, k1 + m])
                tri.append([k, k1 + m, k + m])
     */

    return tri;
}






