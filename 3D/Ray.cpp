/* 
 * File:   Ray.cpp
 * Author: jeferson
 * 
 * Created on 28 de Novembro de 2013, 15:00
 */

#include "Ray.h"



Ray::Ray( const Point3D& origin, const Vector3D<float>& direction )
{
    //common variables
    _origin = origin;
    _direction = direction;

    _inverseDirection = 1.0f / _direction;

    //ray slope
    _ibyj = _direction.x * _inverseDirection.y;
    _jbyi = _direction.y * _inverseDirection.x;
    _jbyk = _direction.y * _inverseDirection.z;
    _kbyj = _direction.z * _inverseDirection.y;
    _ibyk = _direction.x * _inverseDirection.z;
    _kbyi = _direction.z * _inverseDirection.x;

    _c_xy = _origin.y - _jbyi * _origin.x;
    _c_xz = _origin.z - _kbyi * _origin.x;
    _c_yx = _origin.x - _ibyj * _origin.y;
    _c_yz = _origin.z - _kbyj * _origin.y;
    _c_zx = _origin.x - _ibyk * _origin.z;
    _c_zy = _origin.y - _jbyk * _origin.z;


    //ray slope classification
    if (_direction.x < 0)
    {
        if (_direction.y < 0)
        {
            if (_direction.z < 0)
            {
                _classification = MMM;
            }
            else if (_direction.z > 0)
            {
                _classification = MMP;
            }
            else//(k >= 0)
            {
                _classification = MMO;
            }
        }
        else//(j >= 0)
        {
            if (_direction.z < 0)
            {
                _classification = MPM;
                if (_direction.y == 0)
                {
                    _classification = MOM;
                }
            }
            else//(k >= 0)
            {
                if ((_direction.y == 0) && (_direction.z == 0))
                {
                    _classification = MOO;
                }
                else if (_direction.z == 0)
                {
                    _classification = MPO;
                }
                else if (_direction.y == 0)
                {
                    _classification = MOP;
                }
                else
                {
                    _classification = MPP;
                }
            }
        }
    }
    else//(i >= 0)
    {
        if (_direction.y < 0)
        {
            if (_direction.z < 0)
            {
                _classification = PMM;
                if (_direction.x == 0)
                {
                    _classification = OMM;
                }
            }
            else//(k >= 0)
            {
                if ((_direction.x == 0) && (_direction.z == 0))
                {
                    _classification = OMO;
                }
                else if (_direction.z == 0)
                {
                    _classification = PMO;
                }
                else if (_direction.x == 0)
                {
                    _classification = OMP;
                }
                else
                {
                    _classification = PMP;
                }
            }
        }
        else//(j >= 0)
        {
            if (_direction.z < 0)
            {
                if ((_direction.x == 0) && (_direction.y == 0))
                {
                    _classification = OOM;
                }
                else if (_direction.x == 0)
                {
                    _classification = OPM;
                }
                else if (_direction.y == 0)
                {
                    _classification = POM;
                }
                else
                {
                    _classification = PPM;
                }
            }
            else//(k > 0)
            {
                if (_direction.x == 0)
                {
                    if (_direction.y == 0)
                    {
                        _classification = OOP;
                    }
                    else if (_direction.z == 0)
                    {
                        _classification = OPO;
                    }
                    else
                    {
                        _classification = OPP;
                    }
                }
                else
                {
                    if ((_direction.y == 0) && (_direction.z == 0))
                    {
                        _classification = POO;
                    }
                    else if (_direction.y == 0)
                    {
                        _classification = POP;
                    }
                    else if (_direction.z == 0)
                    {
                        _classification = PPO;
                    }
                    else
                    {
                        _classification = PPP;
                    }
                }
            }
        }
    }
}



Ray::~Ray( )
{
}

