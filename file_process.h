int getFileSize(const char *file_name);
int getFileToBuf(char* buf, const char* file_name);
char* readFileToMemory(const char* file_name);
int create_file(char* file_name);
char* fgets_plus(char* buf, int n, FILE* stream);
int LenBetweenTwoDelim(char* buf, char first_delim, char second_delim);
