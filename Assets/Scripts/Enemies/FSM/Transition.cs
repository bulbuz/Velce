using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace FSM
{
    [CreateAssetMenu(menuName = "FSM/Transition")]
    public class Transition : ScriptableObject
    {
        public Decision decision;

        // transition states
        public BaseState trueState;
        public BaseState falseState;
        
        public void Execute(BaseStateMachine machine)
        {
            if (decision.Decide(machine) && !(trueState is RemainInState))
            {
                machine.CurrentState.Exit(machine);
                machine.CurrentState = trueState;
                machine.CurrentState.Enter(machine);
            }
            else if (!(falseState is RemainInState))
            {
                machine.CurrentState.Exit(machine);
                machine.CurrentState = falseState;
                machine.CurrentState.Enter(machine);
            }
        }
    }
}