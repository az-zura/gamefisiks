#include "MassSpringSystemSimulator.h"

// Spring System-------------------------------------------------------------------------------------------------------------------------------
class Spring {
	public:
		int point1;
		int point2;
		float initialLength;

		Spring(int p1, int p2, float initlen) {
			point1 = p1;
			point2 = p2;
			initialLength = initlen;
		}

		Spring(){}
};

class Point {
	public:
		Vec3 position;
		Vec3 velocity;
		Vec3 force;
		bool isFixed;
		Vec3 posmid;
		Vec3 velmid;

		Point(Vec3 pos, Vec3 vel, bool fixed) {
			position = pos;
			velocity = vel;
			force = Vec3(0, 0, 0);
			isFixed = fixed;
			posmid = Vec3(0, 0, 0);
			velmid = Vec3(0, 0, 0);
		}
		
		Point() {};
};

Spring springs[20] = {};
int numSprings = 0;

Point points[40] = {};
int numPoints = 0; 

float gravity = 0;

// Construtors----------------------------------------------------------------------------------------------------------------------------------
MassSpringSystemSimulator::MassSpringSystemSimulator(){
	m_iTestCase = 0;
	setMass(0);
	setStiffness(0);
	setDampingFactor(0);
	m_iIntegrator = 0;
}

// UI Functions---------------------------------------------------------------------------------------------------------------------------------
const char* MassSpringSystemSimulator::getTestCasesStr() {
	return "Demo1,Demo2,Demo3,Demo4,Demo5";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC) {
	this->DUC = DUC;
	Spring spring;
	TwType TW_TYPE_INTEGRATOR = TwDefineEnumFromString("Euler", "Euler,Midpoint,LeapFrog");
	TwAddVarRW(DUC->g_pTweakBar, "Integrator", TW_TYPE_INTEGRATOR, &m_iIntegrator, "");
	switch (m_iTestCase)
	{
	case 0:
		setMass(10);

		addSpring(addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false), addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false), 1);
		setStiffness(40);
		spring = springs[0];
		eulerStep(0.1);
		cout << "Euler Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";

		numPoints = 0;
		numSprings = 0;

		addSpring(addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false), addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false), 1);
		setStiffness(40);
		spring = springs[0];
		midpointStep(0.1);
		cout << "Midpoint Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";

		numPoints = 0;
		numSprings = 0;

		addSpring(addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false), addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false), 1);
		setStiffness(40);
		spring = springs[0];
		leapFrogStep(0.1);
		cout << "Leap Frog Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		break;
	case 1:
		setMass(10);
		addSpring(addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false), addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false), 1);
		setStiffness(40);
		eulerStep(0.005);
		cout << "1. Euler Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		eulerStep(0.005);
		cout << "2. Euler Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		break;
	case 2:
		setMass(10);
		addSpring(addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false), addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false), 1);
		setStiffness(40);
		midpointStep(0.005);
		cout << "1. Midpoint Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		midpointStep(0.005);
		cout << "2. Midpoint Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		break;
	case 3:
		setMass(10);
		addSpring(addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false), addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false), 1);
		setStiffness(40);
		leapFrogStep(0.005);
		cout << "1. Leap Frog Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		midpointStep(0.005);
		cout << "2. Leap Frog Step: \n" << "point 0 vel " << points[springs[0].point1].velocity << ", pos " << points[springs[0].point1].position << "\n";
		cout << "point 1 vel " << points[springs[0].point2].velocity << ", pos " << points[springs[0].point2].position << "\n";
		break;
	case 4:
		
		setMass(10);
		setStiffness(40);
		setDampingFactor(.1);
		gravity = -50;

		TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "min=0 step=0.01");
		TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "min=0 step=0.01");
		TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0 step=0.01");
		TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_FLOAT, &gravity, "max=0 step=0.01");
		
		//Square
		addMassPoint(Vec3(0, 0, 0), Vec3(0, 0, 0), false); //0
		addMassPoint(Vec3(0, 0, 1), Vec3(0, 0, 0), false); //1
		addMassPoint(Vec3(0, 1, 0), Vec3(0, 0, 0), false); //2
		addMassPoint(Vec3(0, 1, 1), Vec3(0, 0, 0), false); //3
		addMassPoint(Vec3(1, 0, 0), Vec3(0, 0, 0), false); //4
		addMassPoint(Vec3(1, 0, 1), Vec3(0, 0, 0), false); //5
		addMassPoint(Vec3(1, 1, 0), Vec3(0, 0, 0), false); //6
		addMassPoint(Vec3(1, 1, 1), Vec3(0, 0, 0), false); //7

		//Line
		addMassPoint(Vec3(-1, -1, -1), Vec3(0, 0, 0), false); //8
		addMassPoint(Vec3(-1, 1, -1), Vec3(0, 0, 0), false); //9
		
		//Square
		addSpring(0, 1, 1);
		addSpring(0, 2, 1);
		addSpring(0, 4, 1);
		addSpring(3, 1, 1);
		addSpring(3, 2, 1);
		addSpring(3, 7, 1);
		addSpring(4, 5, 1);
		addSpring(4, 6, 1);
		addSpring(7, 5, 1);
		addSpring(7, 6, 1);
		addSpring(1, 5, 1);
		addSpring(2, 6, 1);

		//Line
		addSpring(8, 9, 1);

		addSpring(0, 8, 1);
		addSpring(1, 9, 1);
		
		break;
	default: break;
	}
}

