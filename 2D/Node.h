/* 
 * File:   Node.h
 * Author: paulaceccon
 *
 * Created on 10 de Novembro de 2013, 10:46
 */

#ifndef NODE_H
#define NODE_H

class Node 
{
    public:

        /** Left and right nodes **/
        Node * left, * right;

        /** Triangle index **/
        long int triangle_index;

        /** Bounding box **/
        float min_x, min_y, max_x, max_y, min_z, max_z;

        /**
         * Instantiates a point with x, y, z coordinates.
         * 
         * @param triangleIndex
         * @param min_x Minimum coordinate in x.
         * @param min_y Minimum coordinate in y.
         * @param max_x Maximum coordinate in x.
         * @param max_y Maximum coordinate in y.
         * @param min_z Minimum coordinate in z.
         * @param max_z Maximum coordinate in z.
         */
        Node(long int triangleIndex, float min_x, float min_y, float max_x, float max_y, float min_z, float max_z);

        virtual ~Node();
    private:

};

#endif /* NODE_H */

