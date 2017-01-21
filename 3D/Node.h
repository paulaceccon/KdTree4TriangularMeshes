/* 
 * File:   Node.h
 * Author: paulaceccon
 *
 * Created on 10 de Novembro de 2013, 10:46
 */

#ifndef NODE_H
#define	NODE_H

class Node
{
public:
    Node * left, * right;
    long int triangleIndex;
    float minX, minY, maxX, maxY, minZ, maxZ;
    Node( long int triangleIndex, float minX, float minY, float minZ, float maxX, float maxY, float maxZ );
    virtual ~Node( );
private:

};

#endif	/* NODE_H */

