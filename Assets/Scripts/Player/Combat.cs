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
    public float attackDuration = 5 / 12; // 5 frames divided by 12 fps
    public int attackDamage;

    public Vector2 enemyKnockback;

    private float attackTimer = 9999f;
    private bool doAttack = false;

    void Update()
    {
        // check if attack button is pressed and player is not currently attacking
        // -0.1f is used to add 100ms of leway so the user can press attack 100ms before its finished
        if (attackTimer >= attackDuration - 0.1f)
        {
            if (Input.GetKeyDown(KeyCode.J))
            {
                doAttack = true;
            }
        }

        // handle attack
        if (attackTimer >= attackDuration && doAttack)
        {
            attackTimer = 0;
            Attack();
            doAttack = false;
        }

        attackTimer += Time.deltaTime;
        
        // check if attack has finished
        if (attackTimer >= attackDuration)
        {
            PlayerState.SetState(PlayerState.State.ATTACK, false);
        }
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
        PlayerState.SetState(PlayerState.State.ATTACK, true);

        // get enemies the player has hit
        Collider2D[] damagedEnemies = Physics2D.OverlapCircleAll(attackBox.position, attackRadius, enemyLayer);

        foreach (Collider2D enemy in damagedEnemies)
        {
            // take damage
            enemy.GetComponent<HealthUpdate>().TakeDamage(attackDamage);


            // calculate direction of knockback
            bool facingRight = gameObject.GetComponent<PlayerMovement>().facingRight;
            if (facingRight)
                enemyKnockback.x = Mathf.Abs(enemyKnockback.x);
            else
                enemyKnockback.x = Mathf.Abs(enemyKnockback.x) * -1;
            if (enemyKnockback.x < 0 == facingRight)
            {
                enemyKnockback.x *= -1;
            }

            enemy.GetComponent<Rigidbody2D>().AddForce(enemyKnockback);
        }
    }
}