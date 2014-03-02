#include "cppsupport.h"

// This file implements functions required for certain C++ features to work.

extern "C" {

// Required when using pure virtual functions.
void __cxa_pure_virtual() {
    // This is called in case a pure virtual function call cannot be
    // made. This should never happen.
    // TODO: Should we throw an error somehow when that happens?
}

// Below, the runtime API for the Itanium C++ ABI DSO Object
// Desctruction API is implemented. Refer to
// http://sourcery.mentor.com/public/cxx-abi/abi.html#dso-dtor.
// Basically, this is required for global object destructors to be
// called after kmain returns.

atexit_func_entry __atexit_funcs[ATEXIT_MAX_FUNCS];
unsigned __atexit_func_count = 0;

void* __dso_handle = 0; // TODO: According to OSDev, optimally, you
                        // should remove the '= 0' part and define this
                        // in your asm script.

int __cxa_atexit(void (*destructor)(void*), void* obj_ptr, void* dso) {
    if(__atexit_func_count >= ATEXIT_MAX_FUNCS)
        return -1; // Non-success
    __atexit_funcs[__atexit_func_count].destructor = destructor;
    __atexit_funcs[__atexit_func_count].obj_ptr = obj_ptr;
    __atexit_funcs[__atexit_func_count].dso_handle = dso;
    __atexit_func_count++;
    return 0; // Success
}

void __cxa_finalize(void* dso) {
    // According to the Itanium C++ ABI, if NULL is passed, all
    // destructors should be called. This will definitely happen after
    // kmain returns. Note that the destructors must be called in the
    // opposite of the order in which they were added to the list.
    if(dso == nullptr) {
        unsigned i = __atexit_func_count;

        while(i--) {
            if(__atexit_funcs[i].destructor)
                (*__atexit_funcs[i].destructor)(__atexit_funcs[i].obj_ptr);
        }

        __atexit_func_count = 0;
        return;
    }

    // Walk the list calling and removing the destructors if dso matches
    // their dso_handle.
    unsigned i = __atexit_func_count;
    while(i--) {
        if(__atexit_funcs[i].dso_handle == dso) {
            (*__atexit_funcs[i].destructor)(__atexit_funcs[i].obj_ptr);

            // Copy all the higher destructors down one spot to fill the
            // hole in the list due to removing this one.
            for(unsigned j = __atexit_func_count - 1; j > i; j--)
                __atexit_funcs[j-1] = __atexit_funcs[j];

            // There is now one less destructor in the world.
            __atexit_func_count--;
        }
    }
}

}

