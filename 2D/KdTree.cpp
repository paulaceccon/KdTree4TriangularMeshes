/* 
 * File:   KdTree.cpp
 * Author: paulaceccon
 * 
 * Created on 10 de Novembro de 2013, 11:11
 */

#include "KdTree.h"
#include <algorithm>
#include <cstdio>


bool comp_x( const Triangle &t1, const Triangle &t2 )
{
    return t1.centroid_x < t2.centroid_x;
}



bool comp_y( const Triangle &t1, const Triangle &t2 )
{
    return t1.centroid_y < t2.centroid_y;
}



bool comp_z( const Triangle &t1, const Triangle &t2 )
{
    return t1.centroid_z < t2.centroid_z;
}



float find_max( float v1, float v2, float v3 )
{
    return std::max( std::max ( v1, v2 ), v3 );
}



float find_min( float v1, float v2, float v3 )
{
    return std::min( std::min ( v1, v2 ), v3 );
}



KdTree::KdTree( float * coordinates, int * triangleList, int numTriangles, int rSpace)
{   
    triangles = BuildTriangles ( coordinates, triangleList, numTriangles, rSpace);
    root = BuildKdTree( triangles, coordinates, triangleList, 0, numTriangles, 0, rSpace );
}



std::vector<Triangle> KdTree::BuildTriangles( float * coordinates, int * triangleList, 
                                              int numTriangles, int rSpace )
{
    for (int i = 0; i < numTriangles; i ++)
    {
        
        Triangle t;
        t.triangleCode = i;
        
        int index_1 = triangleList[i * 3];
        int index_2 = triangleList[i * 3 + 1];
        int index_3 = triangleList[i * 3 + 2];
        
        t.centroid_x = ( coordinates[index_1 * rSpace] + coordinates[index_2 * rSpace] + coordinates[index_3 * rSpace] )/3;
        t.centroid_y = ( coordinates[index_1 * rSpace + 1] + coordinates[index_2 * rSpace + 1] + coordinates[index_3 * rSpace + 1] )/3;
        
        if (rSpace == 3)
        {
            t.centroid_z = ( coordinates[index_1 * rSpace + 2] + coordinates[index_2 * rSpace + 2] + coordinates[index_3 * rSpace + 2] )/3;
        }
        triangles.push_back( t );
    }
    
    return triangles;
}



Node * KdTree::BuildKdTree( std::vector<Triangle> &triangles, float * coordinates, int * triangleList, 
                            int begin, int end, int depth, int rSpace )
{
    if (end - begin <= 0)
        return 0;

    int axis = depth % rSpace;
    
    // Sorting by axis to insert
    if (axis == 0)
    {
        std::sort( triangles.begin( ) + begin, triangles.begin( ) + end, comp_x );
    }
    else if (axis == 1)
    {
        std::sort( triangles.begin( ) + begin, triangles.begin( ) + end, comp_y );
    }
    else if (axis == 2)
    {
        std::sort( triangles.begin( ) + begin, triangles.begin( ) + end, comp_z );
    }
    else 
    {
        printf( "Axis Error" );
    }

    int half = (begin + end) / 2;
    int triangle_code = triangles[half].triangleCode; 
    
    // Getting the vertices from a triangle
    int index_1 = triangleList[triangle_code * 3];
    int index_2 = triangleList[triangle_code * 3 + 1];
    int index_3 = triangleList[triangle_code * 3 + 2];
       
    // Getting it vertices coordinates
    float x_1 = coordinates[index_1 * rSpace];
    float x_2 = coordinates[index_2 * rSpace];
    float x_3 = coordinates[index_3 * rSpace];
      
    float y_1 = coordinates[index_1 * rSpace + 1];
    float y_2 = coordinates[index_2 * rSpace + 1];
    float y_3 = coordinates[index_3 * rSpace + 1];
        
    float z_1 = 0.0;
    float z_2 = 0.0;
    float z_3 = 0.0;
    
    if (rSpace == 3)
    {
        z_1 = coordinates[index_1 * rSpace + 2];
        z_2 = coordinates[index_2 * rSpace + 2];
        z_3 = coordinates[index_3 * rSpace + 2];
    }
    
    // Finding it bounding box
    float x_min = find_min(x_1, x_2, x_3);
    float x_max = find_max(x_1, x_2, x_3);
    
    float y_min = find_min(y_1, y_2, y_3);
    float y_max = find_max(y_1, y_2, y_3);
    
    float z_min = find_min(z_1, z_2, z_3);
    float z_max = find_max(z_1, z_2, z_3);
    
    // Creating a node for this triangle
    Node * median = new Node( triangle_code, x_min, y_min, z_min, x_max, y_max, z_max );
    median->left = BuildKdTree( triangles, coordinates, triangleList, begin, half, depth + 1, rSpace );
    median->right = BuildKdTree( triangles, coordinates, triangleList, half + 1, end, depth + 1, rSpace );
    
    UpdateBoundingBox( median );
    return median;
}



void KdTree::UpdateBoundingBox( Node * node )
{
    // Merging its bounding box with its children's one
    if ( node->left != 0 && node->right != 0 )
    {
        float children_x_min = find_min(node->min_x, node->left->min_x, node->right->min_x);
        float children_y_min = find_min(node->min_y, node->left->min_y, node->right->min_y);
        float children_z_min = find_min(node->min_z, node->left->min_z, node->right->min_z);

        float children_x_max = find_max(node->max_x, node->left->max_x, node->right->max_x);
        float children_y_max = find_max(node->max_y, node->left->max_y, node->right->max_y);
        float children_z_max = find_max(node->max_z, node->left->max_z, node->right->max_z);
        
        // Updating it bounding box
        node->min_x = children_x_min;
        node->min_y = children_y_min;
        node->min_z = children_z_min;

        node->max_x = children_x_max;
        node->max_y = children_y_max;
        node->max_z = children_z_max;
    }
    else
    {
        if ( node->left != 0 )
        {
            float children_x_min = std::min(node->min_x, node->left->min_x);
            float children_y_min = std::min(node->min_y, node->left->min_y);
            float children_z_min = std::min(node->min_z, node->left->min_z);

            float children_x_max = std::max(node->max_x, node->left->max_x);
            float children_y_max = std::max(node->max_y, node->left->max_y);
            float children_z_max = std::max(node->max_z, node->left->max_z);

            // Updating it bounding box
            node->min_x = children_x_min;
            node->min_y = children_y_min;
            node->min_z = children_z_min;

            node->max_x = children_x_max;
            node->max_y = children_y_max;
            node->max_z = children_z_max;
        }
        else if ( node->right != 0)
        {
            float children_x_min = std::min(node->min_x, node->right->min_x);
            float children_y_min = std::min(node->min_y, node->right->min_y);
            float children_z_min = std::min(node->min_z, node->right->min_z);

            float children_x_max = std::max(node->max_x, node->right->max_x);
            float children_y_max = std::max(node->max_y, node->right->max_y);
            float children_z_max = std::max(node->max_z, node->right->max_z);

            // Updating it bounding box
            node->min_x = children_x_min;
            node->min_y = children_y_min;
            node->min_z = children_z_min;

            node->max_x = children_x_max;
            node->max_y = children_y_max;
            node->max_z = children_z_max;
        }
    }
}


//void KdTree::PrintKdTree( Node * root )
//{
//    if (root == 0)
//    {
//        printf( "Nil\n" );
//        return;
//    }
//    printf( "%lf %lf\n", root->p.x, root->p.y );
//    PrintKdTree( root->left );
//    PrintKdTree( root->right );
//}



KdTree::~KdTree( )
{
}

