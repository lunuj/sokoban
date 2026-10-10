#include "core.h"
#include "map.h"
#include "records.h"

bool mov(Map* map, MovAction* action)
{
    int w = get_map_width(map);
    int h = get_map_height(map);

    switch (action->direct)
    {
    case CMD_MOV_UP:
        action->pos_t = action->pos_f - w * action->step;
        break;
    case CMD_MOV_DOWN:
        action->pos_t = action->pos_f + w * action->step;
        break;
    case CMD_MOV_LEFT:
        action->pos_t = action->pos_f - action->step;
        break;
    case CMD_MOV_RIGHT:
        action->pos_t = action->pos_f + action->step;
        break;
    default:
        return false;
    }

    if (((action->pos_f + 1) % map->w == 0 && (action->pos_t + 1) % map->w == 1) ||
        ((action->pos_f + 1) % map->w == 1 && (action->pos_t + 1) % map->w == 0))
    {
        action->direct = CMD_NONE;
        return false;
    }

    if (action->pos_t < 0 || action->pos_t >= w * h)
    {
        action->direct = CMD_NONE;
        return false;
    }

    if (map_is(map, action->pos_t, MAP_BLOCK))
    {
        action->direct = CMD_NONE;
        return false;
    }

    if (map_is(map, action->pos_t, MAP_BOX))
    {
        if (map_is(map, action->pos_f, MAP_BOX))
        {
            action->direct = CMD_NONE;
            return false;
        }
        action->pos_f = action->pos_t;
        return mov(map, action);
    }

    if (map_is(map, action->pos_f, MAP_PLAYER))
    {
        map_clr(map, action->pos_f, MAP_PLAYER);
        map_set(map, action->pos_t, MAP_PLAYER);
    }

    if (map_is(map, action->pos_f, MAP_BOX))
    {
        map_clr(map, action->pos_f, MAP_BOX);
        map_set(map, action->pos_t, MAP_BOX);
    }

    return true;
}

MovAction core_update(Map* map, Records* records, int cmd)
{
    MovAction action;
    action.pos_f = find_player(map);
    action.direct = CMD_NONE;
    action.step = 1;

    switch (cmd)
    {
    case CMD_MOV_UP:
        action.direct = CMD_MOV_UP;
        if (mov(map, &action))
            records_add(records, &action);
        break;
    case CMD_MOV_DOWN:
        action.direct = CMD_MOV_DOWN;
        if (mov(map, &action))
            records_add(records, &action);
        break;
    case CMD_MOV_LEFT:
        action.direct = CMD_MOV_LEFT;
        if (mov(map, &action))
            records_add(records, &action);
        break;
    case CMD_MOV_RIGHT:
        action.direct = CMD_MOV_RIGHT;
        if (mov(map, &action))
            records_add(records, &action);
        break;
    case CMD_UNDO:
        action = records_undo(records);
        mov(map, &action);
        break;
    case CMD_REDO:
        action = records_redo(records);
        mov(map, &action);
        break;
    case CMD_BACK:
        // TODO to be implase
    default:
        break;
    }
    return action;
}

bool core_is_win(Map* map)
{
    int w = get_map_width(map);
    int h = get_map_height(map);

    for (int i = 0; i < w * h; i++)
    {
        if (map_get(map, i) == MAP_BOX)
            return false;
    }

    return true;
}
