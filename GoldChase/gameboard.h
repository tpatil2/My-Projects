#ifndef gameboard_h
#define gameboard_h


struct GameBoard {
  int row_s; //4 bytes
  int col_s; //4 bytes
  pid_t pid[5];
  int daemonID;
  unsigned char players;
  unsigned char map[0];
};

void create_daemon();
void client_daemon(char* addr);



#endif
