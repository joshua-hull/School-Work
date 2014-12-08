__kernel void myvadd(__global int *c, __global int *a, __global int *b)
{
unsigned int n;

n = get_global_id(0);
c[n] = a[n] + b[n];
}
