/* 
 * File:   IntersectRayTriangle.h
 * Author: jeferson
 *
 * Created on 28 de Novembro de 2013, 16:48
 */

#ifndef INTERSECTRAYTRIANGLE_H
#define	INTERSECTRAYTRIANGLE_H

#include "Ray.h"



/**
 * 
 * @param r
 * @param x1
 * @param y1
 * @param z1
 * @param x2
 * @param y2
 * @param z2
 * @param x3
 * @param y3
 * @param z3
 * @return 0 - nao intercepte, 1 caso intercepte, 2 caso intercepte a borda
 */
short int intersectRayTriangle( const Ray& r, float x1, float y1, float z1,
    float x2, float y2, float z2, float x3, float y3, float z3 )
{
    double eps = 10E-8;

    //vertices do triangulo
    Point3D A( x1, y1, z1 );
    Point3D B( x2, y2, z2 );
    Point3D C( x3, y3, z3 );

    Vector3D<float> normal = (B - A) % (C - A);
    double d = -A * normal;

    double den = normal * r._direction;
    if (fabs( den ) < eps)
    {
        return 0;
    }
    double t = -(normal * r._origin + d) / (den);
    if (t < 0)
    {
        return 0;
    }

    Point3D intersection = r._origin + t * r._direction;

    Vector3D<float> v1 = (B - intersection) % (C - intersection);
    Vector3D<float> v2 = (intersection - A) % (C - A);
    Vector3D<float> v3 = (B - A) % (intersection - A);

    normal.normalize( );
    
    //verifica se o ponto esta dentro do triangulo
    double l1 = normal * (v1);
    double l2 = normal * (v2);
    double l3 = normal * (v3);

    if (l1 > eps && l2 > eps && l3 > eps)
    {
        return 1;
    } else if (fabs( l1 ) <= eps || fabs( l2 ) <= eps || fabs( l3 ) <= eps)
    {
        return 2;
    } else
    {
        return 0;
    }

}


#endif	/* INTERSECTRAYTRIANGLE_H */

