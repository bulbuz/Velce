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

    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<PlayerAnimation>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.J))
            Attack();
    }

    void Attack()
    {
        anim.attackState.PunchingTime = .4f; //runs attack animation for .4s
        Collider2D[] damagedEnemies = Physics2D.OverlapCircleAll(attackBox.position, attackRadius, enemyLayer);

        foreach (Collider2D enemy in damagedEnemies)
        {
            enemy.GetComponent<HealthUpdate>().TakeDamage(attackDamage);
            Debug.Log(enemy.name);
        }
    }
}