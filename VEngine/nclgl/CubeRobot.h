/******************************************************************************
Class:CubeRobot
Implements:SceneNode
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:A cuboid robot, created from a small sub-scenegraph. Root node
(ie 'this') is a transition node, which has a 'body' node as a child, which
in turn has 2 legs, 2 arms, and a head as child nodes. 

The tutorial notes suggest that CreateCube should be called by the student, 
but the constructor will actually call the function if necessary. 

Programs should still call DeleteCube on exit, to prevent memory leaks. 

STUDENT CHALLENGE! How would you re-engineer this class to automatically call
DeleteCube if all CubeRobots have been deleted?

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SceneNode.h"
#include "OBJMesh.h"
#include "common.h"
#include "ControllerHeader.h"

enum AnimationState{JUMP,RUN,STAND,RUSH,FLY,MAXIMENT};

class CubeRobot : public SceneNode	{
public:
	CubeRobot(void);
	virtual ~CubeRobot(void){};

	//Overloaded from SceneNode, makes the CubeRobot dance!
	virtual void	Update(float msec);

	//CubeRobot relies on their being a cube Mesh - this creates that Mesh.
	static void	CreateCube() {
		OBJMesh*m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		cube = m; 
	}

	//Deletes the CubeRobot cube Mesh.
	static void DeleteCube(){
		delete cube;
	}
	void StateTransfer(int number);
	void RobotRunAnimation(float msec);
	void RobotStand();
	void RobotJumpAnimation(float msec);
	void SetJump(bool isjump){ Isjump = isjump; }
	void RobotRush(float msec);
	void JumpPose1();
	void JumpPose2();
	void JumpPose3();
	void RobotFlyAnimation(float msec);
	void FlyPose1();
	void FlyPose2();
	void FlyPose3();
	SceneNode* GetBody(){ return body; }

	void SetState(int state);

	bool checkRunActive(){ return State[RUN];}
protected:
	static Mesh* cube;
	//These are just handy pointers to the limbs, to make animating them easier
	SceneNode*body;
	SceneNode*head;

	SceneNode*LeftForeArm;
	SceneNode*LeftLowArm;

	SceneNode*righForetArm;
	SceneNode*rightLowArm;

	SceneNode*leftForeLeg;
	SceneNode*leftLowLeg;

	SceneNode*rightForeLeg;
	SceneNode*rightLowLeg;

	float ArmtimeStep;
	float LegtimeStep;

	bool doesArmReverse;
	bool doesLegReverse;
	bool pause;
	int Armdirection;
	int Legdirection;

	float ZrotateAngle;
	float XrotateAngle;
	float JumpTime;
	float FlyTime;
	bool IsRum;
	bool Isjump;
	bool IsStand;
	bool State[MAXIMENT];
};

