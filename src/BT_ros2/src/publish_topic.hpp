#pragma once

#include <chrono>
#include <thread>
#include <rclcpp/rclcpp.hpp>
#include <behaviortree_cpp_v3/action_node.h>

class PublishTopic : public BT::SyncActionNode
{
public:
    PublishTopic(const std::string& name, const BT::NodeConfiguration& config)
        : BT::SyncActionNode(name, config)
    {
        node_ = rclcpp::Node::make_shared("lift_control");
        command_ = "ros2 topic pub /lift_control std_msgs/msg/Int32 \"{data: 1}\" --rate 10 --times 20";

    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("command") };
    }

    virtual BT::NodeStatus tick() override
    {
        std::string custom_command;
        if (getInput<std::string>("command", custom_command)) {
            command_ = custom_command;
        }

        RCLCPP_INFO(node_->get_logger(), "Executing command: %s", command_.c_str());

        int result = std::system(command_.c_str());

        if (result == 0) {
            RCLCPP_INFO(node_->get_logger(), "publish topic started successfully.");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            return BT::NodeStatus::SUCCESS;
        } else {
            RCLCPP_ERROR(node_->get_logger(), "Failed to start publish topic: %s", command_.c_str());
            return BT::NodeStatus::FAILURE;
        }
    }

private:
    rclcpp::Node::SharedPtr node_;
    std::string command_;
};

