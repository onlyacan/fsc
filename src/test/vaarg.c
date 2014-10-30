#include <stdarg.h>
#include <stdio.h>

void func(char *fmt, ...)
{
    va_list ap;
    char *x;
    
    va_start(ap, fmt);
    while (1) {
        x = va_arg(ap, char *);
        printf("%p\n",x);
    }
    va_end(ap);
}

int main(void)
{
    func("tete", "tetefenife");
    return 0;
}
