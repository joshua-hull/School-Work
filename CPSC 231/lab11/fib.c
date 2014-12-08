#include <stdio.h>

int fib(register int k)
{
	if (k == 0)
	{
		return 0;
	}
	else if (k == 1)
	{
		return 1;
	}
	else
	{
		return fib(k - 1) + fib(k - 2);
	}
}

void test(register int k)
{
	printf("Element %d (zero-based) in the Fibonacci sequence is %d\n", k, fib(k));
}

int main()
{
	test(0);

	test(1);

	test(6);

	test(10);

	return 0;
}
