# KdTree4TriangularMeshes

### Algorithm

```
Node *buildKdTree(ini, end, triangles)
{
  sort triangles by k-n centroid coordinate
  median = (ini + end)/2
  node.triangle = triangles[med]
  node.left  = buildKdTree(ini, median, points) 
  node.right = buildKdTree(median, fim, points)
  node.aabb  = union(node.triangle.aabb, node.left.aabb, node.right.aabb)
  return node
}
```

---

## 2D Folder
A KdTree implementation for triangular 2D mesh.
- Keys:
  * d: Minus one depth
  * D: Plus one depth
  * v: View the whole kd-tree

- The Triangles:

![The Triangles](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/2D/Screenshots/depth0.png)

- kd-Tree per depth:

![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/2D/Screenshots/depth1.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/2D/Screenshots/depth2.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/2D/Screenshots/depth3.png)

- The whole kd-Tree:

![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/2D/Screenshots/depth4.png)

---

## 3D Folder
A KdTree implementation for triangular 3D mesh.

- Keys:
  * A: Open mesh
  * b: Minus one depth
  * B: Plus one depth
  * g: Turn grid on/off
  * p: Fill surface

---

- kd-Tree per depth:

![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/3D/Screenshots/depth0.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/3D/Screenshots/depth1.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/3D/Screenshots/depth2.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/3D/Screenshots/depth3.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/3D/Screenshots/depth4.png)
![alt text](https://github.com/paulaceccon/KdTree4TriangularMeshes/blob/master/3D/Screenshots/depth5.png)
