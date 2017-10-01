# twion
Twion is intended to be a 2D animation software based heavily on Blender.

## usage
Twion generally matches Blender's control scheme.

### selection
Right-click is used for selection. Shift-right-click is used to select more than one point.

The A key will select / deselect all points.

### transformations
Pressing G, R, or S will translate, rotate, or scale all selected points. Left-click confirms the transformation; right-click cancels it.

The pivot point is usually the center of all selected points. However, when a single handle is selected and is being rotated or scaled, the pivot point is set to it's parent control point.

### other edits
Pressing Control-A will thicken the stroke along selected points.

## todo
- [ ] Fill colors for shapes
- [ ] Shape editing
   - [ ] Transformations in editing
      - [x] Translation
      - [x] Rotation
      - [x] Scale
      - [ ] Various useful pivot points
   - [ ] Dynamic number of points
      - [ ] Extrusions
      - [ ] Creating a new point between points
      - [ ] Deleting points
      - [ ] Changing whether a shape is looped or not
   - [ ] Better selection modes
      - [ ] Lasso select
      - [ ] Box select
- [ ] Improve rendering of edit-able components
   - [ ] Draw lines between handles
   - [ ] Rendering modes for shapes (i.e. with color, without color)
- [ ] Shape color editing
   - [x] Stroke color editing
   - [ ] Fill color editing
- [x] Line width editing
- [ ] General entity editing
   - [ ] Creation of new shapes
   - [ ] Parenting of shapes

