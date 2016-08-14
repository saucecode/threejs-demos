#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <iomanip>

using namespace std;

typedef struct {
	// meters
	double axleLength;
	double wheelDiameter;
	double pulsesPerRevolution;
	double x;
	double y;
	double distancePerPulse;
	// radians
	double heading;
} robot_t;

bool takeUserInput(robot_t& robot);
void moveRobot(robot_t& robot, int leftMotorTicks, int rightMotorTicks);
void printRobot(robot_t robot);

int main(){
	
	cout << "ROBOT POSITION CALCULATOR" << endl;
	cout << "Please enter inputs in centremeters and degrees." << endl;
	robot_t robot = {0.05, 0.036, 45, 0, 0, 0, 0};
	// Distance traveled in meters per pulse
	robot.distancePerPulse = M_PI * robot.wheelDiameter / robot.pulsesPerRevolution;
	
	cout << "Enter the initial robot X coordinate: ";
	cin >> robot.x;
	cout << "Enter the initial robot Y coordinate: ";
	cin >> robot.y;
	cout << "Enter the initial robot heading: ";
	cin >> robot.heading;
	
	// Convert to meters & radians.
	robot.heading *= M_PI / 180.0;
	robot.x /= 100.0;
	robot.y /= 100.0;
	
	printRobot(robot);

	while( takeUserInput(robot) );
	
	cout << "END OF PROGRAM" << endl;
	return 0;
}

bool takeUserInput(robot_t& robot){
	int leftWheelPulses, rightWheelPulses;
	
	cout << "Enter the number of left wheel pulses: ";
	cin >> leftWheelPulses;
	if( leftWheelPulses == -1 ) return false;
	cout << "Enter the number of right wheel pulses: ";
	cin >> rightWheelPulses;
	
	moveRobot(robot, leftWheelPulses, rightWheelPulses);
	printRobot(robot);
	
	return true;
}

void moveRobot(robot_t& robot, int leftMotorPulses, int rightMotorPulses){
	if( leftMotorPulses == rightMotorPulses ){
		double dx = cos(robot.heading) * leftMotorPulses * robot.distancePerPulse;
		double dy = sin(robot.heading) * leftMotorPulses * robot.distancePerPulse;
		robot.x += dx;
		robot.y += dy;
		
	}else{
		double radiusCurvature = 0.5 * robot.axleLength * (leftMotorPulses + rightMotorPulses) / (double) (rightMotorPulses - leftMotorPulses);
		double dHeading = (rightMotorPulses - leftMotorPulses) * robot.distancePerPulse / robot.axleLength;
		double dx = radiusCurvature * (sin(robot.heading + dHeading) - sin(robot.heading));
		double dy = radiusCurvature * (cos(robot.heading) - cos(robot.heading + dHeading));
		robot.x += dx;
		robot.y += dy;
		robot.heading += dHeading;
		
	}
}

void printRobot(robot_t robot){
	cout << fixed << setprecision(2);
	cout << robot.x*100 << "cm " << robot.y*100 << "cm " << robot.heading*180.0/M_PI << "deg" << endl;
}
