using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAnimation : MonoBehaviour
{
    Animator anim;
    Rigidbody2D rb;

    bool isRunning = false;
    bool isJumping = false;
    bool isFalling = false;

    float dX, dY = 0;
    float prevX, prevY;

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

        isRunning = dX > 0.001f || dX < -0.001f;
        isJumping = dY > 0.001f;
        isFalling = dY < -0.001f;

        anim.SetBool("isRunning", isRunning);
        anim.SetBool("isJumping", isJumping);
        anim.SetBool("isFalling", isFalling);
    }
}
