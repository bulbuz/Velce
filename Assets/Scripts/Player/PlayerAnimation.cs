using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Unity.Mathematics;
using UnityEditor.UI;
using UnityEngine;

public class MovementState
{
    public bool IsRunning {  get; set; }
    public bool IsJumping { get; set; }
    public bool IsFalling { get; set; }

    public void UpdateState(Rigidbody2D rb)
    {
        if (rb.velocity.y < -0.01f)
        {
            IsJumping = false;
            IsFalling = true;
        }
        else
        {
            IsFalling = false;
        }
    }
    public AnimationState GetAnimState()
    {
        if (IsFalling)
        {
            return AnimationState.falling;
        }
        else if (IsJumping)
        {
            return AnimationState.jumping;
        }
        else if (IsRunning)
        {
            return AnimationState.running;
        }
        return AnimationState.idle;
    }
}

public class AttackState
{
    public float PunchingTime { get; set; }

    public void UpdateState()
    {
        PunchingTime -= Time.deltaTime;
    }
    public AnimationState GetAttackAnim()
    {
        if (PunchingTime > 0)
        {
            return AnimationState.punching;
        }
        return AnimationState.none;
    }
}

public enum AnimationState
{
    none,
    idle,
    running,
    jumping,
    falling,
    punching,
}

public class PlayerAnimation : MonoBehaviour
{
    private Animator anim;
    private Rigidbody2D rb;
    
    public MovementState moveState = new();
    public AttackState attackState = new();
    private AnimationState animState = new();

    private float motionTime = 0f;
    private void Start()
    {
        anim = GetComponent<Animator>();
        rb = GetComponent<Rigidbody2D>();
    }

    private void Update()
    {
        AnimationState prevAnimState = animState;
        motionTime += Time.deltaTime;

        moveState.UpdateState(rb);
        attackState.UpdateState();

        animState = attackState.GetAttackAnim();
        if (animState == AnimationState.none)
        {
            animState = moveState.GetAnimState();
        }

        if (animState != prevAnimState)
        {
            anim.SetInteger("state", (int)animState);
            motionTime = 0f;
        }
        anim.SetFloat("motionTime", motionTime);
    }
}
