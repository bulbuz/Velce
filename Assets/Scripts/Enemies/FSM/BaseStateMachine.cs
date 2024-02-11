using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace FSM
{
    public class BaseStateMachine : MonoBehaviour
    {

        [SerializeField] private BaseState initState;
        public BaseState CurrentState { get; set; }

        private void Awake()
        {
            CurrentState = initState;
        }

        // Start is called before the first frame update
        void Start()
        {
            CurrentState.Enter(this);
        }

        // Update is called once per frame
        void Update()
        {
            CurrentState.Execute(this);
        }
    }
}

