#include <stdarg.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

void my_puts(const char *s) {
    write(1, s, strlen(s));  // 文字列全体を出力
}

void my_putint(int n) {
    char buf[12];  // 整数の最大桁数 + 終端ヌル文字用のバッファ（最大値は -2147483648\0）
    int i = 0;
    int is_negative = 0;
    
    // 負の数に対応
    if (n < 0) {
        is_negative = 1;  // 負の数フラグを立てる
        n = -n;  // 正の数に変換
    }
    
    // 整数を逆順に格納
    if (n == 0) {
        buf[i++] = '0';
    } else {
        while (n > 0) {
            buf[i++] = (n % 10) + '0';  // 末尾の数字を追加
            n /= 10;
        }
    }

    // 負の数の場合、先頭に負号を追加
    if (is_negative) {
        buf[i++] = '-';
    }

    // 逆順に格納された数字を逆に並べる
    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }
    
    // 終端ヌル文字を追加
    buf[i] = '\0';

    // 出力
    write(1, buf, i);  // 整数を文字列として出力
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
                case 'd': my_putint(va_arg(args, int)); break;
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

    my_printf("正の数: %d\n", 1234);
    my_printf("負の数: %d\n", -56789);

    my_printf("英語: %s\n", "Hello, world!");
    my_printf("日本語: %s\n", "こんにちは 世界");

    my_printf("char: %c\n", 'A');
    my_printf("%%\n");
    my_printf("%a\n");

    return 0;
}
