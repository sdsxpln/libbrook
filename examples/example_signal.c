#include <stdlib.h>
#include <brook.h>
#include <unistd.h>
#include <signal.h>

bool terminated = false;

static void signal_cb(__BK_UNUSED int sig) {
    terminated = true;
    printf("Process terminated\n");
}

int main(void) {
    unsigned char count = 0;
    bk_sigterm(signal_cb);
    while (!terminated) {
        if (count >= 3)
            raise(SIGTERM);
        else {
            printf("Processing\n");
            sleep(1);
        }
        count++;
    }
    return EXIT_SUCCESS;
}
