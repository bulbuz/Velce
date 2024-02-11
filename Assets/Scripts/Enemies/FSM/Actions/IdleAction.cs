using System;
using UnityEngine;

namespace FSM.Actions
{
    [CreateAssetMenu(menuName = "FSM/IdleAction")]
    public class IdleAction : Action
    {
        public override void Enter(BaseStateMachine machine)
        {
            machine.GetComponent<Rigidbody2D>().velocity = Vector2.zero;
            machine.GetComponent<Animator>().SetBool("isWalking", false);
        }

        public override void Execute(BaseStateMachine machine)
        {

        }

        public override void Exit(BaseStateMachine machine)
        {

        }
    }
}
