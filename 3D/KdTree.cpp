/* 
 * File:   KdTree.cpp
 * Author: paulaceccon
 * 
 * Created on 10 de Novembro de 2013, 11:11
 */

#include "KdTree.h"
#include <algorithm>
#include <cstdio>



bool compX( const Triangle &t1, const Triangle &t2 )
{
    return t1._centroidX < t2._centroidX;
}



bool compY( const Triangle &t1, const Triangle &t2 )
{
    return t1._centroidY < t2._centroidY;
}



bool compZ( const Triangle &t1, const Triangle &t2 )
{
    return t1._centroidZ < t2._centroidZ;
}



float findMax( float v1, float v2, float v3 )
{
    return std::max( std::max( v1, v2 ), v3 );
}



float findMin( float v1, float v2, float v3 )
{
    return std::min( std::min( v1, v2 ), v3 );
}



KdTree::KdTree( float * coordinates, int * triangleList, int numTriangles, int numberCoordinatesPerVertex )
{
    _numberCoordinatesPerVertex = numberCoordinatesPerVertex;
    Triangle* triangles = BuildTriangles( coordinates, triangleList, numTriangles );
    _root = buildKdTree( triangles, coordinates, triangleList, 0, numTriangles, 0 );
    delete [] triangles;
}



Triangle* KdTree::BuildTriangles( float * coordinates, int * triangleList, int numTriangles )
{
    Triangle* triangles = new Triangle[numTriangles];
    for (int i = 0; i < numTriangles; i++)
    {
        Triangle t;
        t._triangleIndex = i;
        int index1 = triangleList[i * 3 + 0];
        int index2 = triangleList[i * 3 + 1];
        int index3 = triangleList[i * 3 + 2];

        t._centroidX = (coordinates[index1 * _numberCoordinatesPerVertex + 0] + coordinates[index2 * _numberCoordinatesPerVertex + 0] + coordinates[index3 * _numberCoordinatesPerVertex + 0]) / 3;
        t._centroidY = (coordinates[index1 * _numberCoordinatesPerVertex + 1] + coordinates[index2 * _numberCoordinatesPerVertex + 1] + coordinates[index3 * _numberCoordinatesPerVertex + 1]) / 3;
        t._centroidZ = 0.0;
        
        if (_numberCoordinatesPerVertex == 3)
        {
            t._centroidZ = (coordinates[index1 * _numberCoordinatesPerVertex + 2] + coordinates[index2 * _numberCoordinatesPerVertex + 2] + coordinates[index3 * _numberCoordinatesPerVertex + 2]) / 3;
        }
        triangles[i] = t;
    }
    return triangles;
}



Node * KdTree::buildKdTree( Triangle *triangles, float * coordinates, int * triangleList,
    int begin, int end, int depth )
{
    if (end - begin <= 0)
        return 0;

    int axis = depth % _numberCoordinatesPerVertex;

    // Sorting by axis to insert
    if (axis == 0)
    {
        std::sort( triangles + begin, triangles + end, compX );
    }
    else if (axis == 1)
    {
        std::sort( triangles + begin, triangles + end, compY );
    }
    else if (axis == 2)
    {
        std::sort( triangles + begin, triangles + end, compZ );
    }
    else
    {
        printf( "Axis Error" );
    }

    int half = (begin + end) / 2;
    int triangleIndex = triangles[half]._triangleIndex;

    // Getting the vertices from a triangle
    int index1 = triangleList[triangleIndex * 3 + 0];
    int index2 = triangleList[triangleIndex * 3 + 1];
    int index3 = triangleList[triangleIndex * 3 + 2];

    // Getting it vertices coordinates
    float x1 = coordinates[index1 * _numberCoordinatesPerVertex];
    float x2 = coordinates[index2 * _numberCoordinatesPerVertex];
    float x3 = coordinates[index3 * _numberCoordinatesPerVertex];

    float y1 = coordinates[index1 * _numberCoordinatesPerVertex + 1];
    float y2 = coordinates[index2 * _numberCoordinatesPerVertex + 1];
    float y3 = coordinates[index3 * _numberCoordinatesPerVertex + 1];

    float z1 = 0.0;
    float z2 = 0.0;
    float z3 = 0.0;

    if (_numberCoordinatesPerVertex == 3)
    {
        z1 = coordinates[index1 * _numberCoordinatesPerVertex + 2];
        z2 = coordinates[index2 * _numberCoordinatesPerVertex + 2];
        z3 = coordinates[index3 * _numberCoordinatesPerVertex + 2];
    }

    // Finding it bounding box
    float xMin = findMin( x1, x2, x3 );
    float xMax = findMax( x1, x2, x3 );

    float yMin = findMin( y1, y2, y3 );
    float yMax = findMax( y1, y2, y3 );

    float zMin = findMin( z1, z2, z3 );
    float zMax = findMax( z1, z2, z3 );

    // Creating a node for this triangle
    Node * median = new Node( triangleIndex, xMin, yMin, zMin, xMax, yMax, zMax );
    median->left = buildKdTree( triangles, coordinates, triangleList, begin, half, depth + 1 );
    median->right = buildKdTree( triangles, coordinates, triangleList, half + 1, end, depth + 1 );

    updateBoundingBox( median );

    return median;
}



