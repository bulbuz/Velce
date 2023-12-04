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

    public float coyoteTime = 0.2f;
    private float coyoteCounter = 0;

    public float jumpBufferTime = 0.2f;
    private float jumpBufferCounter = 0;

    public float acceleration;
    public float deceleration;
    public float velPower; // how quickly it de/accelerates
    // ------------------------

    private Rigidbody2D rb;
    public Transform groundCheck;
    public LayerMask groundLayer;

    private PlayerAnimation anim;

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
        anim = GetComponent<PlayerAnimation>();
        rend = GetComponent<SpriteRenderer>();
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

        Ps.SetState(State.LEFT | State.RIGHT, false);
        if (horizontal != 0)
        {
            if (facingRight)
                Ps.SetState(State.RIGHT, true);
            else 
                Ps.SetState(State.LEFT, true);
        }
        Ps.SetState(State.RUNNING, Ps.GetState(State.RIGHT | State.LEFT));

        if (IsGrounded())
            coyoteCounter = coyoteTime;
        else
            coyoteCounter -= Time.deltaTime;

        if (Input.GetButtonDown("Jump"))
            jumpBufferCounter = jumpBufferTime;
        else
            jumpBufferCounter -= Time.deltaTime;

        Ps.SetState(State.JUMP | State.FALL, false);
        // jump buffer and coyote time for better jump yee
        if (coyoteCounter > 0f && jumpBufferCounter > 0f)
        {
            rb.velocity = new Vector2(rb.velocity.x, jumpingPower);
            jumpBufferCounter = 0;
            Ps.SetState(State.JUMP, true);
        }

        // release jump key to fall earlier
        if (rb.velocity.y > 0f && Input.GetButtonUp("Jump"))
        {
            rb.velocity = new Vector2(rb.velocity.x, rb.velocity.y * (1 - incrementalJump));
            coyoteCounter = 0f;
        }
        if (rb.velocity.y < -0.01f)
            Ps.SetState(State.FALL, true);

        // Flip if necessary
        if (horizontal != 0 && (horizontal > 0 != facingRight))
            Flip();
        Ps.SetState(State.IDLE, !Ps.GetState(State.JUMP | State.FALL | State.ATTACK | State.HURT | State.RUNNING));
    }

    //did this for debuging and testing only, feel free to delete this code:
    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.CompareTag("Enemy"))
        {
            rend.material = dmgMaterial;
        }
        else
        {
            rend.material = baseMaterial;
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