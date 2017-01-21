/* 
 * File:   PointInPolygon.cpp
 * Author: jeferson
 * 
 * Created on 19 de Outubro de 2013, 11:05
 */

#include "PointInPolygon.h"



bool equal( double val1, double val2, double tol = 10E-5 )
{
    if (fabs( val1 - val2 ) < tol)
        return true;
    return false;
}



PointInPolygon::PointInPolygon( Point2D* polygon, int numberPoints )
{
    _polygon = polygon;
    _numberPoints = numberPoints;
}



PointInPolygon::~PointInPolygon( )
{
}



short PointInPolygon::rayCrossings( const Point2D& p )
{
    _testPoint = p;
    int cross = 0;
    for (int i = 0; i < _numberPoints; i++)
    {
        int next = (i + 1) % _numberPoints;
        if (!equal( _polygon[i].y, _polygon[next].y, Point2D::eps ))
        {
            Point2D intersection;
            if (computeIntersectionRay( _polygon[i], _polygon[next], intersection ))
            {
                if (equal( _testPoint.x, intersection.x, Point2D::eps ))
                {
                    return 0;
                }
                else
                {
                    if (intersection.x > _testPoint.x && intersection.y > std::min( _polygon[i].y, _polygon[next].y ))
                    {
                        cross++;
                    }
                }
            }
        }
        else
        {
            if (intersectEdge( _polygon[i], _polygon[next] ))
            {
                return 0;
            }
        }
    }
    if (cross % 2 == 1)
    {
        return -1;
    }
    return 1;
}



short PointInPolygon::rotationIndex( const Point2D& p )
{
    _testPoint = p;
    double a = 0.0;
    for (int i = 0; i < _numberPoints; i++)
    {
        int next = (i + 1) % _numberPoints;
        if (intersectEdge( _polygon[i], _polygon[next] ))
        {
            return 0;
        }
        a += angle( _polygon[i], p, _polygon[next] );
    }
    if (equal( a, 0.0 ))
        return 1;
    return -1;
}



bool PointInPolygon::intersectEdge( const Point2D& p1, const Point2D& p2 )
{
    //para está sobre a aresta o roduto vetorial deve ser zero e o produto escalar
    //deve ser menor ou igual a zero.
    Vector2D<double> v1 = p1 - _testPoint;
    Vector2D<double> v2 = p2 - _testPoint;
    return equal( v1 % v2, 0.0,  Point2D::eps ) && v1 * v2 <= 0.0;
}



bool PointInPolygon::computeIntersectionRay( const Point2D& p1, const Point2D& p2, Point2D& intersection )
{
    //calcula o parametro da interpolacao linear que gera o ponto
    //de intersecao do segmento
    double s = (_testPoint.y - p1.y) / (p2.y - p1.y);

    //verifica se a intersecao esta dentro do segmento
    if (s >= 0.0 && s <= 1.0)
    {
        intersection = (1 - s) * p1 + s * p2;
        return true;
    }
    return false;
}



double PointInPolygon::angle( const Point2D& p, const Point2D& q, const Point2D& r )
{
    Vector2D<double> u = p - q;
    Vector2D<double> v = r - q;
    //    return atan2( u % v, u * v );
    double ang1 = pseudoAngle( u );
    double ang2 = pseudoAngle( v );
    double ang = fabs( ang1 - ang2 );
    ang = ang > 4.0 ? 8 - ang : ang;
    int sinal = u % v > 0 ? 1 : -1;
    return sinal * ang;
}



double PointInPolygon::pseudoAngle( const Vector2D<double>& v )
{
    if (v.y >= 0)
    {
        if (v.x >= 0)
        {
            if (v.x >= v.y)
            {
                return ( v.y / v.x);
            }
            else
            {
                return ( 2 - v.x / v.y);
            }
        }
        else
        {
            if (v.x >= -v.y)
            {
                return ( 2 - v.x / v.y);
            }
            else
            {
                return ( 4 + v.y / v.x);
            }
        }
    }
    else
    {
        if (v.x >= 0)
        {
            if (v.x >= -v.y)
            {
                return ( v.y / v.x);
            }
            else
            {
                return ( -2 - v.x / v.y);
            }
        }
        else
        {
            if (v.x >= v.y)
            {
                return ( -2 - v.x / v.y);
            }
            else
            {
                return ( -4 + v.y / v.x);
            }
        }
    }
}