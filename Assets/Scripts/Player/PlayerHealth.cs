using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerHealth : MonoBehaviour
{
    public GameObject heartPrefab;
    public int maxHealth;
    public int curHealth;

    public Image[] hearts;
    public Sprite heart;
    public Sprite emptyHeart;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        for (int i = 0; i < hearts.Length; i++)
        {
            hearts[i].sprite = i < curHealth ? heart : emptyHeart;
            hearts[i].enabled = i < curHealth;
        }
    }
    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.layer == LayerMask.NameToLayer("Enemies"))
        {
            curHealth--;
        }
    }
}