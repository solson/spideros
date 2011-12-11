#ifndef CPPSUPPORT_H
#define CPPSUPPORT_H

const int ATEXIT_MAX_FUNCS = 128; // TODO: Why 128? I don't know.

extern "C" {

struct atexit_func_entry {
    void (*destructor)(void *);
    void *obj_ptr;
    void *dso_handle;
};

int __cxa_atexit(void (*f)(void *), void *obj_ptr, void *dso);
void __cxa_finalize(void *dso);

}

#endif // CPPSUPPORT_H
