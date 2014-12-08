#include <stdio.h>

int main()
{
	int a[10] = {34, 5, 1, 86, 23, 2, 9, 15, 21, 4};
	register int sum = 0, i;

	for (i = 0; i < 10; i++)
	{
		sum += a[i];
	}

	printf("sum = %d\n", sum);
}
