#include "kernel/types.h"
#include "user/user.h"

/* The algorithm of this program primes is best explained on this page https://swtch.com/~rsc/thread/
 * respectively, program is exactly according to webpage made. Best illustration is picture on that page.*/

void primes(int *p_left){
    int p;
    int n;
    int p_out[2];

    pipe(p_out);
    close(p_left[1]);

    if (read(p_left[0], &p, sizeof(int))) { // p = getting number from left neighbor
        printf("prime %d\n", p); // print p
        if (fork() == 0) {
            primes(p_out); // Practically opening of the other neighbor on the right (see picture on webpage)
            exit(0);
        } else {
            while (read(p_left[0], &n, sizeof(int))) { // n = getting number from left neighbor
                if (n % p != 0) { // if p does not divide n - send buf to right neighbor, it will send only correct numbers - primes
                    write(p_out[1], &n, sizeof(int));
                }
            }
            close(p_out[0]);
            close(p_out[1]); // clossing "left side" pipes (see picture on webpage)
            wait(0);
            exit(0);
        }
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    if (argc > 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int pfirst[2];
    pipe(pfirst);

    if (fork() == 0) {
        primes(pfirst); // Practically opening of the other neighbor on the right
        exit(0);
    } else {
        for (int i = 2; i <= 35; ++i) { // First process sending 35 numbers - from 2 to 35.
            write(pfirst[1], &i, sizeof(int));
        }

        close(pfirst[0]);
        close(pfirst[1]); // clossing "left side" pipes (see picture on webpage)
        wait(0);
        exit(0);
    }
}