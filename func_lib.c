#include <stdio.h>
#include <stdlib.h>

char * my_strncpy (char *, const char *, int);
int my_strlen (char *);
char * my_strstr (char *, char *);
int my_strstr_offset (char *, char *);
int my_strstr_count (char *, char *);
char * my_strrep(char *, char *, char *);
void my_swap_intp (int *, int *);
void my_qsort (int *, int);
void out_number(char *, int);

void my_qsort (int *src, int n) {
    int *l, *r, *s, *x;
    int ln, rn;

    s = src;
    l = s + 1;
    r = s + (n - 1);

    for (; (l != r) && (r > l);) {
        //printf("基准 = %d\n", *s);
        for (; (*r >= *s) && (l != r);) {
            //printf("右越过%d\n", *r);
            r--;
        }
        for (; (*l < *s) && (l != r);) {
            //printf("左越过%d\n", *l);
            l++;
        }
        if (l != r) {
            //printf("交换%d - %d\n", *l, *r);
            my_swap_intp(l, r);
            r--;
        }
    }

    if (*r < *s) {
        //printf("最终交换r %d - %d\n", *r, *s);
        my_swap_intp(s, r);
        x = r;
    } else {
        x = r - 1;
    }

    //printf("拆分点 %d\n", *(x));

    ln = x - src;
    rn = n - ln - 1;

    if (ln > 1) {
      my_qsort(src,ln);
    }
    if (rn > 1) {
      my_qsort(x + 1,rn);
    }

}

void my_swap_intp (int *a, int *b) {
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

/*
    字符串替换
*/
char * my_strrep(char *src, char *sub, char *rep) {
    int srclen, sublen, replen, newstrlen, count, i, x;
    char *newstr, *newstr_0;
    srclen = my_strlen(src);
    sublen = my_strlen(sub);
    replen = my_strlen(rep);
    count = my_strstr_count(src, sub);
    //printf("count = %d\n", count);
    newstrlen = (srclen - sublen * count) + (replen * count) + 1;
    newstr = (char *)malloc(newstrlen);
    newstr_0 = newstr;

    if (!*src) {
        return NULL;
    }

    while (*src != '\0') {
        x = 0;
        for (i = 0; *(src + i) == *(sub + i); i++) {
            if (*(sub + i + 1) == '\0') {
                //匹配成功
                my_strncpy(newstr, rep, replen);
                src = src + i;
                newstr = newstr + replen - 1;
                x = 1;
            }
        }
        if (x == 0) {
            *newstr = *src;
        }
        src++;
        newstr++;
    }
    *newstr = '\0';

    return newstr_0;
}


/*
    检测字符串是否包含另一个字符串
    返回位置偏移量
*/
int my_strstr_offset (char *src, char *sub) {
    int offset = 0, i;
    char *res;

    if (!*src) {
        return -1;
    }

    while (*src != '\0') {
        for (i = 0; *(src + i) == *(sub + i); i++) {
            if (*(sub + i + 1) == '\0') {
                //匹配成功
                return offset;
            }
        }
        src++;
        offset++;
    }

    return -1;
}

/*
    检测字符串是否包含另一个字符串
    返回偏移位置指针
*/
char * my_strstr (char *src, char *sub) {
    int number, i;
    char *res;

    if (!*src) {
        return src;
    }

    while (*src != '\0') {
        for (i = 0; *(src + i) == *(sub + i); i++) {
            if (*(sub + i + 1) == '\0') {
                //匹配成功
                return src;
            }
        }
        src++;
    }

    return NULL;
}

/*
    计算一个字符串在另一个字符串中出现的次数
*/
int my_strstr_count (char *src, char *sub) {
    int number, i;
    number = 0;

    if (!*src) {
        return number;
    }

    while (*src != '\0') {
        for (i = 0; *(src + i) == *(sub + i); i++) {
            if (*(sub + i + 1) == '\0') {
                //匹配成功
                number++;
                src = src + i;
            }
        }
        src++;
    }

    return number;
}

/*
    检测字符串长度
*/
int my_strlen (char *str) {
  int count = 0;
  while (*str++ != '\0') {
    count++;
  }
  return count;
}

/*
    完全复制字符串
*/
char * my_strncpy (char *dst, const char *src, int size) {
    char *newstr;
    newstr = dst;
    while ( ((*(dst++) = *(src++)) != '\0') && (dst < (newstr + size)) ) {
        ;
    }
    *++dst = '\0';
    return newstr;
}

/*
    输出计算器数字
*/
void out_number(char *in, int in_len)
{
    int i;
    int j;
    int n;
    int c0 = 48;
    char *a[][10] =
    {
        {" _ ", "| |", "|_|"},
        {"   ", "  |", "  |"},
        {" _ ", " _|", "|_ "},
        {" _ ", " _|", " _|"},
        {"   ", "|_|", "  |"},
        {" _ ", "|_ ", " _|"},
        {" _ ", "|_ ", "|_|"},
        {" _ ", "  |", "  |"},
        {" _ ", "|_|", "|_|"},
        {" _ ", "|_|", " _|"},
    };

    for (j = 0; j < 3; j++) {
        for (i = 0; i < in_len; i++) {
            n = (int)in[i] - c0;
            printf("%s", a[n][j]);
        }
        printf("\n");
    }
}