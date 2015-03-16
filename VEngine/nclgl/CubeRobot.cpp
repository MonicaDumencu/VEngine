#include "CubeRobot.h"

//cube is a static class variable, and so must be initialised outside of the 
//class...here's as good a place as any!
Mesh* CubeRobot::cube = NULL;

CubeRobot::CubeRobot(void)	{	
	if(!cube) {
		CreateCube();
	}
	// ------- Commented out by Monica -------- //
	//Purpose: Not to render the cube between its legs
	//SetMesh(cube);
	// ---------------------------------------- //

	//Make the body
	body = new SceneNode(cube, Vector4(1, 0, 0, 1));
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);

	//Add the head
	head = new SceneNode(cube, Vector4(0, 1, 0, 1));
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	body->AddChild(head);

	//Add the left Forearm
	LeftForeArm = new SceneNode(cube, Vector4(0, 0, 1, 1));
	LeftForeArm->SetModelScale(Vector3(4, -7, 4));
	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-12, 27, -1)));
	body->AddChild(LeftForeArm);
	//Add the left LowArn
	LeftLowArm = new SceneNode(cube, Vector4(1, 0, 1, 1));
	LeftLowArm->SetModelScale(Vector3(3, -7, 3));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -15, 0)));
	LeftForeArm->AddChild(LeftLowArm);

	//Add the right forearm
	righForetArm = new SceneNode(cube, Vector4(0, 0, 1, 1));
	righForetArm->SetModelScale(Vector3(4, -7, 4));
	righForetArm->SetTransform(Matrix4::Translation(Vector3(12, 27, -1)));
	body->AddChild(righForetArm);
	//Add the right lowarm
	rightLowArm = new SceneNode(cube, Vector4(1, 0, 1, 1));
	rightLowArm->SetModelScale(Vector3(3, -7, 3));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -15, 0)));
	righForetArm->AddChild(rightLowArm);

	//Add the left foreleg
	leftForeLeg = new SceneNode(cube, Vector4(0, 0, 1, 1));
	leftForeLeg->SetModelScale(Vector3(3, -6, 3));
	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body->AddChild(leftForeLeg);
	//Add the Left Lowleg
	leftLowLeg = new SceneNode(cube, Vector4(1, 0, 1, 1));
	leftLowLeg->SetModelScale(Vector3(3, -15, 3));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0)));
	leftForeLeg->AddChild(leftLowLeg);

	//Finally the right foreleg!
	rightForeLeg = new SceneNode(cube, Vector4(0, 0, 1, 1));
	rightForeLeg->SetModelScale(Vector3(3, -6, 3));
	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body->AddChild(rightForeLeg);

	//and the rihgt lowleg
	rightLowLeg = new SceneNode(cube, Vector4(1, 0, 1, 1));
	rightLowLeg->SetModelScale(Vector3(3, -15, 3));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0)));
	rightForeLeg->AddChild(rightLowLeg);

	//Giant CubeRobot!
	//transform = Matrix4::Scale(Vector3(10,10,10));

	//The Scene Management Tutorial introduces these, as cheap culling tests
	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	LeftForeArm->SetBoundingRadius(18.0f);
	LeftLowArm->SetBoundingRadius(16.0f);

	righForetArm->SetBoundingRadius(18.0f);
	rightLowArm->SetBoundingRadius(16.0f);

	leftForeLeg->SetBoundingRadius(18.0f);
	leftLowLeg->SetBoundingRadius(16.0f);

	rightForeLeg->SetBoundingRadius(18.0f);

	ArmtimeStep = 0.0f;
	LegtimeStep = 0.0f;
	doesArmReverse = false;
	doesLegReverse = false;
	//IsRum = false;
	pause = false;
	Armdirection = 1;
	Legdirection = 1;
	ZrotateAngle = 0.0f;
	XrotateAngle = 0.0F;
	JumpTime = 0.0f;
	FlyTime = 0.0f;
	//Isjump = true;

	//Initia the aniamtion state
	for (int i = 0; i < MAXIMENT; i++)
	{
		State[i] = false;
	}
}

