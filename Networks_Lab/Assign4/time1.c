#include <time.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char time_string1[] = "Tue, 15 Nov 1994 12:45:26 GMT";
    char time_string2[] = "Tue, 17 Nov 1994 12:45:26 GMT";
    struct tm tm1, tm2;
    memset(&tm1, 0, sizeof(struct tm));
    memset(&tm2, 0, sizeof(struct tm));
    strptime(time_string1, "%a, %d %b %Y %T GMT", &tm1);
    strptime(time_string2, "%a, %d %b %Y %T GMT", &tm2);
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    printf("%ld %ld \n",t2 ,t1);
    if (difftime(t2, t1) >= 172800) {
        printf("Time2 is more than 2 days after time1.\n");
    } else {
        printf("Time2 is less than 2 days after time1.\n");
    }
    return 0;
}
