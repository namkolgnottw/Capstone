#include "websites_function.h"

struct member* loginCheck(char* username, char* password) {
  char member_file_buf[8096];
  int fd = open("member_data.txt", O_RDONLY);
  
}

int memberRegister(char* username, char* password) {
 int new_uid = getuid();//
 // write data into the  member file 

  return 1; // 1 for succeed.
}

int logout(int uid) {
}

int comment(char* string, int uid) {
  // write/update comments to chatboard.txt
  // write user_name, time, date
}

// get recent newest 30 line comments.
char* getRecentComment() {  
};

int downloadFile(int uid, int user_level) {
}
