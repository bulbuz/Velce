using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;

public class EnemyAI : MonoBehaviour
{
    public bool removeAtDeath = true;
    public enum EnemyState
    {
        Attacking,
        Idle,
        Move,
        Dead,
    }

    public enum MovementType
    {
        Patrol,
        Fly,
    }

    public MovementType movementType;
    public EnemyState state = EnemyState.Move;
    private Animator animator;

    private Dictionary<MovementType, Action> movementTypes;
    private PatrolMovement patrol;

    // Start is called before the first frame update
    void Start()
    {
        patrol = GetComponent<PatrolMovement>();
        animator = GetComponent<Animator>();
        movementTypes = new Dictionary<MovementType, Action>
        {
            { MovementType.Patrol, patrol.ManualUpdate }
            // to be expanded
        };
    }

    // Update is called once per frame
    void Update()
    {
        switch (state)
        {
            case EnemyState.Dead:
                Die();
                break;

            case EnemyState.Move:
                Move();
                break;
            case EnemyState.Attacking:
                Attack();
                break;

            default:
                break;
        }
    }

    private void Move()
    {
        if (movementTypes.ContainsKey(movementType))
        {
            movementTypes[movementType].Invoke();
        }
        else
        {
            Debug.LogError("Couldn't find movement type!");
        }
    }
    
    private void Attack()
    {

    }

    private void Die()
    {
        if (!removeAtDeath)
        {
            animator.SetBool("IsDead", true);
            Destroy(GetComponent<Rigidbody2D>());
            Destroy(GetComponent<BoxCollider2D>());
        }
        else
        {
            Destroy(gameObject);
        }
    }
}