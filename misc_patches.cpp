#include <set>
#include "WPILib.h"
// work around https://usfirst.collab.net/sf/go/artf4798
// if this fails, comment out this line
std::set<Ultrasonic*> Ultrasonic::m_sensors;