void MassSpringSystemSimulator::reset() {
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	std::mt19937 eng;
	std::uniform_real_distribution<float> randCol(0.0f, 1.0f);

	for (int i = 0; i < numPoints; i++)
	{
		DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(randCol(eng), randCol(eng), randCol(eng)));
		DUC->drawSphere(points[i].position, (.05, .05, .05));
	}

	for (int i = 0; i < numSprings; i++) {
		Point point1 = points[springs[i].point1];
		Point point2 = points[springs[i].point2];
		DUC->beginLine();
		DUC->drawLine(point1.position, Vec3(0, 1, 0), point2.position, Vec3(1, 0, 0));
		DUC->endLine();
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase) {
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		cout << "Demo1!\n";
		numSprings = 0;
		numPoints = 0;
		applyExternalForce(Vec3(0, 0, 0));
		break;
	case 1:
		cout << "Demo2!\n";
		numSprings = 0;
		numPoints = 0;
		applyExternalForce(Vec3(0, 0, 0));
		break;
	case 2:
		cout << "Demo3!\n";
		numSprings = 0;
		numPoints = 0;
		applyExternalForce(Vec3(0, 0, 0));
		break;
	case 3:
		cout << "Demo4!\n";
		numSprings = 0;
		numPoints = 0;
		applyExternalForce(Vec3(0, 0, 0));
		break;
	case 4:
		cout << "Demo5!\n";
		numSprings = 0;
		numPoints = 0;
		applyExternalForce(Vec3(0, 0, 0));
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed){
	// Apply the mouse deltas to g_vfMovableObjectPos (move along cameras view plane)
	Point2D mouseDiff;
	mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
	mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
	if (mouseDiff.x != 0 || mouseDiff.y != 0)
	{
		Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
		worldViewInv = worldViewInv.inverse();
		Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
		Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
		// find a proper scale!
		float inputScale = 0.01f;
		inputWorld = inputWorld * inputScale;
		applyExternalForce(inputWorld);
	}
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep) {
	switch (m_iTestCase)
	{
		case 0: break;
		case 1: break;
		case 2: break;
		case 3: break;
		case 4: 
			switch (m_iIntegrator) {
				case 0:
					eulerStep(timeStep);
					break;
				case 1: 
					midpointStep(timeStep);
					break;
				case 2: 
					leapFrogStep(timeStep);
					break;
				default: break;
			}
			break;
		default: break;
	}
}

void calculateForce(Vec3 p1, Vec3 p2, float stiffness, Spring spring) {

	//direction vector
	Vec3 d1 = p1.operator-(p2);

	//length of direction vector
	float currentLength = sqrt(p1.squaredDistanceTo(p2));

	// normalize direction vector
	d1 = d1.operator/(currentLength);

	float lengthDiff = currentLength - spring.initialLength;
	float hooke = -stiffness * lengthDiff;

	//update force
	Vec3 f1 = points[spring.point1].force;
	Vec3 f2 = points[spring.point2].force;

	points[spring.point1].force = f1.operator+(d1.operator*(hooke));
	points[spring.point2].force = f2.operator+(d1.operator*(-hooke)); // same as p1 but *-1

}

Vec3 calculatePosAndVel(Vec3 vec1, Vec3 vec2, float timeStep) {

	Vec3 newvec = vec1.operator+(vec2.operator*(timeStep));

	return newvec;

}

void resetForces() {
	for (int i = 0; i < numPoints; i++) {
		points[i].force = Vec3(0, 0, 0);
		points[i].force.y += gravity;
	}
}

void boundingBox(int i) {
	if (points[i].position.x < -1) {
		points[i].position.x = -1;
		points[i].velocity.x *= -0.75;
	}
	if (points[i].position.x > 1) {
		points[i].position.x = 1;
		points[i].velocity.x *= -0.75;
	}

	if (points[i].position.y < -1) {
		points[i].position.y = -1;
		points[i].velocity.y *= -0.75;
	}
	if (points[i].position.y > 1) {
		points[i].position.y = 1;
		points[i].velocity.y *= -0.75;
	}

	if (points[i].position.z < -1) {
		points[i].position.z = -1;
		points[i].velocity.z *= -0.75;
	}
	if (points[i].position.z > 1) {
		points[i].position.z = 1;
		points[i].velocity.z *= -0.75;
	}
}

