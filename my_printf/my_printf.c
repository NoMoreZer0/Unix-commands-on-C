#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdarg.h>

int not_argument(char a, char b){
    if (a != '%') return 1;
    if (b == 'd' || b == 'o' || b == 'u' || b == 'x' || b == 'c' || b == 's' || b == 'p') return 0;
    return 1;
}

int get_sz(int x, int base){
    int sz = 0;
    if (x == 0) sz = 1;
    if (base == 10 && x < 0){
        x = x * (-1);
        sz++;
    }
    while(x){
        ++sz;
        x /= base;
    }
    return sz;
}

char* convert(long long x, int base, int pointer_flag){
    int sz = get_sz(x, base);
    char* res = (char*)(malloc(sz));
    if (pointer_flag){
        if ((void*)x == NULL) return "(nil)";
        write(1, "0x", 2);
    }
    int p = sz - 1, neg = 0;
    if (x < 0) x = x * (-1), neg = 1;
    if (x == 0){
        res[0] = '0';
        return res;
    }
    while(x){
        int rem = x % base;
        if (rem >= 10){
            rem -= 10;
            res[p] = (char)(rem + 'a');
        } else res[p] = (char)(rem + '0');
        --p;
        x /= base;
    }
    if (neg) res[p] = '-';
    return res;
}

int my_printf(char* restrict format, ...){
    va_list ap;
    va_start(ap, format);
    char* buffer = (char*)(malloc(100));
    for(int i = 0; i < 95; ++i) buffer[i] = '\0';
    int p = 0, whole_len = 0;
    for(int i = 0; i < strlen(format); ++i){
        if (i + 1 < strlen(format) && !not_argument(format[i], format[i + 1])){
            write(1, buffer, p);
            whole_len += p;
            p = 0;
            if (format[i + 1] == 'd' || format[i + 1] == 'u'){
                int x = va_arg(ap, int);
                char* ans = convert(x, 10, 0);
                write(1, ans, strlen(ans));
                whole_len += strlen(ans);
            }
            if (format[i + 1] == 'o'){
                int x = va_arg(ap, int);
                char* ans = convert(x, 8, 0);
                write(1, ans, strlen(ans));
                whole_len += strlen(ans);
            }
            if (format[i + 1] == 'x'){
                int x = va_arg(ap, int);
                char* ans = convert(x, 16, 0);
                write(1, ans, strlen(ans));
                whole_len += strlen(ans);
            }
            if (format[i + 1] == 'c'){
                int x = va_arg(ap, int);
                char* ans = (char*)(malloc(1));
                ans[0] = (char)(x);
                write(1, ans, 1);
                whole_len++;
            }
            if (format[i + 1] == 's'){
                char* x = va_arg(ap, char*);
                write(1, x, strlen(x));
                whole_len += strlen(x);
            }
            if (format[i + 1] == 'p'){
                long long x = va_arg(ap, long long);
                char* ans = convert(x, 16, 1);
                write(1, ans, strlen(ans));
                whole_len += (strlen(ans) + 2);
            }
            ++i;
        } 
        else if (i + 1 < strlen(format) && format[i] == '/' && format[i + 1] == 'n'){
            write(1, buffer, p);
            write(1, "\n", 2);
            whole_len = whole_len + p + 1;
            p = 0;
        }
        else buffer[p] = format[i], ++p;
    }
    if (p > 0) write(1, buffer, p), whole_len += p;
    free(buffer);
    va_end(ap);
    return whole_len;
}   


int main (){
}
