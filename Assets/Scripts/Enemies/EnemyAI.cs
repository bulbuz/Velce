using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;


/*
 * ENEMY AI
 * 
 * At the core, the enemy AI is a finite state machine (FSM) on steroids.
 * 
 * There are a few basic states from which all enemies are assembled (see state enum)
 * The enemies are created by the model "Pick and Mix", i.e. for each state, choose the enemy behaviour from a
 * predetermined set of patterns (usually scripts) making this script essentially the "brain"
 * 
 */


public class EnemyAI : MonoBehaviour
{
    public bool removeAtDeath = true;
    public enum State
    {
        Attacking,
        Hurt, // All enemies should have a "hurt" cooldown
        Move,
        Dead,
    }

    public enum MovementMode
    {
        Patrol,
        Fly,
    }

    public MovementMode movementType;
    public State state = State.Move;
    private Animator animator;

    private Dictionary<MovementMode, Action> movementModes;
    private PatrolMovement patrol;

    // Start is called before the first frame update
    void Start()
    {
        patrol = GetComponent<PatrolMovement>();
        animator = GetComponent<Animator>();
        movementModes = new Dictionary<MovementMode, Action>
        {
            { MovementMode.Patrol, patrol.ManualUpdate }
            // to be expanded with more movement types
        };
    }

    // Update is called once per frame
    void Update()
    {
        switch (state)
        {
            case State.Dead:
                Die();
                break;

            case State.Move:
                Move();
                break;
            case State.Attacking:
                Attack();
                break;

            default:
                break;
        }
    }

    public void SetState(EnemyAI.State newState)
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

    private void OnCollisionEnter2D(Collision2D collision)
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