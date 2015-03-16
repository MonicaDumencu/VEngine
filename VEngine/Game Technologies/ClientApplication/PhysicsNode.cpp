#include "PhysicsNode.h"

const Vector3 PhysicsNode::gravity = Vector3(0, -0.01, 0);

PhysicsNode::PhysicsNode(void)	{
	target = NULL;

	// ---------- Added by Monica ------------ //
	useLinearDamping = true;
	applyLinearVelocity = true;
	amountMovement = Vector3(0.0f, 0.0f, 0.0f);
	isAtRest = false;
	useGravity = true;
	// --------------------------------------- //
}

PhysicsNode::PhysicsNode(Quaternion orientation, Vector3 position) {
	m_orientation	= orientation;
	m_position		= position;

	// ---------- Added by Monica ------------ //
	useLinearDamping = true;
	applyLinearVelocity = true;
	amountMovement = Vector3(0.0f, 0.0f, 0.0f);
	isAtRest = false;
	useGravity = true;
	// --------------------------------------- //
}

PhysicsNode::~PhysicsNode(void)	{

}


void	PhysicsNode::Update(float msec) {
	if (!isAtRest) {
		Vector3 acc = m_force*m_invMass + (useGravity ? gravity : Vector3(0, 0, 0));
		acc += m_acceleration;
		m_linearVelocity = m_linearVelocity + acc*msec;

		// ---------- Modified by Monica ------------ //
		// this is added because i do not want the terrain to use damping
		if (applyLinearVelocity) {
			if (useLinearDamping) {
				m_linearVelocity = m_linearVelocity *LINEAR_VELOCITY_DAMP;
			}
			else {
				m_linearVelocity = m_linearVelocity;
			}
		}

		//how much has moved since last update
		amountMovement = m_linearVelocity * msec;
		// ----------------------------------------//

		m_position = m_position + m_linearVelocity*msec;

		// ---- does not let anything fall behind 0 ---- //
		if (m_position.y < 26)
		{
			m_position.y = 26;
		}
		/// -------------------------- ///
	}

	if (target) {
		target->SetTransform(BuildTransform());
	}
}


Matrix4		PhysicsNode::BuildTransform() {
	Matrix4 m = m_orientation.ToMatrix();

	m.SetPositionVector(m_position);

	return m;
}