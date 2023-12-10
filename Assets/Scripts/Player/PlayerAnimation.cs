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
    private float motionTime = 0f;
    private float attackDuration;
    private void Start()
    {
        anim = GetComponent<Animator>();
        attackDuration = GetComponent<Combat>().attackDuration;
    }

    private void PlayAttack()
    {
        if (motionTime >= attackDuration)
        {
            Ps.SetState(State.ATTACK, false);
            motionTime = 0f;
        }
    }
    private void Update()
    {

        int stateIdx = Ps.GetPrio();
        State curState = (State)(1 << stateIdx);

        // reset motionTime if state has changed
        if (curState != prevState)
        {
            anim.SetInteger("state", (int)stateIdx);
            motionTime = 0f;
        }

        // handle attack animation
        if (curState == State.ATTACK)
        {
            PlayAttack();
        }

        motionTime += Time.deltaTime;
        anim.SetFloat("motionTime", motionTime);
        prevState = curState;
    }
}