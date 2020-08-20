struct member{
  int uid;
  char name[50];
  char password[50];
  int level;  // level 1-3 for sharing speed
  int friend_list[20]; // max 20 friends, record by uid
  int favour_movie[50]; //record by movie_id.
};


//// global variables ///
