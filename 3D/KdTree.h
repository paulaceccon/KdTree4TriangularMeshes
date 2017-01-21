/* 
 * File:   KdTree.h
 * Author: paulaceccon
 *
 * Created on 10 de Novembro de 2013, 11:11
 */

#ifndef KDTREE_H
#define	KDTREE_H

#include <vector>
#include "Node.h"

struct Triangle
{
    int _triangleIndex;
    float _centroidX;
    float _centroidY;
    float _centroidZ;
};



class KdTree
{
public:
    Triangle* BuildTriangles( float * coordinates, int * triangleList, int numTriangles );
    KdTree( float * coordinates, int * triangleList, int numTriangles, int rSpace );
    void PrintKdTree( Node * root );
    virtual ~KdTree( );
    const Node* getRoot();
private:

    Node * _root;
    int _numberCoordinatesPerVertex;
    
    void deleteKdTree( Node* node );
    void updateBoundingBox( Node * node );
    Node * buildKdTree( Triangle *triangles, float * coordinates, int * triangleList,
        int begin, int end, int depth);
};

#endif	/* KDTREE_H */

