#pragma once

#include <stdbool.h>

#define RECORD_SIZE 1024

typedef struct MovAction
{
    int pos_f;
    int direct;
    int step;
    int pos_t;
} MovAction;

typedef struct Records
{
    int undo_count;
    int redo_count;
    MovAction undo_stack[RECORD_SIZE];
    MovAction redo_stack[RECORD_SIZE];
} Records;

void records_add(Records* records, MovAction* action);
MovAction records_undo(Records* records);
MovAction records_redo(Records* records);
int records_get_count(Records *records);
