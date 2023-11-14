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
    bool dead = false;
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
            if (freezeTimer >= freezeDuration) {
                isHurt = false;
                freezeTimer = 0;
            }
            dmgTime -= Time.deltaTime;
            animDur += Time.deltaTime;

            if (dmgTime > 0f)
            {
                rend.material = dead ? deathMat : dmgMat;
            }
            else
            {
                rend.material = defaultMat;
                if (dead)
                {
                    Destroy(gameObject);
                }
            }
            rend.material.SetFloat("_AnimDur", animDur);
            //Debug.Log(rend.material.GetFloat("_AnimDur"));
        }
    }
    public bool IsHurt()
    {
        return isHurt;
    }

    public void TakeDamage(int damage)
    {
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
        dead = true;
        dmgTime = 1f;
        animDur = 0f;
    }
}