void MassSpringSystemSimulator::eulerStep(float timeStep) {

	resetForces();

	for (int i = 0; i < numSprings; i++) {
		//Get all forces 
		Spring spring = springs[i];

		Vec3 p1 = getPositionOfMassPoint(spring.point1);
		Vec3 p2 = getPositionOfMassPoint(spring.point2);

		calculateForce(p1, p2, m_fStiffness, spring);
	}

	for (int i = 0; i < numPoints; i++) {

		//calculate acceleration of old position
		Vec3 f = points[i].force.operator/(m_fMass);
		Vec3 dampf = points[i].velocity.operator*(m_fDamping);
		Vec3 a = f.operator-(dampf);
		a = f.operator+(m_externalForce);

		// mid position and velocity
		points[i].position = calculatePosAndVel(points[i].position, points[i].velocity, timeStep);

		points[i].velocity = calculatePosAndVel(points[i].velocity, a, timeStep);

		if (m_iTestCase == 4) {
			boundingBox(i);
		}

	}

}

void MassSpringSystemSimulator::midpointStep(float timeStep) {
	
	resetForces();

	for (int i = 0; i < numSprings; i++) {
		//Get all forces 
		Spring spring = springs[i];

		Vec3 p1 = getPositionOfMassPoint(spring.point1);
		Vec3 p2 = getPositionOfMassPoint(spring.point2);

		calculateForce(p1, p2, m_fStiffness, spring);
	}

	for (int i = 0; i < numPoints; i++) {
		
		//calculate acceleration of old position
		Vec3 f = points[i].force.operator/(m_fMass);
		Vec3 dampf = points[i].velocity.operator*(m_fDamping);
		Vec3 a = f.operator-(dampf); 
		a = f.operator+(m_externalForce);

		// mid position and velocity
		points[i].posmid = calculatePosAndVel(points[i].position, points[i].velocity, timeStep * 0.5);
		
		points[i].velmid = calculatePosAndVel(points[i].velocity, a, timeStep * 0.5);

	}

	resetForces();

	for (int i = 0; i < numSprings; i++) {
		//Get all forces 
		Spring spring = springs[i];

		Vec3 p1 = points[spring.point1].posmid;
		Vec3 p2 = points[spring.point2].posmid;

		calculateForce(p1, p2, m_fStiffness, spring);	
	}

	for (int i = 0; i < numPoints; i++) {

		//calculate acceleration of mid position
		Vec3 f = points[i].force.operator/(m_fMass);
		Vec3 dampf = points[i].velocity.operator*(m_fDamping);
		Vec3 a = f.operator-(dampf);
		a = f.operator+(m_externalForce);

		// calculate actual position and velocity
		points[i].position = calculatePosAndVel(points[i].position, points[i].velmid, timeStep);
		
		points[i].velocity = calculatePosAndVel(points[i].velocity, a, timeStep);

		if (m_iTestCase == 4) {
			boundingBox(i);
		}

	}
}

void MassSpringSystemSimulator::leapFrogStep(float timeStep) {

	resetForces();

	for (int i = 0; i < numSprings; i++) {
		//Get all forces 
		Spring spring = springs[i];

		Vec3 p1 = getPositionOfMassPoint(spring.point1);
		Vec3 p2 = getPositionOfMassPoint(spring.point2);

		calculateForce(p1, p2, m_fStiffness, spring);
	}

	for (int i = 0; i < numPoints; i++) {

		//calculate acceleration of old position
		Vec3 f = points[i].force.operator/(m_fMass);
		Vec3 dampf = points[i].velocity.operator*(m_fDamping);
		Vec3 a = f.operator-(dampf);
		a = f.operator+(m_externalForce);

		//position and velocity
		points[i].velocity = calculatePosAndVel(points[i].velocity, a, timeStep * 0.5);

		points[i].position = calculatePosAndVel(points[i].position, points[i].velocity, timeStep);

		if (m_iTestCase == 4) {
			boundingBox(i);
		}

	}

}

void MassSpringSystemSimulator::onClick(int x, int y) {
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y) {
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

// Specific Functions-------------------------------------------------------------------------------------------------------------------------------
void MassSpringSystemSimulator::setMass(float mass) {
	m_fMass = mass; 
}

void MassSpringSystemSimulator::setStiffness(float stiffness) {
	m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping) {
	m_fDamping = damping; 
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed) {
	Point point = Point(position, Velocity, isFixed);
	points[numPoints] = point;
	numPoints++;
	return (numPoints - 1);
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength) {
	Spring spring = Spring(masspoint1, masspoint2, initialLength);
	springs[numSprings] = spring;
	numSprings++;
}

int MassSpringSystemSimulator::getNumberOfMassPoints() {
	return numPoints;
}

int MassSpringSystemSimulator::getNumberOfSprings() {
	return numSprings;
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) {
	return points[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) {
	return points[index].velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force) {
	m_externalForce = force;
}


