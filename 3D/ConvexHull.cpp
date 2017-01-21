/* 
 * File:   ConvexHull.cpp
 * Author: jeferson
 * 
 * Created on 20 de Outubro de 2013, 18:52
 */

#include "ConvexHull.h"
#include "Point2D.h"
#include <algorithm>
#include <string.h>
#include <iostream>

Point2D _pivot;



ConvexHull::ConvexHull( Point2D* points, int numberPoints )
{
    _points = points;
    //_points = new Point2D[numberPoints];
    //memcpy( _points, points, numberPoints * sizeof (Point2D) );
    _numberPoints = numberPoints;
}



bool sortRadial( const Point2D p1, const Point2D p2 )
{
    Vector2D<double> v1 = p1 - _pivot;
    Vector2D<double> v2 = p2 - _pivot;
    return v1 % v2 >= 0.0;
}



void print( Point2D* p, int n )
{
    printf( "\nPontos no fecho\n" );
    const char* a = "";
    for (int i = 0; i < n; i++)
    {
        printf( "%d ", i );
        p[i].print( a );
    }
}



std::vector<Point2D > ConvexHull::grahamScan( )
{
    std::vector<Point2D > convexHull( _numberPoints );
    //busca o pivo
    _pivot = _points[0];
    int indexPivot = 0;
    for (int i = 1; i < _numberPoints; i++)
    {
        if (_pivot < _points[i])
        {
            _pivot = _points[i];
            indexPivot = i;
        }
    }
    //remove o pivot do vetor
    for (int i = indexPivot; i < _numberPoints - 1; i++)
    {
        _points[i] = _points[i + 1];
    }
    //inclui o pivot no feixo convexo
    convexHull[0] = _pivot;

    //diminui o numero de pontos
    _numberPoints--;

    //oderna os pontos
    std::sort( _points, _points + _numberPoints, sortRadial );
    int j = 1;
    for (int i = 0; i < _numberPoints; i++)
    {
        while (j > 1 && ((convexHull[j - 1] - convexHull[j - 2]) % (_points[i] - convexHull[j - 1])) <= 0)
        {
            j--;
        }
        convexHull[j++] = _points[i];
    }
    convexHull.erase( convexHull.begin( ) + j, convexHull.end( ) );

    std::vector<Point2D> convex(j);
    std::copy( &convexHull[0], &convexHull[0] + j, convex.begin( ) );
    return convex;
}



std::vector<Point2D > ConvexHull::mergeConvexHull( )
{
    std::sort( _points, _points + _numberPoints );
    return mergeHull( 0, _numberPoints );
}



std::vector<Point2D > ConvexHull::mergeHull( int initial, int final )
{
    std::vector<Point2D > convex;
    if (fabs( final - initial ) < 3)
    {
        for (int i = initial; i < final; i++)
        {
            convex.push_back( _points[i] );
        }
        return convex;
    }

    int mid = (initial + final) / 2;

    std::vector<Point2D > left = mergeHull( initial, mid );
    std::vector<Point2D > right = mergeHull( mid, final );

    if (left.size( ) + right.size( ) <= 3 && false)
    {
        std::vector<Point2D > convex;
        for (long unsigned int i = 0; i < left.size( ); i++)
        {
            convex.push_back( left[i] );
        }
        for (long unsigned int i = 0; i < right.size( ); i++)
        {
            convex.push_back( right[i] );
        }
        return convex;
    }
    else
    {
        return merge( left, right );
    }
}



std::vector<Point2D > ConvexHull::merge( std::vector<Point2D >& left, std::vector<Point2D >& right )
{

    double area = 0.0;
    //verifica a orientacao
    for (unsigned int i = 0; i < left.size( ); i++)
    {
        area += left[i] % left[(i + 1) % left.size( )];
    }
    if (area < 0)
    {
        std::reverse( left.begin( ), left.end( ) );
    }

    area = 0.0;
    for (unsigned int i = 0; i < right.size( ); i++)
    {
        area += right[i] % right[(i + 1) % right.size( )];
    }
    if (area > 0)
    {
        std::reverse( right.begin( ), right.end( ) );
    }

    int maxElementLef = maxElement( left );
    int maxElementRigh = maxElement( right );
    int minElementLef = minElement( left );
    int minElementRigh = minElement( right );
    for (long unsigned int i = maxElementLef;; i = (i + 1) % left.size( ))
    {
        Vector2D<double> v1 = left[i] - right[maxElementRigh];
        Vector2D<double> v2 = left[(i + 1) % left.size( )] - right[maxElementRigh];
        if (v1 % v2 >= 0)
        {
            maxElementLef = i;
            break;
        }
    }
    for (long unsigned int i = maxElementRigh;; i = (i + 1) % right.size( ))
    {
        Vector2D<double> v1 = right[i] - left[maxElementLef];
        Vector2D<double> v2 = right[(i + 1) % right.size( )] - left[maxElementLef];
        if (v1 % v2 <= 0)
        {
            maxElementRigh = i;
            break;
        }
    }

    for (long unsigned int i = minElementLef;; i = (i + left.size( ) - 1) % left.size( ))
    {
        Vector2D<double> v1 = left[(i + left.size( ) - 1) % left.size( )] - right[minElementRigh];
        Vector2D<double> v2 = left[i] - right[minElementRigh];
        if (v1 % v2 >= 0)
        {
            minElementLef = i;
            break;
        }
    }

    for (long unsigned int i = minElementRigh;; i = (i + right.size( ) - 1) % right.size( ))
    {
        Vector2D<double> v1 = right[i] - left[minElementLef];
        Vector2D<double> v2 = right[(i + right.size( ) - 1) % right.size( )] - left[minElementLef];
        if (v1 % v2 >= 0)
        {
            minElementRigh = i;
            break;
        }
    }
    std::vector<Point2D > convex;
    for (int i = maxElementLef;; i = (i + 1) % left.size( ))
    {
        convex.push_back( left[i] );
        if (i == minElementLef)
            break;
    }
    std::vector<Point2D > conv2;
    for (int i = maxElementRigh;; i = (i + 1) % right.size( ))
    {
        conv2.push_back( right[i] );
        if (i == minElementRigh)
            break;
    }
    if (conv2.size( ) > 0)
    {
        for (int i = (int) conv2.size( ) - 1; i >= 0; i--)
        {
            convex.push_back( conv2[i] );
        }
    }
    return convex;
}



int ConvexHull::maxElement( const std::vector<Point2D >& elements )
{
    Point2D p = elements[0];
    int index = 0;
    for (unsigned int i = 1; i < elements.size( ); i++)
    {
        if (p.y < elements[i].y)
        {
            p = elements[i];
            index = i;
        }
    }
    return index;
}



int ConvexHull::minElement( const std::vector<Point2D >& elements )
{
    Point2D p = elements[0];
    int index = 0;
    for (unsigned int i = 1; i < elements.size( ); i++)
    {
        if (p.y > elements[i].y)
        {
            p = elements[i];
            index = i;
        }
    }
    return index;
}



ConvexHull::~ConvexHull( )
{
    //delete[] _points;
}

