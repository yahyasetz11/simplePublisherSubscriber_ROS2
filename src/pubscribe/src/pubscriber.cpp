#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class Pubscriber : public rclcpp::Node
{
public:
  Pubscriber() : Node("minimal_pubscriber"),  count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    subscriber_ = this->create_subscription<std_msgs::msg::String>("topic", 10, std::bind(&Pubscriber::subscriber_callback, this, std::placeholders::_1));
    timer_ = this->create_wall_timer(1000ms, std::bind(&Pubscriber::timer_callback, this));
  }

private:
  void subscriber_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    subs_ans = pow(std::stoi(msg->data.c_str()),2);
    RCLCPP_INFO(this->get_logger(), "Received message: '%i'", subs_ans);
  }
  void timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing Quadratic: '%s'", message.data.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
    size_t subs_ans;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Pubscriber>());
  rclcpp::shutdown();
  return 0;
}