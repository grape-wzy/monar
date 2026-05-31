#include <stdio.h>

#include "test_host.h"

int main(void)
{
    mn_test_osal();
    mn_test_lifecycle();
    mn_test_device();

    puts("host tests passed");
    return 0;
}
