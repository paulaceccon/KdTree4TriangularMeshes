# KdTree2D
A KdTree implementation for triangular 2D mesh.

- Keys:
  * d: Minus one depth
  * D: Plus one depth
  * v: View the whole kd-tree

---

- Algorithm

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

- The Triangles:

![The Triangles](https://github.com/paulaceccon/kdTree2D/blob/master/Screenshots/depth0.png)

- kd-Tree per depth:

![alt text](https://github.com/paulaceccon/kdTree2D/blob/master/Screenshots/depth1.png)
![alt text](https://github.com/paulaceccon/kdTree2D/blob/master/Screenshots/depth2.png)
![alt text](https://github.com/paulaceccon/kdTree2D/blob/master/Screenshots/depth3.png)

- The whole kd-Tree:

![alt text](https://github.com/paulaceccon/kdTree2D/blob/master/Screenshots/depth4.png)
