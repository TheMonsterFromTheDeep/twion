# twion
Twion is intended to be a 2D animation software based heavily on Blender.

## building
Twion requires GLFW 3. It doesn't require a very high version of OpenGL, basically because right now it is being developed on a Windows computer that doesn't have very new OpenGL.
Its likely that if you have OpenGL headers, they're a high enough version to build Twion.

For general development purposes, simply run `make`. Right now the makefile only supports building on Windows computers; on Linux, a number of different dependencies have to be linked.
`make clean` clears all built files; this is useful if the project needs to be completely rebuilt.

### standalone executables
`make windows` should create an executable that will run on (most) Windows computers. However, image resources need to be placed in the same directory as the executable-right now
the only image resources are `hsv.png` and `value.png`.

## usage
Twion generally matches Blender's control scheme.

### new objects
Twion starts with no objects in the scene. Pressing Shift-A adds a new object, which can then be edited by entering edit mode with Tab.

### selection
Right-click is used for selection. Shift-right-click is used to select more than one point.

The A key will select / deselect all points.

### transformations
Pressing G, R, or S will translate, rotate, or scale all selected points. Left-click confirms the transformation; right-click cancels it.

While translating or scaling, pressing X or Y will constrain the transformation along that axis. Pressing the key again cancels the constraint.

The pivot point is usually the center of all selected points. However, when a single handle is selected and is being rotated or scaled, the pivot point is set to it's parent control point.

#### translations
There are multiple other features supported by translations. First, if G is pressed again while translating, "sliding" mode is activated, and points will move along the curve. (Endpoints will not move) (Currenly unimplemented).

Pressing Shift-G instead of just G enters "corrective translation," which will cause handles outside the points currently being translated to line themselves up with the points currently being translated. It's somewhat difficult to explain; the best way to see how it works is to just test it with multiple combinations of points / handles selected.

### new points
(Currently unimplemented) Pressing Control-R creates new points between selected points, and immediately enters into sliding transformation mode. Right-clicking to exit sliding transformation will cause the new points to be at the center of the old points, as that is where they are created.

Pressing E creates new points as an "extrusion," and then immediately enters into corrective translation mode. Right clicking will not delete the points; rather they will simply lie on the same point as their sources.

### deleting points
Pressing Delete deletes selected points.

### other edits
Pressing Control-A will thicken the stroke along selected points.

Pressing C will update the stroke color of selected points to the selected color in the color wheel.

Pressing F will join selected points with new curves.

(Currently unimplemented) Press Space to cause the selected control points to become "smooth" -- i.e. every selected control point's handles will be modified to be parallel.

## todo
- [ ] Shape editing
   - [ ] Transformations in editing
      - [x] Translation
      - [x] Rotation
      - [x] Scale
      - [ ] Various useful pivot points
   - [x] Dynamic number of points
      - [x] Extrusions
      - [x] Creating a new point between points
      - [x] Deleting points
   - [ ] Better selection modes
      - [ ] Lasso select
      - [ ] Box select
- [ ] Improve rendering of edit-able components
   - [x] Draw lines between handles
   - [ ] Rendering modes for shapes (i.e. with color, without color)
- [ ] Shape 'detailing'
   - [ ] Extra control points on curves
   - [ ] Creating detail lines
      - [ ] Creating points in space
      - [ ] Creating points on curves
   - [ ] Creating faces
      - [ ] Gradient fill
- [x] Line width editing
- [ ] General entity editing
   - [ ] Creation of new shapes
   - [ ] Parenting of shapes

