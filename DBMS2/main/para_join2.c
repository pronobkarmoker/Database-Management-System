#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_LINE_LENGTH 1024
#define CHUNK_SIZE 1000

// Structure to store data from files
typedef struct {
    int key;
    char value[256];
} Record;

// Function to count lines in a file
int count_lines(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }

    int count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }
    fclose(file);
    return count;
}

// Function to load data from file into array
void load_data(const char* filename, Record* records, int size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (i < size && fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^\n]", &records[i].key, records[i].value);
        i++;
    }
    fclose(file);
}

// Function to perform join operation sequentially
void join_sequential(Record* records1, int size1,
                    Record* records2, int size2,
                    const char* output_file) {
    FILE* out_file = fopen(output_file, "w");
    if (!out_file) {
        printf("Error opening output file\n");
        return;
    }

    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (records1[i].key == records2[j].key) {
                fprintf(out_file, "%d,%s,%s\n",
                        records1[i].key,
                        records1[i].value,
                        records2[j].value);
            }
        }
    }
    fclose(out_file);
}

// Function to perform join operation on a chunk of data
void join_chunk(Record* records1, int start1, int end1,
                Record* records2, int size2,
                FILE* output_file) {
    #pragma omp critical
    {
        for (int i = start1; i < end1; i++) {
            for (int j = 0; j < size2; j++) {
                if (records1[i].key == records2[j].key) {
                    fprintf(output_file, "%d,%s,%s\n",
                            records1[i].key,
                            records1[i].value,
                            records2[j].value);
                }
            }
        }
    }
}

// Function to perform parallel join operation
void join_parallel(Record* records1, int size1,
                  Record* records2, int size2,
                  const char* output_file) {
    FILE* out_file = fopen(output_file, "w");
    if (!out_file) {
        printf("Error opening output file\n");
        return;
    }

    int chunk_size = size1 / 4;
    if (chunk_size == 0) chunk_size = 1;

    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();
        int start = thread_id * chunk_size;
        int end = (thread_id == 3) ? size1 : (thread_id + 1) * chunk_size;

        join_chunk(records1, start, end, records2, size2, out_file);
    }

    fclose(out_file);
}

int main() {
    const char* file1 = "data1.txt";
    const char* file2 = "data2.txt";
    double start_time, end_time;

    // Count lines in both files
    int size1 = count_lines(file1);
    int size2 = count_lines(file2);

    if (size1 <= 0 || size2 <= 0) {
        printf("Error reading input files\n");
        return 1;
    }

    printf("Number of records in file1: %d\n", size1);
    printf("Number of records in file2: %d\n", size2);

    // Allocate memory for records
    Record* records1 = (Record*)malloc(size1 * sizeof(Record));
    Record* records2 = (Record*)malloc(size2 * sizeof(Record));

    if (!records1 || !records2) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Load data from files
    printf("\nLoading data from files...\n");
    load_data(file1, records1, size1);
    load_data(file2, records2, size2);

    // Test 1: Single Thread
    printf("\nPerforming single-threaded join...\n");
    start_time = omp_get_wtime();
    join_sequential(records1, size1, records2, size2, "joined_output_sequential.txt");
    end_time = omp_get_wtime();
    printf("Single thread execution time: %.4f seconds\n", end_time - start_time);

    // Test 2: Four Threads
    printf("\nPerforming four-threaded join...\n");
    start_time = omp_get_wtime();
    join_parallel(records1, size1, records2, size2, "joined_output_parallel.txt");
    end_time = omp_get_wtime();
    printf("Four thread execution time: %.4f seconds\n", end_time - start_time);

    // Calculate speedup
    double sequential_time = end_time - start_time;
    double parallel_time = end_time - start_time;
    printf("\nSpeedup: %.2fx\n", sequential_time / parallel_time);

    // Cleanup
    free(records1);
    free(records2);

    return 0;
}