void CubeRobot::Update(float msec) {
	if (Window::GetKeyboard()->KeyDown(IS_RUM) && !Window::GetKeyboard()->KeyHeld(IS_RUM))
	{
		RobotStand();
		StateTransfer(RUN);

	}
	if (Window::GetKeyboard()->KeyDown(IS_NOT_RUM) && !Window::GetKeyboard()->KeyHeld(IS_NOT_RUM))
	{
		StateTransfer(STAND);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_G) && !Window::GetKeyboard()->KeyHeld(KEYBOARD_G))
	{
		RobotStand();
		StateTransfer(JUMP);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_U) && !Window::GetKeyboard()->KeyHeld(KEYBOARD_U))
	{
		RobotStand();
		StateTransfer(RUSH);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Y) && !Window::GetKeyboard()->KeyHeld(KEYBOARD_Y))
	{
		RobotStand();
		StateTransfer(FLY);
	}

	if (State[RUN])
	{
		RobotRunAnimation(msec);
	}
	else if (State[STAND])
	{
		RobotStand();
	}
	else if (State[JUMP])
	{
		RobotJumpAnimation(msec);
	}
	else if (State[RUSH])
	{
		RobotRush(msec);
	}
	else if (State[FLY])
	{
		RobotFlyAnimation(msec);
	}
	
	SceneNode::Update(msec);
}

void CubeRobot::RobotRunAnimation(float msec){
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))*Matrix4::Rotation(25, Vector3(1, 0, 0)));
	//Arm animation when robot run
	if (ArmtimeStep > 500 && !doesArmReverse)
	{
		doesArmReverse = true;
		Armdirection = -1;
	}

	if (ArmtimeStep < -500 && doesArmReverse)
	{
		doesArmReverse = false;
		Armdirection = 1;
	}
	if (doesArmReverse)
		ArmtimeStep -= msec * 2;
	else
		ArmtimeStep += msec * 2;

	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(75, Vector3(1, 0, 0)));
	LeftForeArm->SetTransform(LeftForeArm->GetTransform() * Matrix4::Rotation(2.2*Armdirection, Vector3(1, 0, 0)));

	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(75, Vector3(1, 0, 0)));
	righForetArm->SetTransform(righForetArm->GetTransform() * Matrix4::Rotation(2.2*(-Armdirection), Vector3(1, 0, 0)));

	//Leg animaiton when robot run
	if (LegtimeStep > 500 && !doesLegReverse)
	{
		doesLegReverse = true;
		Legdirection = -1;
	}
	if (LegtimeStep < -500 && doesLegReverse)
	{
		doesLegReverse = false;
		Legdirection = 1;

	}
	if (doesLegReverse)
		LegtimeStep -= msec * 2;
	else
		LegtimeStep += msec * 2;


	leftForeLeg->SetTransform(leftForeLeg->GetTransform() * Matrix4::Rotation(2.2*(-Legdirection), Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(30, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(rightForeLeg->GetTransform() * Matrix4::Rotation(2.2*(Legdirection), Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(30, Vector3(1, 0, 0)));
}

void CubeRobot::RobotStand(){
	ArmtimeStep = 0.0f;
	LegtimeStep = 0.0f;
	doesArmReverse = false;
	doesLegReverse = false;
	//IsRum = false;
	Armdirection = 1;
	Legdirection = 1;
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))*Matrix4::Rotation(0, Vector3(1, 0, 0)));

	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 27, -1)));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 0)));

	righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 27, -1)));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(1, -14, 0)));

	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0)));

	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0)));
}
void CubeRobot::RobotJumpAnimation(float msec){
	JumpTime += 1;
	if (JumpTime<3)
	{
		;
	}
	else if (JumpTime>3&&JumpTime<6)
	{
		JumpPose1();
	}
	else if (JumpTime>6 && JumpTime<8)
	{
		JumpPose2();

	}
	else if (JumpTime>8 && JumpTime<30){
		body->SetTransform(body->GetTransform()*Matrix4::Rotation(16, Vector3(1, 0, 0)));
	}
	else if (JumpTime>30 && JumpTime<40){
		leftForeLeg->SetTransform(leftForeLeg->GetTransform()*Matrix4::Rotation(4, Vector3(1, 0, 0)));
		rightForeLeg->SetTransform(rightForeLeg->GetTransform()*Matrix4::Rotation(4, Vector3(1, 0, 0)));

		leftLowLeg->SetTransform(leftLowLeg->GetTransform()*Matrix4::Rotation(-4, Vector3(1, 0, 0)));
		rightLowLeg->SetTransform(rightLowLeg->GetTransform()*Matrix4::Rotation(-4, Vector3(1, 0, 0)));
	}
	else if (JumpTime>50)
	{
		JumpTime = 0;
		StateTransfer(RUN);
		RobotStand();
	}
}

