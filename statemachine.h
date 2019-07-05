/*
    Copyright 2019, Andrew Lin.

    This source code is released under the 3-Clause BSD license. See 
    LICENSE.txt, or https://opensource.org/licenses/BSD-3-Clause.
 */
#pragma once

namespace statemachine
{
    /**
     * Result codes for State interfaces.
     */
    enum Result
    {
        OK,
        STATE_TRANSITION_FAILED,
        EVENT_NOT_HANDLED
    };

    /**
     * Base class for events that are to be processed by state machine states.
     */
    class Event
    {
    public:
        /**
         * Event constructor.
         *
         * @param id
         * Unique identifier of the event. Usually an enumerated value created
         * in the header file where the events are defined.
         *
         * @param name
         * Human-readable identifier. Useful for debugging. `name` does not get
         * deallocated by the class destructor.
         */
        Event(int id, char const * name);
        virtual ~Event() = default;

        /**
         * Unique event identifier.
         */
        int const m_id;

        /**
         * Human-readable name. Useful for debugging.
         */
        char const * m_name;
    };

    /**
     * Base class for states in the finite state machine.
     */
    class State
    {
    public:
        /**
         * State machine state constructor.
         *
         * @param name
         * `name` is a human-readable state identifier. Useful for debugging.
         * `name` does not get deallocated by the class destructor.
         *
         * @param parent
         * Pass nullptr as the `parent` state for the root state machine state.
         * Otherwise, pass the parent (containing) state as the `parent`
         * argument.
         */
        State(char const * name, State * parent);
        virtual ~State() = default;

        /**
         * Transition to new state.
         *
         * @param state
         * Pointer to state to transition to.
         *
         * @return result code. OK => success.
         */
        virtual Result transition_to_state(State & state);

        /**
         * Transition to state history.
         * Follows the active state pointer in `state` to its last active
         * substate.
         *
         * @param state
         * State whose history to transition to.
         *
         * @return result code. OK => success.
         */
        Result transition_to_history(State & state);

        /*
            Transition to state's deep history.
         */
        /**
         * Transition to state's deep history.
         * Follows the active state pointer in `state` all the way down to the
         * last active sub-sub-(...)substate.
         *
         * @param state
         * State whose deep history to transition to.
         *
         * @return result code. OK => success.
         */
        Result transition_to_deep_history(State & state);

        /**
         * Process event.
         * Call this from the state machine root class instance to process
         * an event.
         *
         * @param event
         * Event to process.
         *
         * @return result code. OK => success.
         */
        Result handle_event(Event & event);

        /*
         * Return the name of the active state.
         */
        /**
         * Get active state name.
         *
         * @return active state name.
         */
        char const * const active_state_name();
        
    protected:
        /**
         * Override this function to provide state initialization
         * functionality (black dot transition).
         *
         * @return result code. OK => success.
         */
        virtual Result on_initialize();

        /**
         * Override this function to provide functionality for whenever your
         * derived state is entered via a `transition_to_state` call.
         *
         * @return result code. OK => success.
         */
        virtual Result on_entry();

        /**
         * Override this function to provide functionality for whenever your
         * derived state is exited via a `transition_to_state` call.
         *
         * @return result code. OK => success.
         */
        virtual Result on_exit();

        /**
         * Overload this function to process specific events. Override this
         * function to determine the event type and call `on_event` with the
         * proper event type. See the example for a sample implementation.
         *
         * Either way, return `true` if the event was processed. Return `false`
         * if it was not, so the parent state can get a shot.
         *
         * Your machine's derived state machine class should override this
         * function and return `true` so that all events are considered handled.
         *
         * @param event
         * Event to process.
         *
         * @return result code. OK => success.
         */
        virtual bool on_event(Event & event);

        /**
         * Get the root (machine) state.
         *
         * @return pointer to root state.
         */
        State * root_state();

        /**
         * Get the current active state.
         *
         * @return pointer to the active state.
         */
        State * active_state();

        /**
         * Find the common parent of `this` state with state `other`.
         *
         * @param other
         * Pointer to other state.
         *
         * @return pointer to common parent, or `nullptr` if there is not common
         * parent.
         */
        State * find_common_parent(State * other);

        /**
         * Human-readable name. Useful for debugging.
         */
        char const * const m_name;

        /**
         * Points to the immediate substate contained within this state that is
         * active. Creates a pointer chain from the root state machine state to
         * the currently active state.
         */
        State * m_active_state;

        /**
         * Points to the state containing this state. * Creates a pointer chain
         * to the root state machine state.
         */
        State * m_parent_state;
    };
}
