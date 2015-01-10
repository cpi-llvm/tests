#include <stdio.h>

static char sp2[8192];

int main(void)
{
	printf("get_usp: %p\n", __builtin_safestack_get_usp());
	__builtin_safestack_set_usp(sp2 + sizeof (sp2));
	printf("get_usp: %p\n", __builtin_safestack_get_usp());
	return 0;
}
