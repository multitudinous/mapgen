#ifndef GLSTATES_H
#define GLSTATES_H

#include <list>
#include "sysdefs.h"

class GlStates
{
    struct State
    {
        GLenum type;
        GLboolean enabledPrev;
        GLboolean set;
        
        State(GLenum state, GLboolean enabled, GLboolean reset = true)
        {
            type = state;
            enabledPrev = enabled;
            set = reset;
        }
    };

public:
    GlStates();
    virtual ~GlStates();

    void push(GLenum state, GLboolean enable);
    void pop();
    void popAll();

protected: 
    void set(GLenum state, GLboolean enable);

protected:
    std::list<State> _states;
};

#endif