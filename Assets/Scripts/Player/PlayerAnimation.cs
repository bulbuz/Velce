using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Unity.Mathematics;
using UnityEngine;

// abbreviations for the stupid but descriptive names -_-
using Ps = PlayerState;
using State = PlayerState.State;

public class PlayerAnimation : MonoBehaviour
{
    private Animator anim;
    private State prevState = State.IDLE;
    private float attackDuration;
    private int attackCombo = 0;

    private void Start()
    {
        anim = GetComponent<Animator>();
        attackDuration = GetComponent<Combat>().attackDuration;
    }

    private void HandleAttack()
    {
        if (Ps.GetState(State.RUNNING))
        {
            if (attackCombo % 2 == 0)
            {
                anim.SetInteger("state", 9);
            }
            else
            {
                anim.SetInteger("state", 10);
            }
        }
        else
        {
            if (attackCombo % 2 == 0)
            {
                anim.SetInteger("state", 7);
            }
            else
            {
                anim.SetInteger("state", 8);
            }
        }

        attackCombo++;
    }
    private void Update()
    {
        int stateIdx = Ps.GetPrio();
        State curState = (State)(1 << stateIdx);

        // reset motionTime if state has changed
        if (curState != prevState)
        {
            if (curState == State.ATTACK)
            {
                HandleAttack();
            }
            else
            {
                anim.SetInteger("state", (int)stateIdx);
            }
        }
        else
        {
            anim.SetInteger("state", -1);
        }

        prevState = curState;
    }
}