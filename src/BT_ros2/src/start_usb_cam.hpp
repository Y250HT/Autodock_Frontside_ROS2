#pragma once

#include <chrono>
#include <thread>
#include <rclcpp/rclcpp.hpp>
#include <behaviortree_cpp_v3/action_node.h>

class StartUsbCam : public BT::SyncActionNode
{
public:
    StartUsbCam(const std::string& name, const BT::NodeConfiguration& config)
        : BT::SyncActionNode(name, config)
    {
        node_ = rclcpp::Node::make_shared("start_usb_cam");
        // 定义一个命令来启动USB相机节点
        command_ = "ros2 run ros2_image_saver image_saver";
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

        // 执行命令启动相机
        int result = std::system(command_.c_str());

        if (result == 0) {
            RCLCPP_INFO(node_->get_logger(), "USB camera started successfully.");
            // 停顿一段时间以确保相机稳定
            std::this_thread::sleep_for(std::chrono::seconds(3));
            return BT::NodeStatus::SUCCESS;
        } else {
            RCLCPP_ERROR(node_->get_logger(), "Failed to start USB camera with command: %s", command_.c_str());
            return BT::NodeStatus::FAILURE;
        }
    }

private:
    rclcpp::Node::SharedPtr node_;
    std::string command_;
};

