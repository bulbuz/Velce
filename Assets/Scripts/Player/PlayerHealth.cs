using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.XR;

public class PlayerHealth : MonoBehaviour
{
    public int maxHealth;
    public int curHealth;
    public GameObject[] hearts;
    
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
    }

    void Die()
    {
        // TODO: reset everything and possibly death screen??
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.layer == LayerMask.NameToLayer("Enemies"))
        {
            curHealth--;
            var animator = hearts[curHealth].GetComponent<Animator>();
            animator.SetTrigger("Hurt");
            if (curHealth == 0)
            {
                Die();
            }
        }
    }
}