#include <stdarg.h>
#include <unistd.h>

void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    write(1, &c, 1);
                    break;
                }
                case '%': write(1, "%", 1); break;
                default: write(1, "?", 1); break;
            }
        } else {
            // TODO: UTF-8 文字を考慮したフォーマット指定子以外を出力できるようにする
            write(1, format, 1);
        }
        format++;
    }

    va_end(args);
}

int main() {
    my_printf("char: %c\n", 'A');
    my_printf("%%\n");
    my_printf("%a\n");

    return 0;
}
