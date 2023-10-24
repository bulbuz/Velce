using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TardigradeMovement : MonoBehaviour
{
    public float speed;
    private bool facingRight = false;

    private Rigidbody2D rb;
    public Transform wallCheck;
    public LayerMask wallLayer;
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        bool hurt = GetComponent<HealthUpdate>().IsHurt();
        if (!hurt)
        {
            rb.velocity = new Vector2(speed, rb.velocity.y);
            if (HitWall())
            {
                Flip();
                speed *= -1;
            }
        }
    }

    bool HitWall()
    {
        return Physics2D.OverlapCircle(wallCheck.position, 0.2f, wallLayer);
    }

    private void Flip()
    {
        facingRight = !facingRight;
        Vector3 scale = transform.localScale;
        scale.x *= -1;
        transform.localScale = scale;
    }
}