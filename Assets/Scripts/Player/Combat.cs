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

    public Vector2 tardigradeKnockback;

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
            Debug.Log("helu");
            //Hurt(collision.gameObject);
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
                tardigradeKnockback.x = Mathf.Abs(tardigradeKnockback.x);
            else
                tardigradeKnockback.x = Mathf.Abs(tardigradeKnockback.x) * -1;
            if (tardigradeKnockback.x < 0 == facingRight)
            {
                tardigradeKnockback.x *= -1;
            }
            Debug.Log(tardigradeKnockback.x);

            enemy.GetComponent<Rigidbody2D>().AddForce(tardigradeKnockback);
            Debug.Log(enemy.name);
        }
    }
}