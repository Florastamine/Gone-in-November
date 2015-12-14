/////////////////////////////////////////////////////////////
// Physics plugin using NVIDIA PhysX
// (c) 2009 Christian Kahler / oP group Germany              
/////////////////////////////////////////////////////////////
#ifndef ACKPHYSX_H
	#define ACKPHYSX_H
	
	#define PRAGMA_PRINT "\nPhysics module initialization..."

	#define PRAGMA_BIND "ackphysX.dll"
	#define PRAGMA_BIND "PhysXCore.dll"
	#define PRAGMA_BIND "PhysXDevice.dll"
	#define PRAGMA_BIND "PhysXLoader.dll"
	#define PRAGMA_BIND "PhysXCooking.dll"
	#define PRAGMA_BIND "NxCharacter.dll"
	// #define PRAGMA_BIND "cudart32_30_9.dll" // <-- This file doesn't exist, fixed it in the build script.

	#define PH_RIGID		1		// Constant for setting up physics body
	#define PH_STATIC		3		// Constant for setting up static body
	#define PH_CHAR		4		// Creates a character

	#define PH_BOX			0
	#define PH_SPHERE		1
	#define PH_CAPSULE	2
	#define PH_POLY		3
	#define PH_CONVEX		4
	#define PH_TERRAIN	5
	#define PH_PLANE		6
	#define PH_MODIFIED	8

	#define PH_BALL		1
	#define PH_HINGE		2
	#define PH_SLIDER		3
	#define PH_WHEEL		4
	#define PH_ROPE		5
	#define PH_6DJOINT	6
	#define PH_PRISMA		7

	//Contact calls:
	#define	NX_IGNORE_PAIR					(1<<0)	// Disable contact generation for this pair.
	#define	NX_NOTIFY_ON_START_TOUCH	(1<<1)	// Pair callback will be called when the pair starts to be in contact.
	#define	NX_NOTIFY_ON_END_TOUCH		(1<<2)	// Pair callback will be called when the pair stops to be in contact.
	#define	NX_NOTIFY_ON_TOUCH			(1<<3)	// Pair callback will keep getting called while the pair is in contact.

	//6D Joint motions and drives:
	#define	NX_D6JOINT_MOTION_LOCKED	0	// The DOF is locked, it does not allow relative motion.
	#define	NX_D6JOINT_MOTION_LIMITED	1 	// The DOF is limited, it only allows motion within a specific range.
	#define	NX_D6JOINT_MOTION_FREE		2	// The DOF is free and has its full range of motions.
	#define	NX_D6JOINT_DRIVE_POSITION	1	// Used to set a position goal when driving.
	#define	NX_D6JOINT_DRIVE_VELOCITY	2	// Used to set a velocity goal when driving.

	//Force Modes:
	#define	NX_FORCE 						0  // parameter has unit of mass * distance/ time^2, i.e. a force.
	#define	NX_IMPULSE          			1 	// parameter has unit of mass * distance /time (DEFAULT).
	#define	NX_VELOCITY_CHANGE			2	// parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
	#define	NX_SMOOTH_IMPULSE   			3	// same as NX_IMPULSE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
	#define	NX_SMOOTH_VELOCITY_CHANGE	4	// same as NX_VELOCITY_CHANGE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
	#define	NX_ACCELERATION				5	// parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.

	//Shapes Types (needed for Raycast)
	#define	NX_STATIC_SHAPES			(1<<0)										// Hits static shapes.
	#define	NX_DYNAMIC_SHAPES			(1<<1)										// Hits dynamic shapes.
	#define	NX_ALL_SHAPES				NX_STATIC_SHAPES|NX_DYNAMIC_SHAPES	// Hits both static & dynamic shapes.

	//Raycast Types:
	#define	NX_RAYCAST_ENTITY				(1<<0)	// if set, the function returns the closest Entity, that was hit by the ray.
	#define	NX_RAYCAST_IMPACT				(1<<1)	// if set, the function returns the impact vector in world coords.
	#define	NX_RAYCAST_NORMAL				(1<<2)	// if set, the function returns the normal vector in world coords.
	#define	NX_RAYCAST_FACE_INDEX		(1<<3)	// if set, the function returns the faceID , internalFaceID and 0 in a vector.
	#define	NX_RAYCAST_DISTANCE			(1<<4)	// if set, the function returns the distance between the hit shape and the entity position.
	#define	NX_RAYCAST_UV					(1<<5)	// if set, the function returns the U and V coords. And 0 in a vector.
	#define	NX_RAYCAST_FACE_NORMAL		(1<<6)	// if set, the function returns the normal vector in world coords.
	#define	NX_RAYCAST_MATERIAL			(1<<7)	// if set, the function returns the materail index of the shape.

	//Body Flags: 
	//Collection of flags describing the behavior of a dynamic rigid body.
	#define	NX_BF_DISABLE_GRAVITY	 (1<<0) // disables the gravity for the actor
	#define	NX_BF_FROZEN_POS_X		 (1<<1) // Enable/disable freezing for this body/actor.
	#define	NX_BF_FROZEN_POS_Z		 (1<<2) 
	#define	NX_BF_FROZEN_POS_Y		 (1<<3)
	#define	NX_BF_FROZEN_ROLL			 (1<<4)
	#define	NX_BF_FROZEN_PAN			 (1<<5)
	#define	NX_BF_FROZEN_TILT			 (1<<6)
	#define	NX_BF_FROZEN_POS			 NX_BF_FROZEN_POS_X|NX_BF_FROZEN_POS_Y|NX_BF_FROZEN_POS_Z
	#define	NX_BF_FROZEN_ROT			 NX_BF_FROZEN_PAN|NX_BF_FROZEN_TILT|NX_BF_FROZEN_ROLL
	#define	NX_BF_FROZEN				 NX_BF_FROZEN_POS|NX_BF_FROZEN_ROT
	#define	NX_BF_KINEMATIC			 (1<<7)	// Enables kinematic mode for the actor
	#define	NX_BF_VISUALIZATION		 (1<<8) 	// Enable debug renderer for this body (not supported yet)
	#define	NX_BF_DUMMY_0				 (1<<9)  // deprecated flag placeholder
	#define	NX_BF_FILTER_SLEEP_VEL	 (1<<10) // Filter velocities used keep body awake. The filter reduces rapid oscillations and transient spikes
	#define	NX_BF_ENERGY_SLEEP_TEST	 (1<<11) // Enables energy-based sleeping algorithm

	//Material Flags:
	#define	NX_MF_ANISOTROPIC 				(1<<0) // activates the direction of anisotropy in the material class.
	#define	NX_MF_DISABLE_FRICTION 			(1<<4) // If this flag is set, friction computations are always skipped between shapes with this material and any other shape.
	#define	NX_MF_DISABLE_STRONG_FRICTION (1<<5) // the strong friction feature remembers the "friction error" between simulation steps.

	//Actor Flags:
	#define	NX_AF_DISABLE_COLLISION			 	(1<<0) // Enable/disable collision detection
	#define	NX_AF_DISABLE_RESPONSE			 	(1<<1) // Enable/disable collision response (reports contacts but don't use them)
	#define	NX_AF_LOCK_COM							(1<<2) // Disables COM update when computing inertial properties at creation time.
	#define	NX_AF_FLUID_DISABLE_COLLISION		(1<<3) // Enable/disable collision with fluid. (pro version)
	#define	NX_AF_CONTACT_MODIFICATION			(1<<4) // Turn on contact modification callback for the actor.
	#define	NX_AF_FORCE_CONE_FRICTION			(1<<5) // Force cone friction to be used for this actor.		
	#define	NX_AF_USER_ACTOR_PAIR_FILTERING	(1<<6) // Enable/disable custom contact filtering. 

	//Shape Flags:
	#define	NX_TRIGGER_ON_ENTER			 		(1<<0)
	#define	NX_TRIGGER_ON_LEAVE			 		(1<<1)
	#define	NX_TRIGGER_ON_STAY			 		(1<<2)
	#define	NX_TRIGGER_ENABLE				 		NX_TRIGGER_ON_ENTER|NX_TRIGGER_ON_LEAVE|NX_TRIGGER_ON_STAY
	#define	NX_SF_VISUALIZATION					(1<<3) 	// Enable debug renderer for this shape
	#define	NX_SF_DISABLE_COLLISION				(1<<4) 	// Disable collision detection for this shape (counterpart of NX_AF_DISABLE_COLLISION)
	#define	NX_SF_FEATURE_INDICES				(1<<5)	// Enable feature indices in contact stream.
	#define	NX_SF_DISABLE_RAYCASTING			(1<<6) 	// Disable raycasting for this shape
	#define	NX_SF_POINT_CONTACT_FORCE			(1<<7) 	// Enable contact force reporting per contact point in contact stream (otherwise we only report force per actor pair)
	#define	NX_SF_FLUID_DRAIN						(1<<8)	// Sets the shape to be a fluid drain.
	#define	NX_SF_FLUID_DISABLE_COLLISION		(1<<10)	// Disable collision with fluids.
	#define	NX_SF_FLUID_TWOWAY					(1<<11)	// Enables the reaction of the shapes actor on fluid collision.
	#define	NX_SF_DISABLE_RESPONSE				(1<<12)  // Disable collision response for this shape (counterpart of NX_AF_DISABLE_RESPONSE)
	#define	NX_SF_DYNAMIC_DYNAMIC_CCD			(1<<13)  // Enable dynamic-dynamic CCD for this shape. Used only when CCD is globally enabled and shape have a CCD skeleton.
	#define	NX_SF_DISABLE_SCENE_QUERIES		(1<<14)  // Disable participation in ray casts, overlap tests and sweeps.
	#define	NX_SF_CLOTH_DRAIN						(1<<15)	// Sets the shape to be a cloth drain.
	#define	NX_SF_CLOTH_DISABLE_COLLISION		(1<<16)	// Disable collision with cloths.
	#define	NX_SF_CLOTH_TWOWAY					(1<<17)	// Enables the reaction of the shapes actor on cloth collision.
	#define	NX_SF_SOFTBODY_DRAIN					(1<<18)	// Sets the shape to be a soft body drain.
	#define	NX_SF_SOFTBODY_DISABLE_COLLISION	(1<<19)	// Disable collision with soft bodies.
	#define	NX_SF_SOFTBODY_TWOWAY				(1<<20)  // Enables the reaction of the shapes actor on soft body collision.

	//Cloth Flags:
	#define	NX_CLF_PRESSURE               	 (1<<0) // Enable/disable pressure simulation. Note: Pressure simulation only produces meaningful results for closed meshes.
	#define	NX_CLF_STATIC                 	 (1<<1) // Makes the cloth static.
	#define	NX_CLF_DISABLE_COLLISION      	 (1<<2) // Disable collision handling with the rigid body scene.
	#define	NX_CLF_SELFCOLLISION          	 (1<<3) // Enable/disable self-collision handling within a single piece of cloth.
	#define	NX_CLF_VISUALIZATION          	 (1<<4) // Enable/disable debug visualization.
	#define	NX_CLF_GRAVITY            			 (1<<5) // Enable/disable gravity. If off, the cloth is not subject to the gravitational force
	#define	NX_CLF_BENDING            			 (1<<6) // Enable/disable bending resistance. Select the bending resistance through bendingStiffness.
	#define	NX_CLF_BENDING_ORTHO      			 (1<<7) // Enable/disable orthogonal bending resistance. This flag has an effect only if NX_CLF_BENDING is set.
	#define	NX_CLF_DAMPING            			 (1<<8) // Enable/disable damping of internal velocities. Use NxClothDesc.dampingCoefficient to control damping.
	#define	NX_CLF_COLLISION_TWOWAY   		 	 (1<<9) // Enable/disable two way collision of cloth with the rigid body scene.
	#define	NX_CLF_TRIANGLE_COLLISION 			(1<<11) // Not supported in current release. Enable/disable collision detection of cloth triangles against the scene.
	#define	NX_CLF_TEARABLE           			(1<<12) // Defines whether the cloth is tearable. 
	#define	NX_CLF_HARDWARE           			(1<<13) // Defines whether this cloth is simulated on the GPU.
	#define	NX_CLF_COMDAMPING		  				(1<<14) // Enable/disable center of mass damping of internal velocities.
	#define	NX_CLF_VALIDBOUNDS		  			(1<<15) // If the flag NX_CLF_VALIDBOUNDS is set, cloth particles outside the volume defined by validBounds are automatically removed from the simulation.
	//#define	NX_CLF_FLUID_COLLISION    			(1<<16) // Enable/disable collision handling between this cloth and fluids. Not supported yet
	#define	NX_CLF_DISABLE_DYNAMIC_CCD 		(1<<17) // Disable continuous collision detection with dynamic actors. Dynamic actors are handled as static ones.
	#define	NX_CLF_ADHERE							(1<<18) // Moves cloth partially in the frame of the attached actor.
	#define	NX_CLF_HARD_STRETCH_LIMITATION	(1<<20) // Uses the information provided by NxCloth.setConstrainPositions() and NxCloth.setConstrainCoefficients() to make the cloth less stretchy.
	//#define	NX_CLF_UNTANGLING             	(1<<21) // This feature is experimental, use with caution! When this NX_CLF_UNTANGLING is set, the simulator tries to untangle the cloth locally.
	//#define	NX_CLF_INTER_COLLISION        	(1<<22) // Allows cloth to collide with soft bodies and with other cloth. Not supported yet
	
	// The attachmentFlags parameter specifies how the cloth interacts with the shape. The two possible values are shown below:
	// The default is only object->cloth interaction (one way).

	#define  NX_CLOTH_ATTACHMENT_TWOWAY			(1<<0) // With this flag set, cloth->object interaction is turned on as well (only for dynamic shapes).
	#define  NX_CLOTH_ATTACHMENT_TEARABLE		(1<<1) // When this flag is set, the attachment is tearable.
	#define  NX_CLOTH_ATTACHMENT_BOTH			(1<<2) // It combines the two flags NX_CLOTH_ATTACHMENT_TWOWAY|NX_CLOTH_ATTACHMENT_TEARABLE (only for dynamic shapes).

	//TimeStep Methods:
	#define	NX_TIMESTEP_FIXED				0			// The simulation automatically subdivides the passed elapsed time into maxTimeStep-sized substeps.
	#define	NX_TIMESTEP_VARIABLE			1			// The simulation uses the elapsed time that the user passes as-is, substeps (maxTimeStep, maxIter) are not used.
	#define	NX_TIMESTEP_INHERIT			2			// Inherit timing settings from primary scene.  Only valid for compartments.
	#define	NX_NUM_TIMESTEP_METHODS		3

	////////////////////////////////////////////////////////////////////////////////////
	// compatibility redefines
	#ifdef PH_COMPAT
		#define ph_setgravity(v)						pX_setgravity(v)
		#define phent_settype(ent,type,hull)		pXent_settype(ent,type,hull) 
		#define phent_enable(ent,fl)					pXent_enable(ent,fl)
		#define phent_setmass(ent,mass,hull)		pXent_setmass(ent,mass)
		#define phent_setgroup(ent,num)				pXent_setgroup(ent,num)
		#define phent_setfriction(ent,v)				pXent_setfriction(ent,v)
		#define phent_setelasticity(ent,v,s)		pXent_setelasticity(ent,v)
		#define phent_setdamping(ent,lin,ang)		pXent_setdamping(ent,lin,ang)
		#define phent_addcentralforce(ent,v1)		pXent_addforcecentral(ent,v1)
		#define phent_addforceglobal(ent,v1,v2)	pXent_addforceglobal(ent,v1,v2)
		#define phent_addforcelocal(ent,v1,v2)		pXent_addforcelocal(ent,v1,v2)
		#define phent_addvelcentral(ent,v1)			pXent_addvelcentral(ent,v1)
		#define phent_addvelglobal(ent,v1,v2)		pXent_addvelglobal(ent,v1,v2)
		#define phent_addvellocal(ent,v1,v2)		pXent_addvellocal(ent,v1,v2)
		#define phent_addtorqueglobal(ent,v1)		pXent_addtorqueglobal(ent,v1)
		#define phent_addtorquelocal(ent,v1)		pXent_addtorquelocal(ent,v1)
		#define phent_getangvelocity(ent,v1)		pXent_getangvelocity(ent,v1)
		#define phent_getvelocity(ent,v1,v2)		pXent_getvelocity(ent,v1,v2)
		#define phent_clearvelocity(ent)				pXent_clearvelocity(ent)
		#define phcon_add(type,ent1,ent2)			pXcon_add(type,ent1,ent2,0)
		#define phcon_remove(ent)						pXcon_remove((void*)ent)
		#define phcon_setmotor(ent,v1,v2,v3)		pXcon_setmotor((void*)ent,v1,v2) 
		#define phcon_setparams1(ent,v1,v2,v3)		pXcon_setparams1((void*)ent,v1,v2,v3) 
		#define phcon_setparams2(ent,v1,v2,v3)		pXcon_setparams2((void*)ent,v1,v2,v3) 
	#endif
	////////////////////////////////////////////////////////////////////////////////////
	float NxPiF32 = 3.141592653589793;

	var CLOTH_DEFAULT[40] = 
	{
		// thickness: Thickness is usually a fraction of the overall extent of the cloth and should not be set to a value greater than that.
		// A good value is the maximaldistance between two adjacent cloth particles in their rest pose.
		// Visual artifacts or collision problems may appear if the thickness is too small.
		0.2, // Range: [0,inf]
		
		// selfCollisionThickness: Size of the particle diameters used for self collision and inter-collision.
		// The self collision thickness is usually a fraction of the overall extent of the cloth and should not be set to a value greater than that.
		// A good value is the maximal distance between two adjacent cloth particles in their rest pose.
		// Visual artifacts or collision problems may appear if the particle radius is too small.
		0.2, // Range: [0,inf]
		
		// density: Density of the cloth (mass per area).
		1.0, // Range: [0,inf]
		
		// bendingStiffness: Bending stiffness of the cloth. Only has an effect if the flag NX_CLF_BENDING is set.
		1.0, // Range: [0,1]
		
		// stretchingStiffness: Stretching stiffness of the cloth. Stretching stiffness must be larger than 0.
		1.0, // Range: [0,1]
		
		//	hardStretchLimitationFactor: Defines the hard stretch elongation limit.
		// If the flag NX_CLF_HARD_STRETCH_LIMITATION is set, the solver pulls vertices with a maxDistance greater zero towards vertices with maxDistance zero (attached).
		// This reduces the stretchiness even for low solver iteration counts.
		// The process is non-physical and can yield small ghost forces.
		// The hardStretchLimitationFactor defines by what factor the cloth is allowed to stretch.
		1.0, // Range: [0,inf]
		
		// dampingCoefficient: Spring damping of the cloth. Only has an effect if the flag NX_CLF_DAMPING is set.
		0.5, // Range: [0,1]
		
		// friction: Friction coefficient. Defines the damping of the velocities of cloth particles that are in contact.
		0.5, // Range: [0,1]
		
		// pressure: If the flag NX_CLF_PRESSURE is set, this variable defines the volume of air inside the mesh as volume = pressure * restVolume.	
		// For pressure < 1 the mesh contracts w.r.t. the rest shape
		// For pressure > 1 the mesh expands w.r.t. the rest shape
		1.0, // Range: [0,inf]
		
		// tearFactor: If the flag NX_CLF_TEARABLE is set, this variable defines the elongation factor that causes the cloth to tear.
		// When the buffer cannot hold the new vertices anymore, tearing stops.
		1.5, //	Range: [1,inf]
		
		// collisionResponseCoefficient: Defines a factor for the impulse transfer from cloth to colliding rigid bodies. 
		// Only has an effect if NX_CLF_COLLISION_TWOWAY is set.
		0.2, // Range: [0,inf]
		
		// attachmentResponseCoefficient: Defines a factor for the impulse transfer from cloth to attached rigid bodies.
		// Only has an effect if the mode of the attachment is set to NX_CLOTH_ATTACHMENT_TWOWAY.
		0.2, // Range: [0,1]
		
		// attachmentTearFactor: If the flag NX_CLOTH_ATTACHMENT_TEARABLE is set in the attachment method of NxCloth.
		// This variable defines the elongation factor that causes the attachment to tear.
		1.5, // Range: [1,inf]
		
		// toFluidResponseCoefficient: Defines a factor for the impulse transfer from this cloth to colliding fluids.
		// Only has an effect if the NX_CLF_FLUID_COLLISION flag is set. Note: Large values can cause instabilities
		1.0, // Range: [0,inf]
		
		// fromFluidResponseCoefficient: Defines a factor for the impulse transfer from colliding fluids to this cloth.
		// Only has an effect if the NX_CLF_FLUID_COLLISION flag is set. Note: Large values can cause instabilities
		1.0, // Range: [0,inf]
		
		// compressionLimit: Defines a factor up to which the cloth is weak under compression.
		//	This factor is the lower part of a range [limit, 1].
		// Whenever an edge is compressed to something between [limit, 1] * original length, the stretchingStiffness is multiplied with the compressionStiffness.
		//	Since the compressionStiffness is smaller or equal to 1, this will always define a range where the cloth reacts more softly to compressed edges.
		// This feature is useful to generate small wrinkles features on the cloth mesh outside the SDK.
		1.0, // Range: [0,1]
		
		// compressionStiffness: Defines the stiffness of the cloth under compression.
		// This stiffness scales the stretchingStiffness in the range defined by compressionLimit.
		1.0, // Range: [0,1]
		
		// minAdhereVelocity: If the NX_CLF_ADHERE flag is set the cloth moves partially in the frame of the attached actor.
		// This feature is useful when the cloth is attached to a fast moving character.
		// In that case the cloth adheres to the shape it is attached to while only velocities below the parameter minAdhereVelocity are used for secondary effects.
		1.0, // Range: [0,inf]
		
		// solverIterations: Number of solver iterations.
		// Note: Small numbers make the simulation faster while the cloth gets less stiff.
		3, // Range: [1,inf]
		
		// hierarchicalSolverIterations: Number of iterations of the hierarchical cloth solver.
		// For the this value to have an effect, the parameter NxClothMeshDesc.numHierarchyLevels must be greater then one when cooking the cloth mesh.
		// In this case, the hierarchical cloth solver uses the mesh hierarchy to speed up convergence, i.e. makes large pieces of cloth less stretchy.
		2, // Range: [0,inf]
		
		// wakeUpCounter: The cloth wake up counter. Corresponds to 20 frames for the standard time step.
		4.0, // Range: [0,inf]
		
		// sleepLinearVelocity: Maximum linear velocity at which cloth can go to sleep.
		// If negative, the global default will be used.
		-1.0, // Range: [-1,inf]
		
		// collisionGroup: Sets which collision group this cloth is part of.
		0, // Range: [0, 31]
		
		// forceFieldMaterial: Force Field Material Index, index != 0 has to be created.
		0,
		
		// relativeGridSpacing: This parameter defines the size of grid cells for collision detection. 
		// Note: Setting the size of grid cells to small or to big, crashes your exe!		
		0.25, // Range: [0.01,inf]
		
		// externalAcceleration: External acceleration which affects all non attached particles of the cloth.
		0, 0, 0,

		// windAcceleration: Acceleration which acts normal to the cloth surface at each vertex.
		0, 0, 0,
		
		// validBounds: 
		// If the flag NX_CLF_VALIDBOUNDS is set, this variable defines the volume outside of which cloth particle are automatically removed from the simulation.
		// XYZ bounding box minimum coordinate
		0, 0, 0,
		// XYZ bounding box maximum coordinate
		0, 0, 0,
		
		// Flag bits.
		0 // NX_CLF_HARDWARE is set automatically, depending on your GPU (see above)
	};
	
	//main functions
	void* physX_load(); // returns NxPhysicsSDK pointer
	function physX_destroy();
	void* physX_run(var deltatime); // 0 => use fixed time steps; returns NxScene pointer

	//return number of actors for num == 1
	function pX_stats(var);

	//new functions
	
	// Creates a cloth physics object (ent), with an attached entity (ent1,ent2) or NULL, the CLOTH_DEFAULT(var) and cloths flags(var) (see above)
	function pXent_cloth(ENTITY*, ENTITY*, var, ENTITY*, var, var*);
	
	// Reset all SDK Cloths.
	function pXent_reset_cloth();

	// Continuous Collision Detection. Set it, after you loaded the PhysX engine.
	// you have to use pXent_SetCCDSkeleton(entity) to see the effect!
	function pX_setccd(var);

	// var1 = sets the speed of the simulation, 60 is default (intern 1/60).
	// var2 = maxIter provides a cap on the number of sub steps executed, per time step, 8 is default.
	// var3 = TimeStep Methods: NX_TIMESTEP_FIXED NX_TIMESTEP_VARIABLE NX_TIMESTEP_INHERIT
	function pX_setsteprate(var,var,var);

	// scales the A7 world -> more realistic result.
	// a good value is around 0.05 for the carlevel, but it depends on size of your objects (A7 units convert to meter)
	function pX_setunit(var); // default: 1/40

	// pauses the NVIDIA PhysX engine or rather the physX_run function
	function pX_pause(var);

	// set the number of the groups, the third parameter defines whether they collide or not
	function pX_setgroupcollision(var, var, var);

	// change the force mode, affects: pXent_addcentralforce, pXent_addforceglobal, pXent_addforcelocal,...
	function pX_setforcemode(var);

	// the entity must be an actor
	// the second parameter sets the shape flag, the third turns it on/off
	// for trigger flags:
	// uses the volume of an actor as a trigger area, works with the emask: ENABLE_TRIGGER and returns the trigger number.
	// me/my pointer is the trigger Actor, you pointer is the Actor that collides with the trigger
	//(be careful: the actor->userdata gets overwritten(intern))
	function pXent_settriggerflag(ENTITY*, var, var);

	// raises or clears a particular body flag; you can make 2D physics with e.g. NX_FROZEN_POS_Y
	// int: NX_FROZEN_POS, NX_FROZEN_ROT, ...
	function pXent_setbodyflag(ENTITY*, var, var);

	function pXent_setbodyflagall(var mode,var flag)
	{
		for (you = ent_next(NULL); you; you = ent_next(you))
		{ 
			if (you->body) pXent_setbodyflag(you,mode,flag);
		}
	}

	// raises or clears an Actor flag;
	function pXent_setactorflag(ENTITY*, var, var);


	// set a CCD Skeleton for an actor
	// it creates a CCD box in the entity size, the vector scales the box, a good scale vector is: vector(0.5,0.5,0.5).
	// sets the last parameter to 1, if you like tp perform dynamic vs. dynamic CCD collision.
	function pXent_setccdskeleton(ENTITY*,VECTOR*,var);

	//	The solver iteration count determines how accurately joints and contacts are resolved. 
	//	If you are having trouble with jointed bodies oscillating and behaving erratically, then
	//	setting a higher solver iteration count may improve their stability.
	//	Range:[1,255]
	// The solver iteration count defaults to 4, but may be set individually for each body.
	function pXent_setiterations(ENTITY*, var);

	// Sets the SkinWidth default value: 0.025
	function pXent_setskinwidth(ENTITY*, var);

	// Sets Actor to sleep with 1 and wake him up with 0
	function pXent_setsleep(ENTITY *, var);

	// use NX_IGNORE_PAIR, NX_NOTIFY_ON_START_TOUCH,... for the third parameter
	function pXent_setcollisionflag(ENTITY*, ENTITY*, var);

	// removes a shape from an actor, starting with 0
	// the second parameter is for the shape number (normally 0)
	function pXent_removeshape(ENTITY *, var);

	// adds a new shape to an actor: ent1 = actor, ent2 = added shape
	// third parameter = hull (PH_BOX, PH_SPHERE or PH_CAPSULE)
	function pXent_addshape(ENTITY *,ENTITY *,var);

	// directly set an actor to a new position;
	function pXent_setposition(ENTITY*,VECTOR*);

	// direct access to actor->setLinearVelocity(VECTOR);
	function pXent_setvelocity(ENTITY*,VECTOR*);

	// direct access to actor->setAngularVelocity(VECTOR);
	function pXent_setangvelocity(ENTITY*,VECTOR*);

	// deprecated functions
	function pXent_moveglobal( ENTITY*, VECTOR*, ANGLE*);
	function pXent_movelocal( ENTITY*, var, VECTOR*, ANGLE*);
	function pXent_movechar(ENTITY*, VECTOR*, ANGLE*, var);

	// move a physics or kinematic entity, or a character controller
	var pXent_move(ENTITY*,VECTOR* vRelPos,VECTOR* vAbsPos);

	// rotate a physics or kinematic entity, or a character controller
	var pXent_rotate(ENTITY*,ANGLE* vAngle,ANGLE* vAbsAngle);

	// direct access to a NxMaterialDesc material; vectors: standard , FrictionV, anisotropic
	/*		material.restitution=Clamp(_FLOAT(standard->x)*0.01f,0.0f,1.0f);
	material.staticFriction = _FLOAT(standard->y);
	material.dynamicFriction = _FLOAT(standard->z); 
	material.staticFrictionV = _FLOAT(FrictionV->x);  
	material.dynamicFrictionV = _FLOAT(FrictionV->y);  
	material.flags = _INT(FrictionV->z);  
	material.dirOfAnisotropy.set(_FLOAT(anisotropic->x),_FLOAT(anisotropic->y),_FLOAT(anisotropic->z));*/
	function pXent_setmaterial(ENTITY*, VECTOR*, VECTOR*, VECTOR*);

	// create a radial explosion; parameter: entity, vPos, force, radius
	function pXent_addexplosion(ENTITY*, VECTOR*, var, var);


	// with this function you can change the JointMotion:
	/*	NX_D6JOINT_MOTION_LOCKED
	NX_D6JOINT_MOTION_LIMITED
	NX_D6JOINT_MOTION_FREE
	// you can set the three Motions for every axis:
	/*	d6Desc.xMotion = JointMotion[0]
	d6Desc.yMotion = JointMotion[1]
	d6Desc.zMotion = JointMotion[2]
	d6Desc.twistMotion = JointMotion[3]
	d6Desc.swing1Motion = JointMotion[4]
	d6Desc.swing2Motion = JointMotion[5]*/
	void* pXcon_set6djoint(ENTITY*, var*, var*);

	// get revJoint->getGlobalAnchor() Vector
	function pXcon_getanchorpoint(ENTITY* ent,VECTOR* point);

	// traces a ray from the entity pos to a VECTOR = direction, var(1) = ShapeType , var(2) = RaycastType
	function pXent_raycast(void*, VECTOR*, var, var);

	// convert an Actor to a kinematic physics object (entity must be an Actor), sets ent.group = 1
	function pXent_kinematic ( ENTITY*, var );

	// you can pick dynamic actors with the mouse; put it in the main loop
	function pX_pick();


	//adopted functions:

	// pX_selectgroup must be updated manual.
	// when you delete Joints/Wheels with this function, you have to recreate them manual with pXcon_add or pXent_CreateD6Joint.
	// pX_selectgroup doesn't affected character controllers (PH_CHAR)
	//function pX_selectgroup ( var );

	// you can set gravity at any time
	function pX_setgravity (VECTOR*);

	// sets setSleepLinearVelocity and setSleepAngularVelocity for all actors
	function pX_setautodisable ( var, var );

	// create a physics object, ent, body, hull (uses c_updatehull(entity,1) & entity->group=1 intern)
	// PH_PLANE need no entity object (NULL); PH_CHAR doesn't collide with PH_PLANE
	function pXent_settype (ENTITY*, var, var );

	//all forces are similar to the old physics
	function pXent_addforcecentral ( ENTITY*, VECTOR*);
	function pXent_addforceglobal (ENTITY* ,VECTOR* ,VECTOR*);
	function pXent_addforcelocal (ENTITY* , VECTOR* , VECTOR* );
	function pXent_addvelcentral (ENTITY* ,VECTOR* );
	function pXent_addtorqueglobal (ENTITY* , VECTOR* );
	function pXent_addtorquelocal (ENTITY* , VECTOR* );
	function pXent_getangvelocity (ENTITY* , VECTOR* );
	function pXent_getvelocity (ENTITY* , VECTOR* , VECTOR* );

	// sets MaxAngularVelocity, NVIDIA PhysX doesn't have MaxLinearVelocity,
	// you have to set it by your own!(use:pXent_getvelocity() and pXent_SetLinearVelocity())
	function pXent_setmaxspeed(ENTITY*,var);

	// the mass gets automatically set by physX, however you can change it with this function
	function pXent_setmass(ENTITY*,var);
	function pXent_getmass(ENTITY*);

	// local offset of the mass point
	function pXent_setmassoffset(ENTITY *,VECTOR* offset,VECTOR* inertia);

	// remove or recreate an entity/actor
	function pXent_enable(ENTITY*,var);

	// works
	function pXent_setgroup(ENTITY*,var);

	// works
	function pXent_setdamping(ENTITY*,var,var);

	// Creates a new Material; uses the values of the old material and overrides staticFriction & dynamicFriction
	function pXent_setfriction(ENTITY*,var);

	// Creates a new Material; uses the values of the old material and overrides restitution; Range:[0,100]
	function pXent_setelasticity(ENTITY*,var);

	// second parameter is for the shape number from which you get the Dimensions of its bounding box
	function pXent_getbounds(ENTITY*,var,VECTOR*);

	// works
	function pXent_makelocal(ENTITY*,VECTOR*,VECTOR*);

	// new: last var for intern collisions between the two entities (0,1) 
	void* pXcon_add(var,ENTITY*,ENTITY*,var);

	// if Joint -> release / if Wheel -> release Wheel Shape // you have to use pXent_settype(Wheelname,1,hull) if you want a breaking look (see car.c)
	// Joints will be moved to a list of "dead joints" when only the actor is released
	function pXcon_remove(ENTITY*);

	// you can put ENTITY* or a handle in the void* parameter
	function pXcon_setparams1(ENTITY*,VECTOR*,VECTOR*,VECTOR*);
	function pXcon_setparams2(ENTITY*,VECTOR*,VECTOR*,VECTOR*);

	void* pXcon_setwheel(ENTITY* ent,var SteerAngle, var MotorTorque, var BrakeTorque);
	/*function pXcon_setmotor(ENTITY* ent,VECTOR* vSteer,VECTOR* vMotor)
	{
		static var SteerAngle = 0;
		SteerAngle += vSteer.x;
		SteerAngle = clamp(SteerAngle,-vSteer.y,vSteer.y);
		var MotorTorque = clamp(vMotor.x,-vMotor.y,vMotor.y);
		var BrakeTorque = vMotor.z;
		return pXcon_setwheel(ent,SteerAngle,MotorTorque,BrakeTorque);
	}*/

	function pXcon_setmotor(ENTITY *ent,VECTOR* params,var mode);

	// you get for RevoluteJoint: 	Limit Angles
	//				  SphericalJoint:		Global Axis
	//				  CylindricalJoint:  WorldLimitPos
	//				  Wheel: 				x=SteerAngle y=RollAngle z=MotorTorque
	function pXcon_getposition(ENTITY*,VECTOR*);

	/////////////////////////////////////////////////////////
	void* NxPhysicsSDK = NULL;
	void* NxScene = NULL;

	function on_exit_px();
	function on_level_load_px();
	function on_ent_remove_px(ENTITY* ent);

	function on_exit_px0() { return; }
	function on_level_load_px0() { return; }
	function on_ent_remove_px0(ENTITY* ent) { return; }

	function physX_level_load()
	{
		if (!NxPhysicsSDK) return;
		
		if (level_ent)
		pXent_settype(level_ent,PH_STATIC,PH_POLY);
		for (you = ent_next(NULL); you; you = ent_next(you))
		{ 
			if (you->flags & PASSABLE) continue;	
			var type = ent_type(you);
			if(type == 5 && (you->emask & DYNAMIC)) continue;	// only register static models
			if(type < 2 && type > 5)  continue; // blocks, models, or terrain only
			if(type == 4)
			pXent_settype(you,PH_STATIC,PH_TERRAIN);
			else	
			pXent_settype(you,PH_STATIC,PH_POLY);
		}
		on_level_load_px();
	}

	function physX_ent_remove(ENTITY* ent)
	{
		//if(!NxPhysicsSDK) {printf("!NxPhysicsSDK return!"); return;}
		//if(ent.body) {printf("ent.body"); pXent_settype(ent,0,0);}		
		if(!NxPhysicsSDK) return;
		if(ent->body) pXent_settype(ent,0,0);		
		on_ent_remove_px(ent);
	}
	
	function exit_event()
	{
		for(you = ent_next(NULL); you; you = ent_next(you))
		{
			ent_setmesh(you,0,0,0);
		}
		sys_exit(NULL);
	}

	function physX_close()
	{
		physX_destroy();
		NxScene = NULL;
		NxPhysicsSDK = NULL;
		on_exit_px();
	}

	function physX_open()
	{
		if(NxPhysicsSDK) return; // already initialized
		if(version < 8) {
			error("PhysX only supported in A8!");
			return;
		}
		
		NxPhysicsSDK = physX_load();
		pX_setsteprate(60, 16, NX_TIMESTEP_FIXED);
		NxScene = physX_run(0);
		pX_setunit(1/40);

		if(!on_exit) on_exit = on_exit_px0;
		if(!on_level_load) on_level_load = on_level_load_px0;
		if(!on_ent_remove) on_ent_remove = on_ent_remove_px0;

		on_exit_px = on_exit;	// store previous on_exit function
		on_exit = physX_close;
		on_level_load_px = on_level_load;
		on_level_load = physX_level_load;
		on_ent_remove_px = on_ent_remove;
		on_ent_remove = physX_ent_remove;

		while(NxPhysicsSDK) {
			
#ifdef FIXED_TIME
//				printf("FIXED_TIME");
//This for-loop fixes the physX framerate to 60 when the frame rate is below 60 fps
				int loc_n = integer(time_step * 60 / 16) + 1;
				int loc_i = 0;
				if (freeze_mode <= 0)		
				for (; loc_i < loc_n; loc_i++)
				physX_run(0);
#else
				if (freeze_mode <= 0)		
				physX_run(time_frame/16);
#endif
			proc_mode = PROC_EARLY;	// call physX_run before entity actions.
			wait(1);
		}	
	}
	
#endif // ACKPHYSX_H

