/* 
 * File:   Node.cpp
 * Author: paulaceccon
 * 
 * Created on 10 de Novembro de 2013, 10:46
 */

#include "Node.h"



Node::Node( long int triangleIndex, float minX, float minY, float minZ,
            float maxX, float maxY, float maxZ )
{
    this->triangleIndex = triangleIndex;
    this->left = 0;
    this->right = 0;
    this->minX = minX;
    this->minY = minY;
    this->maxX = maxX;
    this->maxY = maxY;
    this->minZ = minZ;
    this->maxZ = maxZ;
}



Node::~Node( )
{
}

