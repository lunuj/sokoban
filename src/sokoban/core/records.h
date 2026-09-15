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
    int record_count;
    MovAction action[RECORD_SIZE];
} Records;

void records_add(Records *records, MovAction *action);
MovAction records_undo(Records *records);
MovAction records_redo(Records* records);
