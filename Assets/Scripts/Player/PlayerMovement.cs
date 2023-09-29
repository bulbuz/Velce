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
        rb.velocity = new Vector2(horizontal * speed, rb.velocity.y);

        if (Input.GetButtonDown("Jump"))
        {
            if (IsGrounded())
            {
                rb.AddForce(new Vector2(rb.velocity.x, jumpingPower));
            }
        }

        if ((rb.velocity.x < 0 && facingRight) || (rb.velocity.x > 0 && !facingRight)) 
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