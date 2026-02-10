#include"rclcpp/rclcpp.hpp"
#include"geometry_msgs/msg/twist.hpp"
#include"chrono"
#include"turtlesim/msg/pose.hpp"
#include <cmath>
using namespace std::chrono_literals;
double target_x{1.0};
double target_y{2.0};
double k=3.0;

class TurtleControl: public rclcpp::Node
{
private:
    // rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriber_;
public:
    explicit TurtleControl(const std::string&node_name):Node(node_name)
    {
        publisher_=this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel",10);
        // timer_=this->create_wall_timer(1000ms,std::bind(&TurtleControl::timer_callback,this));
        subscriber_=this->create_subscription<turtlesim::msg::Pose>(
            "/turtle1/pose",
            10,
            std::bind((&TurtleControl::callback),this,std::placeholders::_1));
    }   

    // void timer_callback()
    // {
    //     auto msg=geometry_msgs::msg::Twist();
    //     msg.linear.x=1.0;
    //     msg.angular.z=1.0;
    //     publisher_->publish(msg);
    // }

    void callback(const turtlesim::msg::Pose::SharedPtr pose)
    {
        //1.获取当前位置
        auto current_x=pose->x;
        auto current_y=pose->y;
        RCLCPP_INFO(get_logger(),"当前位置为x=%f,y=%f",current_x,current_y);
        //2.计算距离
        auto distance=std::sqrt((target_x-current_x)*(target_x-current_x)+(target_y-current_y)*(target_y-current_y));
        auto angle=std::atan2(((target_y-current_y)),(target_x-current_x))-pose->theta;
        //3.控制速度
        auto msg=geometry_msgs::msg::Twist();
        if (distance>0.1)
        {
            if (fabs(angle)>0.2)
            {
                msg.angular.z=k*(target_y-current_y);
            }else
            {
                msg.linear.x=k*distance;
            }
        }

        publisher_->publish(msg);
    }
};

int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    auto node= std::make_shared<TurtleControl>("turtle_control");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}