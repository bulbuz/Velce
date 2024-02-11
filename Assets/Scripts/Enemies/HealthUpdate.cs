using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthUpdate : MonoBehaviour
{
    public int maxHealth;
    int curHealth;
    bool isHurt = false;
    public float freezeDuration;
    float freezeTimer = 0;

    //materials
    [SerializeField]
    Material defaultMat;
    [SerializeField]
    Material dmgMat;
    [SerializeField]
    Material deathMat;

    //variables for handling shaders
    SpriteRenderer rend;
    float dmgTime = 0f;
    public bool dead = false;
    float animDur = 0f;

    // Start is called before the first frame update
    void Start()
    {
        rend = GetComponent<SpriteRenderer>();
        curHealth = maxHealth;
    }

    // Update is called once per frame
    void Update()
    {
        if (isHurt)
        {
            freezeTimer += Time.deltaTime;
            if (freezeTimer >= freezeDuration)
            {
                isHurt = false;
                freezeTimer = 0;
            }
        }
    }
    public bool IsHurt()
    {
        return isHurt;
    }

    public void TakeDamage(int damage)
    {
        // if not dead, remove health and set hurt to true
        if (!dead)
        {
            curHealth -= damage;
            dmgTime = 1f;
            isHurt = true;
            if (curHealth <= 0)
            {
                Die();
            }
        }
    }

    void Die()
    {
        GetComponent<EnemyAI>().state = EnemyAI.State.Dead;
        dead = true;
        dmgTime = 1f;
        animDur = 0f;
        // Destroy(gameObject); don't remove if object got a "dead state"
    }
}