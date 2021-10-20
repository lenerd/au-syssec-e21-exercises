#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: %s <file to print>\n", argv[0]);
        exit(1);
    }

    // print the currend real and effective user ids
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("Real user id: %d\n", uid);
    printf("Effective user id: %d\n", euid);

    // name of file to print
    const char* file_name = argv[1];
    printf("Selected file: %s\n", file_name);

    // test if the file is readable
    // access uses the real user id
    printf("Testing access\n");
    int result = access(file_name, R_OK);
    if (result == -1) {
        perror("no read access");
        exit(1);
    }

    // open the file
    int file_descriptor = open(file_name, O_RDONLY);
    if (file_descriptor == -1) {
        perror("could not open file");
        exit(1);
    }

    // reading file attributes
    struct stat statbuf;
    result = fstat(file_descriptor, &statbuf);
    if (result == -1) {
        perror("fstat failed");
        exit(1);
    }

    // printing file size
    size_t file_size = statbuf.st_size;
    printf("File has size %zu\n", file_size);

    printf("Printing file now:\n");
    printf("--------------------------------------------------------------------------------\n");

    // writing the file contents to standard output
    size_t bytes_sent = 0;
    while (bytes_sent < file_size) {
        ssize_t result = sendfile(1, file_descriptor, NULL, file_size);
        if (result == -1) {
            perror("sendfile failed");
            exit(1);
        }
        bytes_sent += result;
    }

    return 0;
}
