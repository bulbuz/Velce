using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.XR;

public class PlayerHealth : MonoBehaviour
{
    public GameObject heartPrefab;
    public int maxHealth;
    public int curHealth; 
    public GameObject[] hearts;
    private bool isHurt = false;
    private float hurtTime = 0;
    
    enum States
    {
        IDLE,
        POP,
        EMPTY,
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (isHurt)
        {
            hurtTime += Time.deltaTime;
        } 
        else
        {
            hurtTime = 0;
        }
        if (hurtTime >= 0.4)
            isHurt = false;

        for (int i = 0; i <  hearts.Length; i++)
        {
            var animator = hearts[i].GetComponent<Animator>();
            if (i == curHealth && isHurt)
            {
                animator.SetTrigger("popped");
            }
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.layer == LayerMask.NameToLayer("Enemies"))
        {
            curHealth--;
            isHurt = true;
        }
    }
}