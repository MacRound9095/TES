#include <sensor_msgs/Imu.h>
#include <cmath>


//可以在Estimator里声明这个类
class detect(){
// Global variables to track frame drop statistics
int total_seq_diff = 0;
double total_time_diff = 0.0;
double total_displacement_norm = 0.0;
double total_rotation_norm = 0.0;
double global_threshold=0.0;//动态阈值系数，当且仅当

//手动在failuredetection处setzero
void setzero(){
    total_seq_diff = 0;
    total_time_diff = 0.0;
    total_displacement_norm = 0.0;
    total_rotation_norm = 0.0;
}

bool detect_frame_drop(const sensor_msgs::Imu& prev_msg, const sensor_msgs::Imu& curr_msg,bool initialized) {
    if (initialized==false)
        return false;
    int seq_diff = curr_msg.header.seq - prev_msg.header.seq;
    // 判断当前序列是否连续(已弃用，手动setzero)
    // if(seq_diff==1){
    //     std::cout << "Frame drop detected!" << std::endl;
    //      total_seq_diff = 0;
    //      total_time_diff = 0.0;
    //      total_displacement_norm = 0.0;
    //      total_rotation_norm = 0.0;
    // }
        
    double time_diff = (curr_msg.header.stamp - prev_msg.header.stamp).toSec();
    Eigen::Vector3d cur_position(curr_msg.linear_acceleration.x,curr_msg.linear_acceleration.y,curr_msg.linear_acceleration.z);
    Eigen::Vector3d prev_position(prev_msg.linear_acceleration.x,prev_msg.linear_acceleration.y,prev_msg.linear_acceleration.z);

    // Calculate displacement norm
    // double displacement_norm = std::sqrt(
    //     std::pow(curr_msg.linear_acceleration.x - prev_msg.linear_acceleration.x, 2) +
    //     std::pow(curr_msg.linear_acceleration.y - prev_msg.linear_acceleration.y, 2) +
    //     std::pow(curr_msg.linear_acceleration.z - prev_msg.linear_acceleration.z, 2)
    // );
    double displacement_norm=(cur_position-prev_position).norm();


    // Calculate rotation norm (using quaternions)
    double rotation_norm = std::sqrt(
        std::pow(curr_msg.orientation.x - prev_msg.orientation.x, 2) +
        std::pow(curr_msg.orientation.y - prev_msg.orientation.y, 2) +
        std::pow(curr_msg.orientation.z - prev_msg.orientation.z, 2) +
        std::pow(curr_msg.orientation.w - prev_msg.orientation.w, 2)
    );

    // Update global variables
    total_seq_diff += seq_diff;
    total_time_diff += time_diff;
    total_displacement_norm += displacement_norm;
    total_rotation_norm += rotation_norm;

    // Criteria for detecting a frame drop
    bool is_frame_dropped = (seq_diff > 1 || time_diff > 0.1); // Example thresholds
    global_threshold=5.0/displacement_norm;


    // Output current stats
    if (is_frame_dropped) {
        ROS_INFO("Frame drop detected!");
        ROS_INFO("Seq diff: %d, Time diff: %f seconds", seq_diff, time_diff.toSec());
        ROS_INFO("Displacement norm: %f, Rotation norm: %f", displacement_norm, rotation_norm);
        ROS_INFO("Total Seq diff: %d, Total Time diff: %f seconds", total_seq_diff, total_time_diff.toSec());
        ROS_INFO("Total Displacement norm: %f, Total Rotation norm: %f", total_displacement_norm, total_rotation_norm);
        ROS_INFO("dynamic_threshold:%f,and total=:%f", global_threshold,displacement_norm*global_threshold);
    }

    return is_frame_dropped;
}

}
