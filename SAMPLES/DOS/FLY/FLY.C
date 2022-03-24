/*
 * Copyright (c) 1993 Argonaut Software Ltd. All rights reserved.
 */

/*
  Damped rocket flight dynamics: handles momentum and angular
  momentum correctly.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include "brender.h"

#include "fly.h"

#define CYCLES 25
#define S0 BR_SCALAR(0.0)
#define S1 BR_SCALAR(1.0)
#define S1024 BR_SCALAR(1024.0)

void FlLinearImpulse(struct flight *s,br_scalar x,br_scalar y,br_scalar z) {
    br_vector3 impulse,impulse_abs;

    BrVector3Set(&impulse,x,y,z);
    BrMatrix34ApplyV(&impulse_abs,&impulse,&s->theta);
    BrVector3Add(&s->p,&s->p,&impulse_abs);
}

void FlAngularImpulse(struct flight *s,br_scalar x,br_scalar y,br_scalar z) {
    br_vector3 impulse,impulse_abs;

    BrVector3Set(&impulse,x,y,z);
    BrMatrix34ApplyV(&impulse_abs,&impulse,&s->theta);
    BrVector3Add(&s->omega,&s->omega,&impulse_abs);
}

/*
  Code to evolve physics
*/

void FlEvolve(struct flight *s,br_scalar t) {
    br_vector3 axis,tmp_omega;
    br_scalar angle;

    /* Apply damping */

    BrVector3Scale(&s->p,&s->p,s->k_p);
    BrVector3Scale(&s->omega,&s->omega,s->k_omega);

    /* Exponentiate up momentum and apply to position */

    BrMatrix34PostTranslate(&s->x,
	BR_MUL(t,s->p.v[0]),BR_MUL(t,s->p.v[1]),BR_MUL(t,s->p.v[2]));

    /* Exponentiate up angular momentum and apply to orientation */

    BrVector3Scale(&tmp_omega,&s->omega,t);
    angle = BrVector3Length(&s->omega);
    if (angle<8)
	return;
    BrVector3Normalise(&axis,&s->omega);

    BrMatrix34PostRotate(&s->theta,BrScalarToAngle(angle),
	&axis);
}

void FlAutolevel(struct flight *s) {
    br_matrix34 inverse;
    br_vector3 plane,closest;
    int i,flag = 0;
    br_scalar highest_ny = -S1024;

    /* Transform levellevelling planes to local coordinates */

    BrMatrix34Inverse(&inverse,&s->theta);

    for (i = 0; i<16; i++) {
	if (s->flags & (1 << i)) {
	    BrMatrix34ApplyV(&plane,&s->level[i],&inverse);
	    if (plane.v[1]>highest_ny) {
		highest_ny = plane.v[1];
		BrVector3Copy(&closest,&plane);
		flag = 1;
	    }
	}
    }

    if (flag)
	FlAngularImpulse(s,
	    BR_MUL(closest.v[2],s->c),S0,-BR_MUL(closest.v[0],s->c));
}

void FlTransformFromState(br_matrix34 *m,struct flight *s) {
   BrMatrix34Mul(m,&s->theta,&s->x);
}

void FlPlaneEnable(struct flight *s,int p) {
    s->flags |= 1 << p;
}

void FlPlaneDisable(struct flight *s,int p) {
    s->flags &= ~(1 << p);
}

void FlSetAutolevelRate(struct flight *s,br_scalar c) {
    s->c = c;
}

void FlSetLinearDrag(struct flight *s,br_scalar k) {
    s->k_p = k;
}

void FlSetAngularDrag(struct flight *s,br_scalar k) {
    s->k_omega = k;
}

void FlSetPosition(struct flight *s,br_matrix34 *m) {

    /* Break up transform into translational and angular parts */

    /* First the linear part...chuck away angular bit */

    BrMatrix34Copy(&s->x,m);
    s->x.m[0][0] = s->x.m[1][1] = s->x.m[2][2] = S1;
    s->x.m[0][1] = s->x.m[0][2] = S0;
    s->x.m[1][0] = s->x.m[1][2] = S0;
    s->x.m[2][0] = s->x.m[2][1] = S0;

    /* Now get angular bit */

    BrMatrix34Copy(&s->theta,m);
    s->theta.m[3][0] = s->theta.m[3][1] = s->theta.m[3][2] = S0;
}

void FlSetPlane(struct flight *s,int n,br_scalar x,br_scalar y,br_scalar z) {
    BrVector3Set(&s->level[n],x,y,z);
}

void FlInit(struct flight *s) {
    br_matrix34 id;

    BrMatrix34Identity(&id);
    FlSetPosition(s,&id);
    BrVector3Set(&s->p,S0,S0,S0);
    BrVector3Set(&s->omega,S0,S0,S0);
    FlSetLinearDrag(s,BR_SCALAR(0.9));
    FlSetAngularDrag(s,BR_SCALAR(0.8));
    s->flags = 0;
    FlSetAutolevelRate(s,BR_SCALAR(0.002));
}
