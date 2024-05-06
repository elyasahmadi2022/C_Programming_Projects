#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

// Function to count the number of files in a directory
int countFiles(const char *dirPath) {
    int count = 0;
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirPath);
    if (dir == NULL) {
        printf("Unable to open directory '%s'\n", dirPath);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            count++;
        }
    }

    closedir(dir);
    return count;
}

// Function to get the file extension from a given filename
const char *getFileExtension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return "";
    }
    return dot + 1;
}

// Function to check if a format already exists in the formatCounts array
int formatExists(const char *formatCounts[], const char *format) {
    for (int i = 0; formatCounts[i] != NULL; i++) {
        if (strcmp(formatCounts[i], format) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to count the number of files with each format and display the table
void countFormatsAndDisplay(const char *dirPath) {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;

    dir = opendir(dirPath);
    if (dir == NULL) {
        printf("Unable to open directory '%s'\n", dirPath);
        return;
    }

    int totalFiles = 0;
    const char *formatCounts[100] = {NULL};
    int formatNumbers[100] = {0};
    double formatSizes[100] = {0};
    double totalSize = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            char filePath[256];
            strcpy(filePath, dirPath);
            strcat(filePath, "/");
            strcat(filePath, entry->d_name);

            if (stat(filePath, &fileStat) == 0) {
                totalFiles++;

                const char *ext = getFileExtension(entry->d_name);

                // Check if the format already exists in the formatCounts array
                if (!formatExists(formatCounts, ext)) {
                    int formatIndex = 0;
                    while (formatCounts[formatIndex] != NULL) {
                        formatIndex++;
                    }
                    formatCounts[formatIndex] = ext;
                }

                // Find the index of the format in the formatCounts array
                int formatIndex = 0;
                while (strcmp(formatCounts[formatIndex], ext) != 0) {
                    formatIndex++;
                }

                // Increment the number of files, add the file size for the corresponding format,
                // and update the total size
                formatNumbers[formatIndex]++;
                formatSizes[formatIndex] += fileStat.st_size;
                totalSize += fileStat.st_size;
            }
        }
    }

    closedir(dir);

    printf("Total Files:\t\t\t  File Format:\t\t\t  Total Size (KB):\n");
    for (int i = 0; formatCounts[i] != NULL; i++) {
        printf("%12d%25s%25.2f\n", formatNumbers[i], formatCounts[i], formatSizes[i] / 1024);
    }

    printf("------------------------------------------------------\n");
    printf("Total Number of Files:\t%d\n", totalFiles);
    printf("Total Size of Files:\t%.2f KB\n", totalSize / 1024);
}

int main() {
    char directory[256];
    printf("Enter the directory path: ");
    scanf("%255s", directory);

    countFormatsAndDisplay(directory);

    return 0;
}