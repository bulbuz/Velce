using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Combat : MonoBehaviour
{
    public Animator animator;
    public Transform attackBox;
    public float attackRadius = 0.5f;
    public LayerMask enemyLayer;

    private PlayerAnimation anim;
    public int attackDamage;

    public Vector2 enemyKnockback;

    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<PlayerAnimation>();
    }

    // Update is called once per frame
    void Update()
    {
        // Attack if not in hurt
        if (Input.GetKeyDown(KeyCode.J))
            Attack();
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.layer == enemyLayer)
        {
            int damage = collision.gameObject.GetComponent<EnemyAttack>().attackDamage;
            GetComponent<HealthUpdate>().TakeDamage(damage);
        }
    }

    private void Hurt(GameObject enemy)
    {
        int enemyAttackDamage = enemy.GetComponent<EnemyAttack>().attackDamage;
        gameObject.GetComponent<HealthUpdate>().TakeDamage(enemyAttackDamage);
    }

    void Attack()
    {
        anim.attackState.PunchingTime = .4f; //runs attack animation for .4s
        Collider2D[] damagedEnemies = Physics2D.OverlapCircleAll(attackBox.position, attackRadius, enemyLayer);

        foreach (Collider2D enemy in damagedEnemies)
        {
            enemy.GetComponent<HealthUpdate>().TakeDamage(attackDamage);

            bool facingRight = gameObject.GetComponent<PlayerMovement>().facingRight;
            if (facingRight)
                enemyKnockback.x = Mathf.Abs(enemyKnockback.x);
            else
                enemyKnockback.x = Mathf.Abs(enemyKnockback.x) * -1;
            if (enemyKnockback.x < 0 == facingRight)
            {
                enemyKnockback.x *= -1;
            }
            Debug.Log(enemyKnockback.x);

            enemy.GetComponent<Rigidbody2D>().AddForce(enemyKnockback);
        }
    }
}