# Lengine
Lengine is a project containing a simple hot reload executable and some example plugins it runs. The idea is to be able to pass the absolute path of this plugin objects (.dll/.so) and have the Lengine executable run them, while also allowing for cool features as hot reloads, which can make software development more convinient.

To build this, simply compile the Lengine.c file linking against dlfcn in linux and macos or ws2_32 in windows, or use cmake

To use it include the environment.h header and define the void plugin_init(Env*) and bool plugin_update(Plugin*) methods, additionally you may define the void plugin_retrieve_state(void*) method for hot reloading. Here is the usage:

  Lengine Help:

    Usage: ./Lengine <path_to_plugin> <optional: path_to_subsystem>\n");
    To Display This Help Message: ./Lengine or ./Lengine --help\n");
    Lengine Takes A Path To A Valid Plugin, Loads It And Runs The Plugin. If Passed, The Object File (aka dll) At path_to_subsystem> Will Get Loaded To The Shared Channel Where Plugins Can Retrieve Symbols And Methods From It (see the map_designer example).

    The Plugin Must Contain The Following Methods:
      void plugin_init(Env*):
        Used to initiate the plugin once first loaded, and on all subsequential loads if hot realoading is not implemented (see void plugin_retireve_state(void*)). It takes a pointer to the Lengine environment in case the plugin wants to internally manipulate it or use its channel.
      bool plugin_update(Plugin*):\n"
        The method to be called every frame that the plugin is active, it takes a pointer to its own Plugin object (see Plugin). This method should also return 0 if the plugin is to be closed or 1 other wise. If you wish to hot reload, write the state of your plugin to the passed Plugin object's state to save it and use the void (*overwrite_plugin)(Plugin*, const char*) callback method of the Env object to overwrite the current plugin with the object file at the passed path. Be wary that Lengine does not free any memory dynamically allocated by the plugin, not even if it's on its Plugin object's state, you should handle that on the plugin_retrieve_state method and when terminally closing the plugin.
    The Plugin May Contain The Follwing Method (For Hot Reloading):
      void plugin_retrieve_state(void*):
        Used for hot reloading, if a plugin doesn't want/need hot reloading simply don't define this method in it (in which case the Lengine aplication will simply start the plugin using plugin_init). This function (if defined by the plugin) will be called for reopening the plugin, so it can retrieve its previous state from before closing when the environment's int overwrite_plugin(Plugin*, const char*) method is called. The state that is passed to this function is the one in the Plugin object's state.

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
