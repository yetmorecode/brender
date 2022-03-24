struct flight {
    br_matrix34 x;	/* Current position */
    br_matrix34 theta;	/* Current orientation */
    br_vector3 p;	/* Linear momentum */
    br_vector3 omega;	/* Angular momentum */
    br_scalar k_p;	/* Linear damping */
    br_scalar k_omega;	/* Angular damping */
    int auto_on;	/* Levelling on? */
    int flags;		/* Which planes are on */
    br_vector3 level[16];	/* Autolevelling plane */
    br_scalar c;	/* Correction rate */
    int mouse_x,mouse_y;
    br_vector3 mouse_lr;
    br_vector3 mouse_ud;
    br_vector3 button_l;
    br_vector3 button_r;
    br_vector3 button_lr;
    int key[16];
    enum { LINEAR, ANGULAR } key_type[16];
    br_vector3 key_impulse[16];
};

#define S0 BR_SCALAR(0.0)
#define S1 BR_SCALAR(1.0)
#define S1024 BR_SCALAR(1024.0)

/*
  FlLinearImpulse() applies an instantaneous impulse along vector (x,y,z)
  in the coordinate frame of the object.
*/

void FlLinearImpulse(struct flight *s,br_scalar x,br_scalar y,br_scalar z);

/*
  FlAngularImpulse() applies an instantaneous angular impulse. The torque
  is applied around the axis defined by (x,y,z) and the magnitude of this
  vector gives the size of the torque.
*/

void FlAngularImpulse(struct flight *s,br_scalar x,br_scalar y,br_scalar z);

/*
  Evolve(s,t) means that the system s is left to evolve for a time t. This
  means that it spins and translates an amount depending on the time and
  the current angular and linear momenta
*/

void FlEvolve(struct flight *s,br_scalar t);

/*
  Apply autolevelling. A corrective impulse is applied.
*/

void FlAutolevel(struct flight *s);

/*
  FlTransformFromState() returns the current position in the 'flight'
  into the matrix m. Simply use this matrix to define the position and
  orientation of the flier
*/

void FlTransformFromState(br_matrix34 *m,struct flight *s);

/*
  Turn on p-th autolevelling plane out of a maximum of 16
*/

void FlPlaneEnable(struct flight *s,int p);

/*
  Disable p-th plane
*/
void FlPlaneDisable(struct flight *s,int p);

/*
  At any moment in time 16 keys can be defined. These are stored in a table of
  16 slots. Each one defines a key, the type of impulse and the size of
  impulse.

  FlKeyConfig() sets key slot n to interpret key 'key' to apply
  impulse defined by (x,y,z). If type==LINEAR [resp. ANGULAR] the impulse
  is linear [resp. angular].
*/
  
void FlKeyConfig(struct flight *s,int n,int key,int type,br_scalar x,br_scalar y,br_scalar z);

/*
  Turn off key slot n
*/

void FlKeyDisable(struct flight *s,int n);

/*
  Sets auto-levelling rate
*/

void FlSetAutolevelRate(struct flight *s,br_scalar c);

/*
  Sets linear drag
*/

void FlSetLinearDrag(struct flight *s,br_scalar k);

/*
  Sets angular drag - ie. rate at which spinning bodies come to rest
*/

void FlSetAngularDrag(struct flight *s,br_scalar k);

/*
  Set current position+orientation in flight from a matrix.
*/

void FlSetPosition(struct flight *s,br_matrix34 *m);

/*
  Initialise a 'flight' object
*/

void FlInit(struct flight *s);

/*
  Set autolevelling plane. When such planes are set impulses are
  automatically applied to drive current orientation to be parallel
  to the nearest plane. (x,y,z) should be unit though useful things
  are possible if it isn't...
*/

void FlSetPlane(struct flight *s,int n,br_scalar x,br_scalar y,br_scalar z);
