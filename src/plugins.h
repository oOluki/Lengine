#ifndef LE_PLUGUINLOAD
#define LE_PLUGUINLOAD
#include "environment.h"

#define LE_PLUGINCALL

#define LE_DECLSPEC


#ifdef __cplusplus__
extern 'C'{
#endif

/* Used to initiate the plugin once first loaded, and on all subsequential loads if hot realoading is not implemented
* see plugin_retireve_state() for hot reloading explanation.
* \param env the pointer to the environment that requested initiation
* \since 0.1.0
*/
extern LE_DECLSPEC void LE_PLUGINCALL plugin_init(Env* env);


// Used for hot reloading, if a plugin doesn't want/need hot reloading simply don't define this method in it (in which case
// the main aplication will simply start the plugin using plugin_init()).
// This function (if defined by the plugin) will be called for reopening the plugin, so it can retrieve
// its previos state from before closing.
// \param plugin_state the state of the plugin before reloading (see plugin_close())
extern LE_DECLSPEC void LE_PLUGINCALL plugin_retrive_state(void* plugin_state);

/*
* The method to be called every frame that the plugin is active.
* This method should also return 0 if the plugin is to be closed or 1 other wise.
* \return 0 if the plugin is to be closed or 1 otherwise.
*/
extern LE_DECLSPEC bool LE_PLUGINCALL plugin_update(Plugin* self);


#ifdef __cplusplus__
}
#endif

#undef LE_PLUGINCALL

#undef LE_DECLSPEC 




#endif // END OF CODE ====================================================================================