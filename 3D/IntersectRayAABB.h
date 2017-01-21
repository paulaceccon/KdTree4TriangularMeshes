/* 
 * File:   ComputeIntersectRayAABB.h
 * Author: jeferson
 *
 * Created on 28 de Novembro de 2013, 15:30
 */

#ifndef INTERSECTRAYAABB_H
#define	INTERSECTRAYAABB_H
#include "Ray.h"



bool intersectRayAABB( const Ray& r, float x0, float y0, float z0, float x1, float y1, float z1 )
{
    switch (r._classification)
    {
        case MMM:

            if ((r._origin.x < x0) || (r._origin.y < y0) || (r._origin.z < z0)
                || (r._jbyi * x0 - y1 + r._c_xy > 0)
                || (r._ibyj * y0 - x1 + r._c_yx > 0)
                || (r._jbyk * z0 - y1 + r._c_zy > 0)
                || (r._kbyj * y0 - z1 + r._c_yz > 0)
                || (r._kbyi * x0 - z1 + r._c_xz > 0)
                || (r._ibyk * z0 - x1 + r._c_zx > 0)
                )
                return false;

            return true;

        case MMP:

            if ((r._origin.x < x0) || (r._origin.y < y0) || (r._origin.z > z1)
                || (r._jbyi * x0 - y1 + r._c_xy > 0)
                || (r._ibyj * y0 - x1 + r._c_yx > 0)
                || (r._jbyk * z1 - y1 + r._c_zy > 0)
                || (r._kbyj * y0 - z0 + r._c_yz < 0)
                || (r._kbyi * x0 - z0 + r._c_xz < 0)
                || (r._ibyk * z1 - x1 + r._c_zx > 0)
                )
                return false;

            return true;

        case MPM:

            if ((r._origin.x < x0) || (r._origin.y > y1) || (r._origin.z < z0)
                || (r._jbyi * x0 - y0 + r._c_xy < 0)
                || (r._ibyj * y1 - x1 + r._c_yx > 0)
                || (r._jbyk * z0 - y0 + r._c_zy < 0)
                || (r._kbyj * y1 - z1 + r._c_yz > 0)
                || (r._kbyi * x0 - z1 + r._c_xz > 0)
                || (r._ibyk * z0 - x1 + r._c_zx > 0)
                )
                return false;

            return true;

        case MPP:

            if ((r._origin.x < x0) || (r._origin.y > y1) || (r._origin.z > z1)
                || (r._jbyi * x0 - y0 + r._c_xy < 0)
                || (r._ibyj * y1 - x1 + r._c_yx > 0)
                || (r._jbyk * z1 - y0 + r._c_zy < 0)
                || (r._kbyj * y1 - z0 + r._c_yz < 0)
                || (r._kbyi * x0 - z0 + r._c_xz < 0)
                || (r._ibyk * z1 - x1 + r._c_zx > 0)
                )
                return false;

            return true;

        case PMM:

            if ((r._origin.x > x1) || (r._origin.y < y0) || (r._origin.z < z0)
                || (r._jbyi * x1 - y1 + r._c_xy > 0)
                || (r._ibyj * y0 - x0 + r._c_yx < 0)
                || (r._jbyk * z0 - y1 + r._c_zy > 0)
                || (r._kbyj * y0 - z1 + r._c_yz > 0)
                || (r._kbyi * x1 - z1 + r._c_xz > 0)
                || (r._ibyk * z0 - x0 + r._c_zx < 0)
                )
                return false;

            return true;

        case PMP:

            if ((r._origin.x > x1) || (r._origin.y < y0) || (r._origin.z > z1)
                || (r._jbyi * x1 - y1 + r._c_xy > 0)
                || (r._ibyj * y0 - x0 + r._c_yx < 0)
                || (r._jbyk * z1 - y1 + r._c_zy > 0)
                || (r._kbyj * y0 - z0 + r._c_yz < 0)
                || (r._kbyi * x1 - z0 + r._c_xz < 0)
                || (r._ibyk * z1 - x0 + r._c_zx < 0)
                )
                return false;

            return true;

        case PPM:

            if ((r._origin.x > x1) || (r._origin.y > y1) || (r._origin.z < z0)
                || (r._jbyi * x1 - y0 + r._c_xy < 0)
                || (r._ibyj * y1 - x0 + r._c_yx < 0)
                || (r._jbyk * z0 - y0 + r._c_zy < 0)
                || (r._kbyj * y1 - z1 + r._c_yz > 0)
                || (r._kbyi * x1 - z1 + r._c_xz > 0)
                || (r._ibyk * z0 - x0 + r._c_zx < 0)
                )
                return false;

            return true;

        case PPP:

            if ((r._origin.x > x1) || (r._origin.y > y1) || (r._origin.z > z1)
                || (r._jbyi * x1 - y0 + r._c_xy < 0)
                || (r._ibyj * y1 - x0 + r._c_yx < 0)
                || (r._jbyk * z1 - y0 + r._c_zy < 0)
                || (r._kbyj * y1 - z0 + r._c_yz < 0)
                || (r._kbyi * x1 - z0 + r._c_xz < 0)
                || (r._ibyk * z1 - x0 + r._c_zx < 0)
                )
                return false;

            return true;

        case OMM:

            if ((r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.y < y0) || (r._origin.z < z0)
                || (r._jbyk * z0 - y1 + r._c_zy > 0)
                || (r._kbyj * y0 - z1 + r._c_yz > 0)
                )
                return false;

            return true;

        case OMP:

            if ((r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.y < y0) || (r._origin.z > z1)
                || (r._jbyk * z1 - y1 + r._c_zy > 0)
                || (r._kbyj * y0 - z0 + r._c_yz < 0)
                )
                return false;

            return true;

        case OPM:

            if ((r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.y > y1) || (r._origin.z < z0)
                || (r._jbyk * z0 - y0 + r._c_zy < 0)
                || (r._kbyj * y1 - z1 + r._c_yz > 0)
                )
                return false;

            return true;

        case OPP:

            if ((r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.y > y1) || (r._origin.z > z1)
                || (r._jbyk * z1 - y0 + r._c_zy < 0)
                || (r._kbyj * y1 - z0 + r._c_yz < 0)
                )
                return false;

            return true;

        case MOM:

            if ((r._origin.y < y0) || (r._origin.y > y1)
                || (r._origin.x < x0) || (r._origin.z < z0)
                || (r._kbyi * x0 - z1 + r._c_xz > 0)
                || (r._ibyk * z0 - x1 + r._c_zx > 0)
                )
                return false;

            return true;

        case MOP:

            if ((r._origin.y < y0) || (r._origin.y > y1)
                || (r._origin.x < x0) || (r._origin.z > z1)
                || (r._kbyi * x0 - z0 + r._c_xz < 0)
                || (r._ibyk * z1 - x1 + r._c_zx > 0)
                )
                return false;

            return true;

        case POM:

            if ((r._origin.y < y0) || (r._origin.y > y1)
                || (r._origin.x > x1) || (r._origin.z < z0)
                || (r._kbyi * x1 - z1 + r._c_xz > 0)
                || (r._ibyk * z0 - x0 + r._c_zx < 0)
                )
                return false;

            return true;

        case POP:

            if ((r._origin.y < y0) || (r._origin.y > y1)
                || (r._origin.x > x1) || (r._origin.z > z1)
                || (r._kbyi * x1 - z0 + r._c_xz < 0)
                || (r._ibyk * z1 - x0 + r._c_zx < 0)
                )
                return false;

            return true;

        case MMO:

            if ((r._origin.z < z0) || (r._origin.z > z1)
                || (r._origin.x < x0) || (r._origin.y < y0)
                || (r._jbyi * x0 - y1 + r._c_xy > 0)
                || (r._ibyj * y0 - x1 + r._c_yx > 0)
                )
                return false;

            return true;

        case MPO:

            if ((r._origin.z < z0) || (r._origin.z > z1)
                || (r._origin.x < x0) || (r._origin.y > y1)
                || (r._jbyi * x0 - y0 + r._c_xy < 0)
                || (r._ibyj * y1 - x1 + r._c_yx > 0)
                )
                return false;

            return true;

        case PMO:

            if ((r._origin.z < z0) || (r._origin.z > z1)
                || (r._origin.x > x1) || (r._origin.y < y0)
                || (r._jbyi * x1 - y1 + r._c_xy > 0)
                || (r._ibyj * y0 - x0 + r._c_yx < 0)
                )
                return false;

            return true;

        case PPO:

            if ((r._origin.z < z0) || (r._origin.z > z1)
                || (r._origin.x > x1) || (r._origin.y > y1)
                || (r._jbyi * x1 - y0 + r._c_xy < 0)
                || (r._ibyj * y1 - x0 + r._c_yx < 0)
                )
                return false;

            return true;

        case MOO:

            if ((r._origin.x < x0)
                || (r._origin.y < y0) || (r._origin.y > y1)
                || (r._origin.z < z0) || (r._origin.z > z1)
                )
                return false;

            return true;

        case POO:

            if ((r._origin.x > x1)
                || (r._origin.y < y0) || (r._origin.y > y1)
                || (r._origin.z < z0) || (r._origin.z > z1)
                )
                return false;

            return true;

        case OMO:

            if ((r._origin.y < y0)
                || (r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.z < z0) || (r._origin.z > z1)
                )
                return false;

            return true;

        case OPO:

            if ((r._origin.y > y1)
                || (r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.z < z0) || (r._origin.z > z1)
                )
                return false;

            return true;

        case OOM:

            if ((r._origin.z < z0)
                || (r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.y < y0) || (r._origin.y > y1)
                )
                return false;

            return true;

        case OOP:

            if ((r._origin.z > z1)
                || (r._origin.x < x0) || (r._origin.x > x1)
                || (r._origin.y < y0) || (r._origin.y > y1)
                )
                return false;

            return true;

    }

    return false;
}

#endif	/* INTERSECTRAYAABB_H */

