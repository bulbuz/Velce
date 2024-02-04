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

    private Dictionary<MovementType, Action> movementModes;
    private PatrolMovement patrol;

    // Start is called before the first frame update
    void Start()
    {
        patrol = GetComponent<PatrolMovement>();
        animator = GetComponent<Animator>();
        movementModes = new Dictionary<MovementType, Action>
        {
            { MovementType.Patrol, patrol.ManualUpdate }
            // to be expanded with more movement types
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

    public void SetState(EnemyAI.EnemyState newState)
    {
        state = newState;
    }

    private void Move()
    {
        if (movementModes.ContainsKey(movementType))
        {
            movementModes[movementType].Invoke();
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