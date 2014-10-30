#include "control.h"


/*
  timectrl_constr: construct the timeCtrl object
*/

timeCtrl *timectrl_constr(real tmin, real tmax, real dt)
{
    timeCtrl *tc;

    tc = smalloc( sizeof (timeCtrl), "in control.c: timectrl_constr(), for tc");
    tc -> tmin = tmin;
    tc -> tmax = tmax;
    tc -> dt   = dt;
    tc -> timeLevel = 0;

    return tc;
}

/*
  advance:
  1. move to next time step
  2. save wall time when this time step starts
  return the current time 
*/
real advance(timeCtrl *tc, real dt)
{
    if (tc->t < tc-> tmax){ // no matter if the new time will over tmax
        t_advanced = tc->t + dt;
        if (t_advanced <= tc-> tmax){
            tc->t = t_advanced;
        }
        else{
            tc-> t = tc->tmax;
        }

        tc->timeLevel += 1;
        return tc->t;
    }
    else{
        return NULL;
    }
}    

