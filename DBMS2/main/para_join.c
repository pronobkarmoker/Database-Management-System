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

int main() {
    const char* file1 = "data1.txt";
    const char* file2 = "data2.txt";
    const char* output_file = "joined_output.txt";

    // Count lines in both files
    int size1 = count_lines(file1);
    int size2 = count_lines(file2);

    if (size1 <= 0 || size2 <= 0) {
        printf("Error reading input files\n");
        return 1;
    }

    // Allocate memory for records
    Record* records1 = (Record*)malloc(size1 * sizeof(Record));
    Record* records2 = (Record*)malloc(size2 * sizeof(Record));

    if (!records1 || !records2) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Load data from files
    load_data(file1, records1, size1);
    load_data(file2, records2, size2);

    // Open output file
    FILE* out_file = fopen(output_file, "w");
    if (!out_file) {
        printf("Error opening output file\n");
        free(records1);
        free(records2);
        return 1;
    }

    // Calculate chunk size for file1
    int chunk_size = size1 / 4;
    if (chunk_size == 0) chunk_size = 1;

    // Perform parallel join operation
    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();
        int start = thread_id * chunk_size;
        int end = (thread_id == 3) ? size1 : (thread_id + 1) * chunk_size;

        join_chunk(records1, start, end, records2, size2, out_file);
    }

    // Cleanup
    fclose(out_file);
    free(records1);
    free(records2);

    printf("Join operation completed. Results written to %s\n", output_file);
    return 0;
}