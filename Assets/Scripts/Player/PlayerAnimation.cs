using System.Collections;
using System.Collections.Generic;
using UnityEngine;
struct State
{
    public bool running { get; set; }
    public bool jumping { get; set; }
    public bool falling { get; set; }
}

public class PlayerAnimation : MonoBehaviour
{
    Animator anim;
    Rigidbody2D rb;

    float dX, dY = 0;
    float prevX, prevY;

    State state;

    private void Start()
    {
        anim = GetComponent<Animator>();
        rb = GetComponent<Rigidbody2D>();

        prevX = rb.position.x;
        prevY = rb.position.y;
    }

    private void FixedUpdate()
    {
        dX = rb.position.x - prevX;
        dY = rb.position.y - prevY;

        prevX = rb.position.x;
        prevY = rb.position.y;

        state.running = dX > 0.001f || dX < -0.001f;
        state.jumping = dY > 0.001f;
        state.falling = dY < -0.001f;

        anim.SetBool("isRunning", state.running);
        anim.SetBool("isJumping", state.jumping);
        anim.SetBool("isFalling", state.falling);
    }
}
