#include <sys/mman.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ucontext_t uctx_main, uctx_func1, uctx_func2;

#define STACK_SIZE (1 << 14)

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void
func1(void)
{
    char buffer[256] = "Salut ma belle";
    printf("func1: started %p, %s\n", buffer, buffer);
    printf("func1: swapcontext(&uctx_func1, &uctx_func2)\n");
    if (swapcontext(&uctx_func1, &uctx_func2) == -1)
        handle_error("swapcontext");
    printf("func1: returning, %p, %s\n", buffer, buffer);
}

static void
func2(void)
{
    char buffer[256] = "Salut ma biche";
    printf("func2: started, %p, %s\n", buffer, buffer);
    printf("func2: swapcontext(&uctx_func2, &uctx_func1)\n");
    if (swapcontext(&uctx_func2, &uctx_func1) == -1)
        handle_error("swapcontext");
    printf("func2: returning, %p, %s\n", buffer, buffer);
}

int
main(int argc, char *argv[])
{
    memset(&uctx_main, 0, sizeof (uctx_main));
    memset(&uctx_func1, 0, sizeof (uctx_main));
    memset(&uctx_func2, 0, sizeof (uctx_main));

    char *func1_stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    char *func2_stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);

    int var = 0;
    printf("main unsafe stack: %p\n", &var);

    if (func1_stack == MAP_FAILED || func2_stack == MAP_FAILED) {
        fprintf(stderr, "map failed: %m\n");
	return 1;
    }

    if (getcontext(&uctx_func1) == -1)
        handle_error("getcontext");
    uctx_func1.uc_stack.ss_sp = func1_stack;
    uctx_func1.uc_stack.ss_size = STACK_SIZE;
    uctx_func1.uc_link = &uctx_main;
    makecontext(&uctx_func1, func1, 0);

    if (getcontext(&uctx_func2) == -1)
        handle_error("getcontext");
    uctx_func2.uc_stack.ss_sp = func2_stack;
    uctx_func2.uc_stack.ss_size = STACK_SIZE;
    /* Successor context is f1(), unless argc > 1 */
    uctx_func2.uc_link = (argc > 1) ? NULL : &uctx_func1;
    makecontext(&uctx_func2, func2, 0);

    printf("main: swapcontext(&uctx_main, &uctx_func2)\n");
    if (swapcontext(&uctx_main, &uctx_func2) == -1)
        handle_error("swapcontext");

    printf("unmapping");
    munmap(func1_stack, STACK_SIZE);
    munmap(func2_stack, STACK_SIZE);
    printf("main: exiting\n");
    exit(EXIT_SUCCESS);
}