void KdTree::updateBoundingBox( Node * node )
{
    // Merging its bounding box with its children's one
    if (node->left != 0 && node->right != 0)
    {
        float childrenXMin = findMin( node->minX, node->left->minX, node->right->minX );
        float childrenYMin = findMin( node->minY, node->left->minY, node->right->minY );
        float childrenZMin = findMin( node->minZ, node->left->minZ, node->right->minZ );

        float childrenXMax = findMax( node->maxX, node->left->maxX, node->right->maxX );
        float childrenYMax = findMax( node->maxY, node->left->maxY, node->right->maxY );
        float childrenZMax = findMax( node->maxZ, node->left->maxZ, node->right->maxZ );

        // Updating it bounding box
        node->minX = childrenXMin;
        node->minY = childrenYMin;
        node->minZ = childrenZMin;

        node->maxX = childrenXMax;
        node->maxY = childrenYMax;
        node->maxZ = childrenZMax;
    }
    else
    {
        if (node->left != 0)
        {
            float childrenXMin = std::min( node->minX, node->left->minX );
            float childrenYMin = std::min( node->minY, node->left->minY );
            float childrenZMin = std::min( node->minZ, node->left->minZ );

            float childrenXMax = std::max( node->maxX, node->left->maxX );
            float childrenYMax = std::max( node->maxY, node->left->maxY );
            float childrenZMax = std::max( node->maxZ, node->left->maxZ );

            // Updating it bounding box
            node->minX = childrenXMin;
            node->minY = childrenYMin;
            node->minZ = childrenZMin;

            node->maxX = childrenXMax;
            node->maxY = childrenYMax;
            node->maxZ = childrenZMax;
        }
        else if (node->right != 0)
        {
            float childrenXMin = std::min( node->minX, node->right->minX );
            float childrenYMin = std::min( node->minY, node->right->minY );
            float childrenZMin = std::min( node->minZ, node->right->minZ );

            float childrenXMax = std::max( node->maxX, node->right->maxX );
            float childrenYMax = std::max( node->maxY, node->right->maxY );
            float childrenZMax = std::max( node->maxZ, node->right->maxZ );

            // Updating it bounding box
            node->minX = childrenXMin;
            node->minY = childrenYMin;
            node->minZ = childrenZMin;

            node->maxX = childrenXMax;
            node->maxY = childrenYMax;
            node->maxZ = childrenZMax;
        }
    }
}



KdTree::~KdTree( )
{
    deleteKdTree( _root );
}



void KdTree::deleteKdTree( Node* node )
{
    if (node == 0)
    {
        return;
    }
    deleteKdTree( node->left );
    deleteKdTree( node->right );
    delete node;
}



const Node* KdTree::getRoot( )
{
    return _root;
}