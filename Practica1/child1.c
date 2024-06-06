#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    
    const char *params = "Read";

    
    int pipe_fd = 1;  

    write(pipe_fd, params, strlen(params));
    close(pipe_fd);

    return 0;
}
