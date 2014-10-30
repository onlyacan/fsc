#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "type.h"

typedef struct 
{
    real tmin;
    real tmax;
    real t;
    real dt;
    id timeLevel;

}timeCtrl;


typedef struct
{

    int max_it;
    double tol;
    double iter;
    double error;
    int flag;

} innerIterCtrl;


timeCtrl *timectrl_constr(real tmin, real tmax, real dt); // construct 
real advance(timeCtrl *tc, real dt);  // advance the time to next time step



#endif /*__CONTROL_H__*/
