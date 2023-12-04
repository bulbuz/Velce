using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class PlayerState
{
    // The list is ranked in which state is "prioritized" (lower index = higher priority)
    [Flags]
    public enum State
    {
        ATTACK  =   1 << 0,
        HURT    =   1 << 1,
        JUMP    =   1 << 2,
        FALL    =   1 << 3,
        RIGHT   =   1 << 4,
        LEFT    =   1 << 5,
        IDLE    =   1 << 6,
    }
    static State cur_state;

    public static void SetState(State state, bool value)
    {
        if (value && (int)(cur_state & state) == 0)
        {
            cur_state |= state;
        }
        else if (!value && (int)(cur_state & state) != 0)
        {
            cur_state &= ~state;
        }
    }

    public static bool GetState(State state)
    {
        return (int)(cur_state & state) != 0;
    }

    public static State GetCopy()
    {
        return cur_state;
    }

    // Debug purposes
    public static void PrintState()
    {
        /*
        string mask = Convert.ToString((int)cur_state, 2);
        Debug.Log(mask);
        */
        Debug.Log(cur_state);
    }
}
