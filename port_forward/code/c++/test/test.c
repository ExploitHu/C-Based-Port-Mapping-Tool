#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void)
{
    void *handle;
    double (*cosine)(double);
    char *error;

    //handle = dlopen("/school/nodejs/lib/librsl_ui_func.so", RTLD_LAZY);
    handle = dlopen("/port_forward/code/node/lib/libui.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    //dlerror();    /* Clear any existing error */

    //cosine = (double (*)(double)) dlsym(handle, "cos");

    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    //printf("%f\n", (*cosine)(2.0));
    dlclose(handle);
    exit(EXIT_SUCCESS);
}