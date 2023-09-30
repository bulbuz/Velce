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
    public float incrementalJump;
    private bool facingRight = true;

    public float acceleration;
    public float deceleration;
    public float velPower;

    private Rigidbody2D rb;
    public Transform groundCheck;
    public LayerMask groundLayer;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        // Get raw input and set horizontal velocity
        horizontal = Input.GetAxisRaw("Horizontal");

        float targetSpeed = horizontal * speed;
        float deltaSpeed = targetSpeed - rb.velocity.x;
        float curAcc = (Mathf.Abs(targetSpeed) > 0.01f) ? acceleration : deceleration;
        float movement = Mathf.Pow(Mathf.Abs(deltaSpeed) * curAcc, velPower) * Mathf.Sign(deltaSpeed);
        rb.AddForce(movement * Vector2.right);

        if (Input.GetButtonDown("Jump"))
        {
            if (IsGrounded())
            {
                rb.AddForce(Vector2.up * jumpingPower, ForceMode2D.Impulse);
            }
        }
       
        if (rb.velocity.y > 0 && !Input.GetButton("Jump"))
        {
            rb.AddForce(Vector2.down * rb.velocity.y * (1 - incrementalJump), ForceMode2D.Impulse);
        }

        if (horizontal != 0 && (horizontal > 0 != facingRight))
        {
            Flip();
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        // Check for collision with enemy
        if (collision.gameObject.CompareTag("Enemy"))
        {

        }
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
}