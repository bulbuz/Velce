using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerState : MonoBehaviour
{
    // The list is ranked in which state is "prioritized" (lower index = higher priority)
    [Flags]
    public enum State
    {
        ATTACK  = 1 << 0,
        HURT    = 1 << 1,
        JUMP    = 1 << 2,
        FALL    = 1 << 3,
        RIGHT   = 1 << 4,
        LEFT    = 1 << 5,
        IDLE    = 1 << 6,
    }
    State cur_state;
    // Start is called before the first frame update
    void Start()
    {
        cur_state = State.IDLE;
    }

    public void SetState(State state, bool value)
    {
        if (value && (int)(cur_state & state) == 0)
        {
            cur_state |= state;
        } 
        else if (!value && (int)(cur_state & state) == 1)
        {
            cur_state &= ~state;
        }
    }

    public bool GetState(State state)
    {
        return (int)(cur_state & state) == 1;
    }

    // Debug purposes
    public void PrintState()
    {
        Debug.Log((int)cur_state);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
