#include <monar/monar.h>

#include "board.h"

int main(void)
{
    mn_status_t ret;

    ret = mn_osal_init();
    (void)ret;

    mn_board_init();

    while (1) {
    }
}
