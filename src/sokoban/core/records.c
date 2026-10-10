#include <assert.h>

#include "core.h"
#include "records.h"

void records_add(Records* records, MovAction* action)
{
    assert(records->undo_count < RECORD_SIZE && "record too big!!");
    records->undo_stack[records->undo_count++] = *action;
    records->redo_count = 0;
}

MovAction records_undo(Records* records)
{
    MovAction action;

    /* TODO */
    /* mov() 推箱子时action.pos_f 会被递归改写 */
    /* 撤销时只反转一步方向，但推箱链可能移动了多个箱子 */
    /* 要么限制"一次只推一个箱子"，要么记录整条链 */
    /* 是否支持多箱连环 */
    action.direct = CMD_NONE;

    if (records->undo_count > 0)
    {
        action = records->undo_stack[--records->undo_count];
        records->redo_stack[records->redo_count++] = action;
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

    if (records->redo_count > 0)
    {
        action = records->redo_stack[--records->redo_count];
        records->undo_stack[records->undo_count++] = action;
    }

    return action;
}

int records_get_count(Records* records)
{
    return records->undo_count;
}