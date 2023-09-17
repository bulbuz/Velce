using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    // Movement
    public float speed;
    private float horizontal; // value between -1 and 1
    public float jumpingPower;

    private bool facingRight = true;

    private Rigidbody2D rb;
    public Transform groundCheck;
    public LayerMask groundLayer;
    

    //Vilgots animation-variables :)
    private Animator animator;
    private bool isRunning = false;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        horizontal = Input.GetAxisRaw("Horizontal");
        rb.velocity = new Vector2(horizontal * speed, rb.velocity.y);

        isRunning = (horizontal != 0);

        if (Input.GetButtonDown("Jump"))
        {
            if (IsGrounded())
            {
                rb.AddForce(new Vector2(rb.velocity.x, jumpingPower));
            } else if (rb.velocity.y > 0)
            {
                rb.velocity = new Vector2(rb.velocity.x, rb.velocity.y * 0.5f);
            }
        }

        if ((rb.velocity.x < 0 && facingRight) || (rb.velocity.x > 0 && !facingRight)) 
        {
            Flip();
        }

        UpdateAnimations();
    }

    private bool IsGrounded()
    {
        return Physics2D.OverlapCircle(groundCheck.position, 0.2f, groundLayer);
    }

    private void Flip()
    {
        facingRight = !facingRight;
        Vector3 scale = transform.localScale;
        scale.x *= -1;
        transform.localScale = scale;
    }

    private void UpdateAnimations()
    {
        animator.SetBool("isRunning", isRunning);
    }
}