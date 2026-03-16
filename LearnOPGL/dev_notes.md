# important notes about this program will go here.

3/9/2026

1. ~~currently our renderer that setups up meshs (MeshBuffers class) is handling both EBO and VBO, but it should be split into two classes, one for EBO and one for VBO, this isn't a big deal right now, but if we have many objects in the scene, it will be more efficient to have separate classes for EBO and VBO.~~ semi solved
2. ~~camera is a global right now in the window class, this is not a desired state, we should be creating the camera in the main function or somewhere else, will try to make this change soon.~~  With the addition of the input handler this annoying thing is now handled somewhat appropriately.

---

3/16/2026

1.  Issue: Renderer is currently hard coded to take in certain types of vertex layouts, this isnt good, later it needs to be able to take in any type of vertex layout!
	1. Solve Method: Y
	1. Date: X/X/2026
2.  Issue: X
	1. Solve Method: Y
	1. Date: X/X/2026

---

3/X/2026

1.  Issue: X
	1. Solve Method: Y
	1. Date: X/X/2026
2.  Issue: X
	1. Solve Method: Y
	1. Date: X/X/2026
