#include <stdio.h>

int a = -1, b = 1;

int inc_a()
{
    int b = a;
    b = b + 1;
    a = b;
    return a;
}

int main()
{
    int k = 5;
    while (k >= 0) 
	{
        if (inc_a() && inc_a() && inc_a()) 
		{
            printf("%d %d\n", a, b);
        }
        if (inc_a() < 14 || inc_a() && inc_a() - inc_a() + 1) 
		{
            printf("%d\n", a);
            b = b * 2;
        } 
		else 
		{
            inc_a();
        }
        k = k - 1;
    }
    printf("%d %d\n", a, b);
    return a;
}
