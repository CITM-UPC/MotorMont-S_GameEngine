# MOTOR MONT-S GAME ENGINE README

Version 1.0 of the Motor Montés. Developed with love and care. this version displays only a model with its 
respective checker box. Minimal UI has been implemented. 

### The developers

- [Sergi Gantzer](https://github.com/sgantzer12).
- [Ivo Montés](https://github.com/Ivomm9).
- [Biel Liñán](https://github.com/Drauguer).

### Engine controls

### Camera control
- Zoom In : Mousewheel up
- Zoom Out: Mousewheel down
- Move Forward: Right Click + W key
- Move Backward: Right Click + S key
- Move Left: Right Click + A key
- Move Right: Right Click + D key
- Move Up: Right Click + Q key
- Move Down: Right Click + E key
- Focus on Selected Object: F key (not implemented yet)
- Orbit Around Selected Object: Alt Key + Left Click (not implemented yet)
- Move twice as fast: Shift Key
- Frustum added and working

### UI control
- Menu bar in the top section of the window with "options"
- Activate and deactivate sections of the UI
- Link to our GitHub
- Quit app
- Display only Transform and Mesh&Texture information
- Load and save scene option
- Hierarchy with parenting
- Delete: select an object in the hierarchy and press supr
- Parent an object by dragging another to it in the hierarchy
- View option to change between console and assets folder
- Mesh option to create different shapes

### NOTES
- Delivery is in release, not debug. When we changed to debug, 24 external errors appeared and it wasn't fixed switching C++ versions
- Loading Meshes and Textures theoretically works, the message is displayed in the console and the number of meshes goes up, however it's not appearing in the scene

