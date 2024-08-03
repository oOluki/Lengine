#pragma once

namespace Le::SDL{

enum EventTypes {
    FIRSTEVENT     = 0,     
    
    QUIT           = 0x100, 

    TERMINATING,        
    APP_LOWMEMORY,          
    WILLENTERBACKGROUND, 
    DIDENTERBACKGROUND, 
    WILLENTERFOREGROUND, 
    DIDENTERFOREGROUND, 

    LOCALECHANGED,  
    
    DISPLAYEVENT   = 0x150,  
    
    WINDOWEVENT    = 0x200, 
    SYSWMEVENT,             
    
    KEYDOWN        = 0x300, 
    KEYUP,                  
    TEXTEDITING,            
    TEXTINPUT,              
    KEYMAPCHANGED,          

    MOUSEMOTION    = 0x400, 
    MOUSEBUTTONDOWN,        
    MOUSEBUTTONUP,          
    MOUSEWHEEL,         

    JOYAXISMOTION  = 0x600, 
    JOYBALLMOTION,          
    JOYHATMOTION,           
    JOYBUTTONDOWN,          
    JOYBUTTONUP,            
    JOYDEVICEADDED,         
    JOYDEVICEREMOVED,       

    CONTROLLERAXISMOTION  = 0x650, 
    CONTROLLERBUTTONDOWN,          
    CONTROLLERBUTTONUP,            
    CONTROLLERDEVICEADDED,         
    CONTROLLERDEVICEREMOVED,       
    CONTROLLERDEVICEREMAPPED,      
    CONTROLLERTOUCHPADDOWN,        
    CONTROLLERTOUCHPADMOTION,      
    CONTROLLERTOUCHPADUP,          
    CONTROLLERSENSORUPDATE,        

    FINGERDOWN      = 0x700,
    FINGERUP,
    FINGERMOTION,

    DOLLARGESTURE   = 0x800,
    DOLLARRECORD,
    MULTIGESTURE,

    CLIPBOARDUPDATE = 0x900, 

    DROPFILE        = 0x1000, 
    DROPTEXT,                 
    DROPBEGIN,                
    DROPCOMPLETE,             

    AUDIODEVICEADDED = 0x1100, 
    AUDIODEVICEREMOVED,        

    SENSORUPDATE = 0x1200,     
    RENDER_TARGETS_RESET = 0x2000, 

    RENDER_DEVICE_RESET, 

    POLLSENTINEL = 0x7F00, 

    USEREVENT    = 0x8000,

    LASTEVENT    = 0xFFFF
};

}

