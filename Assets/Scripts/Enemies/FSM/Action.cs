using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace FSM
{
    public abstract class Action : ScriptableObject
    {
        public virtual void Enter(BaseStateMachine machine) { }
        public virtual void Execute(BaseStateMachine machine) { }
        public virtual void Exit(BaseStateMachine machine) { }
    }
}