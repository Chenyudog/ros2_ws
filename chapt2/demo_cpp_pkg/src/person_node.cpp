#include "rclcpp/rclcpp.hpp"

class PersonNode : public rclcpp::Node
{
    private:
        std::string name;
        int age;

    public:
        PersonNode(const std::string &node_name):Node(node_name)
        {
            
        }
}

int main(int argc,char** argv)
{
    rclcpp::init(argc,argv);
    auto node =std::make_shared<rclcpp::Node>"person_node";
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;    
}