#include <monar/monar.h>

#include "board.h"

int main(void)
{
    mn_status_t ret;

    mn_board_init();

    ret = mn_osal_init();
    (void)ret;

    while (1) {
    }
}
