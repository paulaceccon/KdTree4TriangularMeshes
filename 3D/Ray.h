/* 
 * File:   Ray.h
 * Author: jeferson
 *
 * Created on 28 de Novembro de 2013, 15:00
 */

#ifndef RAY_H
#define	RAY_H

#include "Vector3D.h"

enum CLASSIFICATION
{
    MMM, MMP, MPM, MPP, PMM, PMP, PPM, PPP, POO, MOO, OPO, OMO,
    OOP, OOM, OMM, OMP, OPM, OPP, MOM, MOP, POM, POP, MMO, MPO,
    PMO, PPO
};

class Ray
{
public:
    /**
     * Construtor do raio que recebe a origem e direcao do mesmo
     * @param origin - origem do raio
     * @param direction - direcao
     */
    Ray( const Point3D& origin, const Vector3D<float>& direction );

    /**
     * Destrutor do raio
     */
    virtual ~Ray( );

    /**
     * Origem do raio
     */
    Point3D _origin;

    /**
     * Direcao do raio
     */
    Vector3D<float> _direction;

    /**
     * Direcao inversa das componentes
     */
    Point3D _inverseDirection;


    CLASSIFICATION _classification;

    float _ibyj, _jbyi, _kbyj, _jbyk, _ibyk, _kbyi; //slope
    float _c_xy, _c_xz, _c_yx, _c_yz, _c_zx, _c_zy;
    
private:
};

#endif	/* RAY_H */

