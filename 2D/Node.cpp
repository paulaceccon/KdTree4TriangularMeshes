/* 
 * File:   Node.cpp
 * Author: paulaceccon
 * 
 * Created on 10 de Novembro de 2013, 10:46
 */

#include "Node.h"



Node::Node( long int triangle_index, float min_x, float min_y, float min_z,
            float max_x, float max_y, float max_z )
{
    this->triangle_index = triangle_index;
    this->left = 0;
    this->right = 0;
    this->min_x = min_x;
    this->min_y = min_y;
    this->max_x = max_x;
    this->max_y = max_y;
    this->min_z = min_z;
    this->max_z = max_z;
}



Node::~Node( )
{
}

