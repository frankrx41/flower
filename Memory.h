#pragma once

// Memory
error   Engine_Memory_Initialize    ();

tptr    Engine_Memory_Alloc         (const tchar* debug_name, tsize size);
error   Engine_Memory_Free          (const tchar* debug_name, tptr ptr);
tptr    Engine_Memory_CloneInfo     (const tchar* debug_name, const tptr ptr);
tptr    Engine_Memory_Clone         (const tchar* debug_name, const tptr ptr);
tptr    Engine_Memory_SafeClone     (const tchar* debug_name, const tptr ptr);

tptr    Engine_Memory_Copy          (tptr dst_ptr, const tptr src_ptr, tsize size);
tptr    Engine_Memory_Set           (tptr address, int32 val, tsize size);
tptr    Engine_Memory_FullFill      (tptr address, int32 val);
tptr    Engine_Memory_Zero          (tptr address);

tsize   Engine_Memory_GetSize       (const tptr ptr);


#define MemNew(debug_name, type)    (type*)Engine_Memory_Alloc(debug_name, sizeof(type))
#define MemDel(debug_name, ptr)     do{Engine_Memory_Free(debug_name, ptr); ptr = NULL;}while(0)