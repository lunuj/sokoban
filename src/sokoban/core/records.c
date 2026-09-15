#include <assert.h>

#include "core.h"
#include "records.h"

void records_add(Records* records, MovAction* action)
{
    assert(records->record_count < RECORD_SIZE && "record too big!!");
    records->action[records->record_count] = *action;
    records->record_count++;
}

MovAction records_undo(Records* records)
{
    MovAction action;

    if (records->record_count > 0)
    {
        records->record_count--;
        action = records->action[records->record_count];
        action.pos_f = action.pos_t;

        switch (action.direct)
        {
        case CMD_MOV_UP:
            action.direct = CMD_MOV_DOWN;
            break;
        case CMD_MOV_DOWN:
            action.direct = CMD_MOV_UP;
            break;
        case CMD_MOV_LEFT:
            action.direct = CMD_MOV_RIGHT;
            break;
        case CMD_MOV_RIGHT:
            action.direct = CMD_MOV_LEFT;
            break;
        default:
            break;
        }
    }

    return action;
}

MovAction records_redo(Records* records)
{
    MovAction action;

    records->record_count++;
    if (records->action->direct != CMD_NONE)
    {
        action = records->action[records->record_count];
    }

    return action;
}