#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define MAX_LINE_LENGTH 256
#define MAX_THREADS 256

typedef struct {
    const char* filename;
    const char* target_string;
} ThreadData;

void* search_file(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    char line[MAX_LINE_LENGTH];
    FILE* file = fopen(data->filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    int line_number = 1;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (strstr(line, data->target_string) != NULL) {
            printf("%s:%d: %s", data->filename, line_number, line);
        }
        line_number++;
    }

    fclose(file);
    pthread_exit(NULL);
}

void search_files(const char* folder_path, const char* target_string) {
    DIR* directory;
    struct dirent* entry;

    directory = opendir(folder_path);
    if (directory == NULL) {
        perror("Error opening directory");
        return;
    }

    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    int num_threads = 0;

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s", folder_path, entry->d_name);

            thread_data[num_threads].filename = strdup(filepath);
            thread_data[num_threads].target_string = target_string;

            pthread_create(&threads[num_threads], NULL, search_file, &thread_data[num_threads]);
            num_threads++;

            if (num_threads >= MAX_THREADS) {
                break;
            }
        }
    }

    closedir(directory);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        free((void*)thread_data[i].filename);
    }
}

int main() {
    char folder_path[256] = "/data/workspace/myshixun/texts";
    char target_string[256];

    scanf("%s", target_string);
    search_files(folder_path, target_string);

    return 0;
}
