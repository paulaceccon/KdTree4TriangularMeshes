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

/** Struct that define a triangle **/
struct Triangle
{
    long int triangleCode;  /** Triangle ID   **/
    float centroid_x;       /** Centroid in x **/
    float centroid_y;       /** Centroid in y **/
    float centroid_z;       /** Centroid in z **/
};
    
class KdTree
{
public:
    
    /** Vector of Triangles **/
    std::vector<Triangle> triangles;
    
    /** Root of the kd-tree **/
    Node * root;
    
    /**
     * Fills a list of triangles with specific informations.
     * @param coordinates       A list of coordinates.
     * @param triangleList      Triangles indexation.
     * @param numTriangles      The number of triangles.
     * @param rSpace            The r-d space (2d, 3d).
     * @return                  A list of triangles.
     */
    std::vector<Triangle> BuildTriangles( float * coordinates, int * triangleList, 
                                        int numTriangles, int rSpace );
    
    /**
     * Builds a kd-tree based on the triangles information.
     * @param triangles         A list containing the triangles information.
     * @param coordinates       A list of coordinates.
     * @param triangleList      Triangles indexation.
     * @param begin             Begin of the list of triangles to be considered.
     * @param end               End of the list of triangles to be considered.
     * @param depth             Depth of the kd-tree.       
     * @param rSpace            The r-d space (2d, 3d).
     * @return                  A kd-tree based on the triangles information.
     */
    Node * BuildKdTree ( std::vector<Triangle> &triangles, float * coordinates, int * triangleList, 
                         int begin, int end, int depth, int rSpace );
    
    /**
     * Controller to build the triangles list and the kd-tree.
     * @param coordinates       A list of coordinates.
     * @param triangleList      Triangles indexation.
     * @param numTriangles      The number of triangles.
     * @param rSpace            The r-d space (2d, 3d).
     * @return                  A kd-tree based on the triangles information. 
     */
    KdTree( float * coordinates, int * triangleList, int numTriangles, int rSpace);
    
    void UpdateBoundingBox( Node * node );
    void PrintKdTree ( Node * root );
    virtual ~KdTree ( );
private:

};

#endif	/* KDTREE_H */

