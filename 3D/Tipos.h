
#ifndef _TIPOS_
#define _TIPOS_
#define MAX_TRIANGULOS 1 
#define TIPO float
#define INF 2000000000
#define PI 3.141592
#include<iostream>
using namespace std;

/*--------------------------------------------------------------------------*
Define o tipo camera usado para auxiliar na movimentacao da cena
--------------------------------------------------------------------------*/
struct Camera 
{
    double positionX, positionY, positionZ; //posicao da camera
    double targetX, targetY, targetZ; //alvo da camera, onde olha
    double walkingDirectionX, walkingDirectionY, walkingDirectionZ; //direcao(anda)
    double walkingLateralDirectionX, walkingLateralDirectionY, walkingLateralDirectionZ; //direcao(anda)
    double velocity, lateralVelocity;
    double rotateX, rotateY;
    double velocityRotation;

    /*-99.787116, -196.355437, 141.518071      -98.592712, -194.109091, 139.928313

    0.398134, 0.748782, -0.529919      0.748782, -0.398134, -0.529919

    28.000000, -32.000000*/
    Camera() {
        positionX = -99.787116;
        positionY = -196.355437;
        positionZ = 141.518071;
        targetX = -98.592712;
        targetY = -194.109091;
        targetZ = 139.928313;
        walkingDirectionX = 0.398134;
        walkingDirectionY = 0.748782;
        walkingDirectionZ = -0.529919;
        walkingLateralDirectionX = 0.748782;
        walkingLateralDirectionY = -0.398134;
        walkingLateralDirectionZ = -0.529919;
        velocity = lateralVelocity = 0.0;
        rotateX = 28.000000;
        rotateY = -32.000000;
        velocityRotation = 1;
    }
};
#endif