void CubeRobot::StateTransfer(int number){
	for (int i = 0; i < MAXIMENT; i++)
	{
		State[i] = false;
	}
	State[number] = true;
	if (FlyTime!=0||JumpTime!=0)
	{
		FlyTime = 0;
		JumpTime = 0;
	}
}
void CubeRobot::JumpPose1(){
	
	//cout << ".............runpose1........" << endl;
	body->SetTransform(body->GetTransform()*Matrix4::Rotation(10, Vector3(1, 0, 0)));
	
	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 30, -1)));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	

	righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 30, -1)));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));


	/*leftForeLeg->SetTransform(leftForeLeg->GetTransform() * Matrix4::Rotation(2.2*(-Legdirection), Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(30, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(rightForeLeg->GetTransform() * Matrix4::Rotation(2.2*(Legdirection), Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(30, Vector3(1, 0, 0)));*/

	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0))*Matrix4::Rotation(-45, Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(45, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0))*Matrix4::Rotation(-45, Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(45, Vector3(1, 0, 0)));

}
void CubeRobot::JumpPose2(){
	body->SetTransform(body->GetTransform()*Matrix4::Rotation(10, Vector3(1, 0, 0)));

	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 30, -1)));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));


	righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 30, -1)));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));


	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
}
void CubeRobot::JumpPose3(){
		//cout << ".............runpose1........" << endl;
		body->SetTransform(body->GetTransform()*Matrix4::Rotation(10, Vector3(1, 0, 0)));

		LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 30, -1)));
		LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));

		righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 30, -1)));
		rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));

		leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0))*Matrix4::Rotation(-45, Vector3(1, 0, 0)));
		leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(45, Vector3(1, 0, 0)));
		rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0))*Matrix4::Rotation(-45, Vector3(1, 0, 0)));
		rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(45, Vector3(1, 0, 0)));
}
void CubeRobot::RobotRush(float msec){
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))*Matrix4::Rotation(45, Vector3(1, 0, 0)));
	if (ArmtimeStep > 500 && !doesArmReverse)
	{
		doesArmReverse = true;
		Armdirection = -1;
	}

	if (ArmtimeStep < -500 && doesArmReverse)
	{
		doesArmReverse = false;
		Armdirection = 1;
	}
	if (doesArmReverse)
		ArmtimeStep -= msec * 3;
	else
		ArmtimeStep += msec * 3;

	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(75, Vector3(1, 0, 0)));
	LeftForeArm->SetTransform(LeftForeArm->GetTransform() * Matrix4::Rotation(5.5*Armdirection, Vector3(1, 0, 0)));

	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(75, Vector3(1, 0, 0)));
	righForetArm->SetTransform(righForetArm->GetTransform() * Matrix4::Rotation(5.5*(-Armdirection), Vector3(1, 0, 0)));

	//Leg animaiton when robot rush
	if (LegtimeStep > 500 && !doesLegReverse)
	{
		doesLegReverse = true;
		Legdirection = -1;
	}
	if (LegtimeStep < -500 && doesLegReverse)
	{
		doesLegReverse = false;
		Legdirection = 1;

	}
	if (doesLegReverse)
		LegtimeStep -= msec * 3;
	else
		LegtimeStep += msec * 3;
	leftForeLeg->SetTransform(leftForeLeg->GetTransform() * Matrix4::Rotation(4.4*(-Legdirection), Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(35, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(rightForeLeg->GetTransform() * Matrix4::Rotation(4.4*(Legdirection), Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(35, Vector3(1, 0, 0)));
}

void CubeRobot::RobotFlyAnimation(float msec){
	FlyTime += 1;
	if (FlyTime<3)
	{
		FlyPose1();
	}
	
	else if (FlyTime>3&&FlyTime<13)
	{
		righForetArm->SetTransform(righForetArm->GetTransform()*Matrix4::Rotation(2, Vector3(1, 0, 0)));

		LeftForeArm->SetTransform(LeftForeArm->GetTransform()*Matrix4::Rotation(-3.5, Vector3(1, 0, 0)));
		LeftLowArm->SetTransform(LeftLowArm->GetTransform()*Matrix4::Rotation(2, Vector3(1, 0, 0)));
	}
	else if (FlyTime>13&&FlyTime<22)
	{
		FlyPose2();
	}
	else if (FlyTime>22 && FlyTime<60)
	{
		body->SetTransform(body->GetTransform()*Matrix4::Rotation(1, Vector3(1, 0, 0)));
		LeftForeArm->SetTransform(LeftForeArm->GetTransform()*Matrix4::Rotation(-2.5, Vector3(1, 0, 0)));
		LeftLowArm->SetTransform(LeftLowArm->GetTransform()*Matrix4::Rotation(1.5, Vector3(1, 0, 0)));
	}
	else if (FlyTime>60 && FlyTime<380)
	{
		;
	}
	else if (FlyTime>380 && FlyTime<390)
	{
		FlyPose3();
	}
	
	else if (FlyTime>390)
	{
		FlyTime = 0;
		StateTransfer(RUN);
		RobotStand();
	}

}
void CubeRobot::FlyPose1(){
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))*Matrix4::Rotation(20, Vector3(1, 0, 0)));

	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 30, -1))*Matrix4::Rotation(-45, Vector3(1, 0, 0)));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -16, -5))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));


	righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 30, -1))*Matrix4::Rotation(45, Vector3(1, 0, 0)));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -16, -5))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	

	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0))*Matrix4::Rotation(0, Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(0, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0))*Matrix4::Rotation(-45, Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
}
void CubeRobot::FlyPose2(){
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))*Matrix4::Rotation(35, Vector3(1, 0, 0)));

	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 30, -1))*Matrix4::Rotation(-65, Vector3(1, 0, 0)));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -16, 0))*Matrix4::Rotation(-65, Vector3(1, 0, 0)));


	righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 30, -1))*Matrix4::Rotation(45, Vector3(1, 0, 0)));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -16, -5))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));


	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0))*Matrix4::Rotation(0, Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(0, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0))*Matrix4::Rotation(-15, Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(35, Vector3(1, 0, 0)));
}
void CubeRobot::FlyPose3(){
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))*Matrix4::Rotation(40, Vector3(1, 0, 0)));

	LeftForeArm->SetTransform(Matrix4::Translation(Vector3(-13, 30, -1)));
	LeftLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));


	righForetArm->SetTransform(Matrix4::Translation(Vector3(13, 30, -1)));
	rightLowArm->SetTransform(Matrix4::Translation(Vector3(-1, -14, 10))*Matrix4::Rotation(90, Vector3(1, 0, 0)));


	leftForeLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	leftLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	rightForeLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0))*Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	rightLowLeg->SetTransform(Matrix4::Translation(Vector3(0, -12, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
}

void CubeRobot::SetState(int state){
	StateTransfer(state);
}