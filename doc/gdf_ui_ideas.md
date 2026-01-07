# gdf UI ideas

- Retained mode seems simpler overall
- All things in the UI hierarchy are "frames", rectangles
- A frame specifies how it lays out its children
- Anything more complex than a frame is a separate thing that owns a frame
  - So a button would own a frame which it draws text into
  - Possibly with a callback?
- All frames will have mouse state callbacks and pressed callbacks etc
- Keeping the main tree of all the frames as uniform and tightly packable is important for later optimization
  - We want to make sure that (if not initially) we can make it so all frames are in one CPU cachable chunk of memory
