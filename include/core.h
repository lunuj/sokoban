#pragma once

#define BOLCK '#'
#define SPACE ' '
#define PLAYER 'A'
#define TARGET '_'
#define BOX 'X'
#define PLAYER_ON_TARGET 'B'
#define BOX_ON_TARGET 'O'

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

void update(char c);

void mov(int pos_f, int direct, int step);
int find_player();

bool is_win();

void records_add(int pos_f, int direct, int step, int pos_t);
void records_undo();
