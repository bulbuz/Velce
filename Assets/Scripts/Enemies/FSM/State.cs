using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace FSM
{
    [CreateAssetMenu(menuName = "FSM/State")]
    public class State : BaseState
    {
        public List<Action> actions = new List<Action>();
        public List<Transition> transitions = new List<Transition>();

        public override void Enter(BaseStateMachine machine)
        {
            foreach (var action in actions) 
                action.Enter(machine);
        }

        public override void Exit(BaseStateMachine machine)
        {
            foreach (var action in actions)
                action.Exit(machine);
        }

        public override void Execute(BaseStateMachine machine)
        {
            foreach (var action in actions)
                action.Execute(machine);

            foreach (var transition in transitions)
                transition.Execute(machine);
        }
    }
}

