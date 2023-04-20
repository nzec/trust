#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

int main() {
    struct passwd *p = getpwuid(getuid());
    char *user = p->pw_name;
    printf("Current User Logged in: %s\n", user);
    if (getuid() == 0) {
        printf("Authorized ✔\n");
    } else {
        printf("Not Authorized ❌\n");
    }
}
