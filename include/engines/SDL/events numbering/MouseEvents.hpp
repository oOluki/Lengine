#pragma once

namespace Le::SDL{

enum SystemCursor{
    SYSCURARROW,     
    SYSTEM_CURSOR_IBEAM,     
    SYSCURWAIT,      
    SYSTEM_CURSOR_CROSSHAIR, 
    SYSCURWAITARROW, 
    SYSTEM_CURSOR_SIZENWSE,  
    SYSCURSIZENESW,  
    SYSTEM_CURSOR_SIZEWE,    
    SYSCURSIZENS,    
    SYSTEM_CURSOR_SIZEALL,   
    SYSCURNO,        
    SYSTEM_CURSOR_HAND,      
    SYSCURSORS
};

enum MouseDirection{
    MOUSEWHEEL_NORMAL,    
    MOUSEWHEEL_FLIPPED
};

}