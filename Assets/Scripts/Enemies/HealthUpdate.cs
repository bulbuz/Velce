using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthUpdate : MonoBehaviour
{
    public int maxHealth;
    int curHealth;
    bool isHurt = false;
    public float hurtDuration;
    float hurtTimer = 0;

    // Start is called before the first frame update
    void Start()
    {
        curHealth = maxHealth;
    }

    // Update is called once per frame
    void Update()
    {
        if (isHurt)
        {
            hurtTimer += Time.deltaTime;
            if (hurtTimer >= hurtDuration) {
                isHurt = false;
                hurtTimer = 0;
            }
        }
    }
    public bool IsHurt()
    {
        return isHurt;
    }

    public void TakeDamage(int damage)
    {
        isHurt = true;
        curHealth -= damage;
        if (curHealth <= 0)
        {
            Die();
        }
    }

    void Die()
    {
        Object.Destroy(gameObject);
    }
}
