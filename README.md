# Lengine
WARNING: Lengine is an ongoing project still in develepment originally designed for recreational purposes, as such it may contain dangerous and unsafe code that may, for reasons that were never originally intended by the author, harm your machine or some other software you use. Use with discretion.


Lengine is a project containing various libraries to facilitate very basic and recreational software develepment.
Most of the available libraries seek to facilitate the use of other consolidated libraries, such as: SDL2, SDL2_image, SDL2_mixer, SDL2_ttf, GLFW and GLEW.
The standard C and C++ libraries are also used.
Lengine also contains a very simple aplication for plugin loading and hot reloading with support to all the libraries mentioned above.

Plugins:

plugin.c aka simple:
  just for showcase, an example plugin that just draws a rectangle on screen, if you click somewhere on screen it will move there.

map_designer:
  a plugin that allows for tile map design.
  
  usage:
  press h to go to home or press m to go to the map editor;
  
  while in home you can click and change various map and tile settings (press enter after each change to validate it), 
  begin by dragging and dropping a .png file that has your tilesheet in it then set the appropriate tilew and tile h 
  (you may need to change the mapsizex and mapsizey aswell as the plugin won't allow for combinations that would cause it to try to access tiles outside allocated memory),
  you can now use the mouse wheel to go over the loaded tiles or click on the tilesheet image and select which one you want (if you do the later press h to go back to home).
  To save/load enter a valid path to the output/input png file and click on the save/load button (the file doesn't need to exist in advance for saving).
  
  While in the map editor you can hover over the loaded tiles with the mouse wheel, place tiles with the left mouse button and remove them with the right mouse button, you can also drap and drop a png map file and it will get loaded.
  Use the sliders or wsad to move around the map.
  If ctrl is pressed the mouse wheel will be used to zoom in and out (be careffull as if you zoom too much out the tiles may not be rendered) and you can click and drag to crate a selection, use c (ctrl unpressed) to copy a selction, v (ctrl unpressed) to paste the copied selection on to the new selected area and delete to delete tiles insided the selection. The selection can be moved using the arrow keys. Press ctrl shift to deactivate the selections.
