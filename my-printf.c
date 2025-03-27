#include <stdarg.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

void my_puts(const char *s) {
    write(1, s, strlen(s));  // 文字列全体を出力
}

// UTF-8 文字を 1 文字ずつ出力
void my_put_utf8_char(const char **p) {
    const unsigned char *s = (const unsigned char *)(*p);
    int bytes = 1;

    // 先頭バイトからバイト数を判定
    if ((*s & 0x80) == 0)        bytes = 1;  // ASCII (1バイト)
    else if ((*s & 0xE0) == 0xC0) bytes = 2;  // 2バイト文字
    else if ((*s & 0xF0) == 0xE0) bytes = 3;  // 3バイト文字
    else if ((*s & 0xF8) == 0xF0) bytes = 4;  // 4バイト文字

    write(1, s, bytes);  // 必要なバイト数だけ出力
    *p += bytes;  // ポインタを進める
}

void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': my_puts(va_arg(args, char*)); break;
                case 'c': {
                    char c = (char)va_arg(args, int);
                    write(1, &c, 1);
                    break;
                }
                case '%': write(1, "%", 1); break;
                default: write(1, "?", 1); break;
            }
        } else {
            my_put_utf8_char(&format);  // UTF-8 文字対応
            continue;  // `my_put_utf8_char` で `format` を更新したので `format++` は不要
        }
        format++;
    }

    va_end(args);
}

int main() {
    setlocale(LC_ALL, "");  // UTF-8 のロケール設定

    my_printf("英語: %s\n", "Hello, world!");
    my_printf("日本語: %s\n", "こんにちは 世界");
    
    my_printf("char: %c\n", 'A');
    my_printf("%%\n");
    my_printf("%a\n");

    return 0;
}
