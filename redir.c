#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Get input file, output file, and command
    char *in_file = argv[1];
    char *cmd = argv[2];
    char *out_file = argv[3];

    char *cmd_args[10];         // Stores command and arguments
    int ix = 0;

    // Split command into command and arguments
    char *cmd_token = strtok(cmd, " ");
    while (cmd_token != NULL && ix < 9) {
        cmd_args[ix] = cmd_token;
        ix++;
        cmd_token = strtok(NULL, " ");
    }
    cmd_args[ix] = NULL;        // End array with NULL

    pid_t pid = fork();
    if (pid < 0) {
       printf("Fork failed");
       return 1;
    } else if (pid == 0) {      // Child process
        // Redirect input
        int in_file_desc = open(in_file, O_RDONLY);
        dup2(in_file_desc, STDIN_FILENO);
        close(in_file_desc);

        //Redirect output
        int out_file_desc = open(out_file, O_WRONLY);
        dup2(out_file_desc, STDOUT_FILENO);
        close(out_file_desc);

        execvp(cmd_args[0], cmd_args);  // Execute command
    } else {                    // Parent process
        wait(NULL);
    }

    return 0;
}