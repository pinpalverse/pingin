// This test file is written with AI
// Cuz who likes testing stuff, right?
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "../conf_parser.h"

// Helper function to create a temporary config file
char* create_temp_config_file(const char* content) {
    char temp_filename[] = "/tmp/config_test_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        perror("mkstemp");
        return NULL;
    }
    
    FILE* temp_file = fdopen(fd, "w");
    if (!temp_file) {
        perror("fdopen");
        close(fd);
        return NULL;
    }
    
    fprintf(temp_file, "%s", content);
    fclose(temp_file);
    
    return strdup(temp_filename);
}

// Helper function to cleanup
void cleanup_config(Conf* cfg) {
    if (cfg->filename) free(cfg->filename);
    if (cfg->values) {
        for (int i = 0; i < cfg->columns; i++) {
            if (cfg->values[i]) {
                if (cfg->values[i]->k) free(cfg->values[i]->k);
                if (cfg->values[i]->v) free(cfg->values[i]->v);
                free(cfg->values[i]);
            }
        }
        free(cfg->values);
    }
}

// Test 1: Basic integer parsing
void test_basic_integer() {
    printf("Running test_basic_integer... ");
    
    const char* config_content = "age=25\ncount=100\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(strcmp(cfg.values[0]->k, "age") == 0);
    assert(strcmp(cfg.values[0]->v, "25") == 0);
    assert(cfg.values[0]->v_type == INT);
    assert(strcmp(cfg.values[1]->k, "count") == 0);
    assert(cfg.values[1]->v_type == INT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 2: Float parsing
void test_float_parsing() {
    printf("Running test_float_parsing... ");
    
    const char* config_content = "pi=3.14159\nratio=0.5\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(strcmp(cfg.values[0]->k, "pi") == 0);
    assert(strcmp(cfg.values[0]->v, "3.14159") == 0);
    assert(cfg.values[0]->v_type == FLOAT);
    assert(cfg.values[1]->v_type == FLOAT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 3: Boolean parsing
void test_boolean_parsing() {
    printf("Running test_boolean_parsing... ");
    
    const char* config_content = "enabled=true\ndisabled=false\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(strcmp(cfg.values[0]->k, "enabled") == 0);
    assert(strcmp(cfg.values[0]->v, "true") == 0);
    assert(cfg.values[0]->v_type == BOOL);
    assert(cfg.values[1]->v_type == BOOL);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 4: Text parsing with quotes
void test_text_parsing() {
    printf("Running test_text_parsing... ");
    
    const char* config_content = "name=\"John Doe\"\ndescription=\"Hello World\"\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(strcmp(cfg.values[0]->k, "name") == 0);
    assert(strcmp(cfg.values[0]->v, "John Doe") == 0);
    assert(cfg.values[0]->v_type == TEXT);
    assert(cfg.values[1]->v_type == TEXT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 5: Comments handling
void test_comments() {
    printf("Running test_comments... ");
    
    const char* config_content = "# This is a comment\nname=\"test\"\n# Another comment\nvalue=123\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(strcmp(cfg.values[0]->k, "name") == 0);
    assert(strcmp(cfg.values[1]->k, "value") == 0);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 6: Mixed types
void test_mixed_types() {
    printf("Running test_mixed_types... ");
    
    const char* config_content = "count=42\npi=3.14\nactive=true\ndesc=\"text\"\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 4);
    assert(cfg.values[0]->v_type == INT);
    assert(cfg.values[1]->v_type == FLOAT);
    assert(cfg.values[2]->v_type == BOOL);
    assert(cfg.values[3]->v_type == TEXT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 7: Empty file
void test_empty_file() {
    printf("Running test_empty_file... ");
    
    const char* config_content = "";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 0);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 8: Lines with only comments
void test_only_comments() {
    printf("Running test_only_comments... ");
    
    const char* config_content = "# comment1\n# comment2\n# comment3\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 0);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 9: Non-existent file
void test_nonexistent_file() {
    printf("Running test_nonexistent_file... ");
    
    Conf cfg = {0};
    int result = parse("/tmp/nonexistent_file_12345.txt", &cfg);
    
    assert(result == 1); // Should return error code 1
    assert(cfg.filename == NULL);
    
    printf("PASSED\n");
}

// Test 10: Invalid syntax
void test_invalid_syntax() {
    printf("Running test_invalid_syntax... ");
    
    const char* config_content = "invalid line without equals\nkey==value\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 2); // Should return error code 2 for parsing error
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 11: Large numbers
void test_large_numbers() {
    printf("Running test_large_numbers... ");
    
    const char* config_content = "bigint=999999999\nbigfloat=12345.67890\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(cfg.values[0]->v_type == INT);
    assert(cfg.values[1]->v_type == FLOAT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 12: Special characters in text
void test_text_special_characters() {
    printf("Running test_text_special_characters... ");
    
    const char* config_content = "special=\"Hello, World!\"\npath=\"C:\\\\test\\\\file\"\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(cfg.columns == 2);
    assert(cfg.values[0]->v_type == TEXT);
    assert(cfg.values[1]->v_type == TEXT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

// Test 13: Whitespace handling (note: your parser doesn't trim whitespace)
void test_whitespace() {
    printf("Running test_whitespace... ");
    
    const char* config_content = "key   =   value\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    // Your parser includes whitespace in the key
    assert(result != 0); // Should fail or behave as implemented
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED (expected behavior verified)\n");
}

// Test 14: Multiple files (ensuring no memory leaks)
void test_multiple_files() {
    printf("Running test_multiple_files... ");
    
    const char* config1 = "test1=100\n";
    const char* config2 = "test2=200\n";
    
    char* temp_file1 = create_temp_config_file(config1);
    char* temp_file2 = create_temp_config_file(config2);
    assert(temp_file1 != NULL && temp_file2 != NULL);
    
    Conf cfg1 = {0}, cfg2 = {0};
    
    int result1 = parse(temp_file1, &cfg1);
    int result2 = parse(temp_file2, &cfg2);
    
    assert(result1 == 0 && result2 == 0);
    assert(cfg1.columns == 1);
    assert(cfg2.columns == 1);
    
    cleanup_config(&cfg1);
    cleanup_config(&cfg2);
    free(temp_file1);
    free(temp_file2);
    printf("PASSED\n");
}

// Test 15: Text with spaces
void test_text_with_spaces() {
    printf("Running test_text_with_spaces... ");
    
    const char* config_content = "message=\"Hello World from Config\"\n";
    char* temp_file = create_temp_config_file(config_content);
    assert(temp_file != NULL);
    
    Conf cfg = {0};
    int result = parse(temp_file, &cfg);
    
    assert(result == 0);
    assert(strcmp(cfg.values[0]->v, "Hello World from Config") == 0);
    assert(cfg.values[0]->v_type == TEXT);
    
    cleanup_config(&cfg);
    free(temp_file);
    printf("PASSED\n");
}

int main() {
    printf("Starting configuration parser tests...\n");
    printf("========================================\n");
    
    test_basic_integer();
    test_float_parsing();
    test_boolean_parsing();
    test_text_parsing();
    test_comments();
    test_mixed_types();
    test_empty_file();
    test_only_comments();
    test_nonexistent_file();
    test_invalid_syntax();
    test_large_numbers();
    test_text_special_characters();
    test_whitespace();
    test_multiple_files();
    test_text_with_spaces();
    
    printf("========================================\n");
    printf("All tests completed!\n");
    
    return 0;
}