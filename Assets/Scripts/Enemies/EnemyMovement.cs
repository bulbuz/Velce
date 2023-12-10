using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMovement : MonoBehaviour
{
    public float speed;
    public bool facingRight = true;

    private Rigidbody2D rb;
    public Transform wallCheck;
    public LayerMask wallLayer;
    private HealthUpdate healthUpdate;
    public Transform groundCheck;
    private BoxCollider2D boxCollider;
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        healthUpdate = GetComponent<HealthUpdate>();
        boxCollider = GetComponent<BoxCollider2D>();

        if (!facingRight)
            speed *= -1;
    }

    // Update is called once per frame
    void Update()
    {
        bool hurt = healthUpdate.IsHurt();
        
        // don't update and wait if hit
        if (hurt)
            return;

        rb.velocity = new Vector2(speed, rb.velocity.y);

        // turn around if hit wall or no more ground
        if (HitWall() || !GroundAhead())
        {
            Flip();
            speed *= -1;
        }
    }

    // checks whether the there is ground ahead of the entity
    bool GroundAhead()
    {
        float offset = 0.1f;
        return Physics2D.Raycast(groundCheck.position, Vector2.down, boxCollider.bounds.extents.y + offset, wallLayer);
    }

    // uses the built in overlap for collision against wall mask
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