//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Copyright (c) 2019, Alex Mous
//Licensed under the CC BY-NC SA 4.0

//Example code
#include <mpu6050/MPU6050.h>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include <math.h>
#include "rclcpp/rclcpp.hpp"

MPU6050 device(0x68);

int main(int argc, char * argv[]) {
	float ax, ay, az, gr, gp, gy; //Variables to store the accel, gyro and angle values

	sleep(1); //Wait for the MPU6050 to stabilize


	//Calculate the offsets
	std::cout << "Calculating the offsets...\n    Please keep the accelerometer level and still\n    This could take a couple of minutes...";
	device.getOffsets(&ax, &ay, &az, &gr, &gp, &gy);
	std::cout << "Gyroscope R,P,Y: " << gr << "," << gp << "," << gy << "\nAccelerometer X,Y,Z: " << ax << "," << ay << "," << az << "\n";


	//Read the current yaw angle
	device.calc_yaw = true;

	// Initialize ROS 2
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("transform_publisher");

    // Create a TransformStamped message
    geometry_msgs::msg::TransformStamped transform_stamped;

    // Create a TransformBroadcaster
    tf2_ros::TransformBroadcaster broadcaster(node);

    // Set the frame IDs
    transform_stamped.header.frame_id = "base_frame";
    transform_stamped.child_frame_id = "child_frame";

    // Main loop
    rclcpp::Rate loop_rate(1); // 1 Hz

	// for (int i = 0; i < 40; i++) {
	while (rclcpp::ok()){
		device.getAngle(0, &gr);
		device.getAngle(1, &gp);
		device.getAngle(2, &gy);
		std::cout << "Current angle around the roll axis: " << gr << "\n";
		std::cout << "Current angle around the pitch axis: " << gp << "\n";
		std::cout << "Current angle around the yaw axis: " << gy << "\n";
		// usleep(250000); //0.25sec
		// Update quaternion from Euler angles
        tf2::Quaternion quaternion;
        quaternion.setRPY(gr/M_PI, gp/M_PI, gy/M_PI); // Replace with your desired Euler angles

        // Set the quaternion and timestamp in the TransformStamped message
        transform_stamped.transform.rotation.x = quaternion.x();
        transform_stamped.transform.rotation.y = quaternion.y();
        transform_stamped.transform.rotation.z = quaternion.z();
        transform_stamped.transform.rotation.w = quaternion.w();
        transform_stamped.header.stamp = node->now();

        // Broadcast the transform
        broadcaster.sendTransform(transform_stamped);

        // Spin and sleep
        rclcpp::spin_some(node);
        loop_rate.sleep();
	}
	rclcpp::shutdown();
	//Get the current accelerometer values
	device.getAccel(&ax, &ay, &az);
	std::cout << "Accelerometer Readings: X: " << ax << ", Y: " << ay << ", Z: " << az << "\n";

	//Get the current gyroscope values
	device.getGyro(&gr, &gp, &gy);
	std::cout << "Gyroscope Readings: X: " << gr << ", Y: " << gp << ", Z: " << gy << "\n";

	return 0;
}


