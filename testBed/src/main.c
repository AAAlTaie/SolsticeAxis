#include "test.h"
#include <stdio.h>  // Required for getchar()

int main(void) {
    print_int(42);

    // Prevent command window from closing immediately
    printf("\nPress Enter to exit...");
    getchar();

    return 0;
}
