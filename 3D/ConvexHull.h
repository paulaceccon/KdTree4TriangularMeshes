/* 
 * File:   ConvexHull.h
 * Author: jeferson
 *
 * Created on 20 de Outubro de 2013, 18:52
 */

#ifndef CONVEXHULL_H
#define	CONVEXHULL_H

#include "Point2D.h"
#include <vector>

class ConvexHull
{
public:
    /**
     * Construtor quee recebe o conjunto dos pontos e numero de pontos
     * que sera calculado o convexHull
     * @param points - lista de pontos
     * @param numberPoints - numero de pontos
     */
    ConvexHull ( Point2D* points, int numberPoints );

    /**
     * Destrutor
     */
    virtual ~ConvexHull ( );

    /**
     * Calcula o Convex Hull usando Graham Scan
     * @return - pontos do convex hull
     */
    std::vector<Point2D > grahamScan ( );

    /**
     * Calcula o Convex Hull usando merge convex hull
     * @return - pontos do convex hull
     */
    std::vector<Point2D > mergeConvexHull ( );

private:

    std::vector<Point2D > mergeHull ( int initial, int final );

    std::vector<Point2D > merge ( std::vector<Point2D >& left, std::vector<Point2D >& right );
    
    int maxElement( const std::vector<Point2D >& elements );
    
    int minElement( const std::vector<Point2D >& elements );
    
    /**
     * Lista de pontos a ser calculado o convexHull
     */
    Point2D* _points;

    /**
     * Numero de pontos
     */
    int _numberPoints;
};

#endif	/* CONVEXHULL_H */

