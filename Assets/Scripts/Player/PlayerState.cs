using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;

public static class PlayerState
{
    // The list is ranked in which state is "prioritized" (higher value = higher priority)
    [Flags]
    public enum State
    {
        IDLE    =   1 << 0,
        LEFT    =   1 << 1,
        RIGHT   =   1 << 2,
        RUNNING =   1 << 3,
        FALL    =   1 << 4,
        JUMP    =   1 << 5,
        HURT    =   1 << 6,
        ATTACK  =   1 << 7,
    }
    static State cur_state = State.IDLE;

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

    // Returns the index of the most "urgent" visual state
    public static int GetPrio()
    {
        string bin = Convert.ToString((int)cur_state, 2);

        for (int i = 0; i < bin.Length; i++)
        {
            if (bin[i] == '1')
                return bin.Length - i - 1;
        }

        // ERROR we should not've come to this!
        Debug.Assert(true, "player is stateless!!!");
        return -1;
    }

    public static State GetCopy()
    {
        return cur_state;
    }

    // Debug purposes
    public static void PrintState()
    {
        Debug.Log(cur_state);
    }
    // Debug purposes
    public static void PrintStateBin()
    {
        string mask = Convert.ToString((int)cur_state, 2);
        Debug.Log(mask);
    }
}
