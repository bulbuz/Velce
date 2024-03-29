using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

// abbreviations for the stupid but descriptive names -_-
using Ps = PlayerState;
using State = PlayerState.State;

public class PlayerMovement : MonoBehaviour
{
    // Movement vars
    // ------------------------
    public float speed;
    private float horizontal; // value between -1 and 1
    public float jumpingPower;
    public float incrementalJump;
    public bool facingRight = true;
    public float minJumpCutoffVel = 50;
    private bool isJumping = false;

    public float coyoteTime = 0.2f;
    private float coyoteCounter = 0;

    public float jumpBufferTime = 0.2f;
    private float jumpBufferCounter = 0;

    public float acceleration;
    public float deceleration;
    public float velPower; // sensitivity for (de)acceleration
    // ------------------------

    private Rigidbody2D rb;
    public Transform groundCheck;
    public LayerMask groundLayer;
    BoxCollider2D boxCollider;
    public float castDistance = 0.5f;

    //variables for handling shaders
    [SerializeField]
    private Material baseMaterial;
    [SerializeField]
    private Material dmgMaterial;
    SpriteRenderer rend;
   
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        rend = GetComponent<SpriteRenderer>();
        boxCollider = GetComponent<BoxCollider2D>();
    }

    // Update is called once per frame
    void Update()
    {
        // Get raw input and set horizontal velocity
        horizontal = Input.GetAxisRaw("Horizontal");

        // Calculate movement variables
        float targetSpeed = horizontal * speed;
        float deltaSpeed = targetSpeed - rb.velocity.x;
        float curAcc = (Mathf.Abs(targetSpeed) > 0.01f) ? acceleration : deceleration;
        float movement = Mathf.Pow(Mathf.Abs(deltaSpeed) * curAcc, velPower) * Mathf.Sign(deltaSpeed);
        
        // Add movement force
        rb.AddForce(movement * Vector2.right);

        Ps.SetState(Ps.State.LEFT | Ps.State.RIGHT, false);
        if (horizontal != 0)
        {
            if (facingRight)
                Ps.SetState(Ps.State.RIGHT, true);
            else
                Ps.SetState(Ps.State.LEFT, true);
        }
        Ps.SetState(Ps.State.RUNNING, Ps.GetState(Ps.State.RIGHT | Ps.State.LEFT));

        if (IsGrounded())
            coyoteCounter = coyoteTime;
        else
            coyoteCounter -= Time.deltaTime;

        if (Input.GetButtonDown("Jump"))
            jumpBufferCounter = jumpBufferTime;
        else
            jumpBufferCounter -= Time.deltaTime;

        Ps.SetState(Ps.State.JUMP | Ps.State.FALL, false);
        // jump buffer and coyote time for better jump yee
        if (coyoteCounter > 0f && jumpBufferCounter > 0f)
        {
            rb.velocity = new Vector2(rb.velocity.x, jumpingPower);
            jumpBufferCounter = 0;
            isJumping = true;
        }

        // release jump key to fall earlier
        if (rb.velocity.y > 0f && Input.GetButtonUp("Jump"))
        {
            rb.velocity = new Vector2(rb.velocity.x, rb.velocity.y * (1 - incrementalJump));
            coyoteCounter = 0f;
        }
        if (rb.velocity.y < -0.01f)
        {
            Ps.SetState(Ps.State.FALL, true);
            isJumping = false;
        }
        Ps.SetState(Ps.State.JUMP, isJumping);

        // Flip if necessary
        if (horizontal != 0 && (horizontal > 0 != facingRight))
            Flip();
        Ps.SetState(Ps.State.IDLE, !Ps.GetState(Ps.State.JUMP | Ps.State.FALL | Ps.State.ATTACK | Ps.State.HURT | Ps.State.RUNNING));
    }

    private bool IsGrounded()
    {
        float offset = 0.1f;
        
        return Physics2D.Raycast(boxCollider.bounds.center, Vector2.down, boxCollider.bounds.extents.y + offset, groundLayer);
    }

    private void Flip()
    {
        facingRight = !facingRight;
        Vector3 scale = transform.localScale;
        scale.x *= -1;
        transform.localScale = scale;
    }
}