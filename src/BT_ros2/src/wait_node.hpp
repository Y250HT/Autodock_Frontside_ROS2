#pragma once

#include <chrono>
#include <thread>
#include <behaviortree_cpp_v3/action_node.h>

class WaitNode : public BT::SyncActionNode
{
public:
    WaitNode(const std::string& name, const BT::NodeConfiguration& config)
        : BT::SyncActionNode(name, config)
    {
        wait_time_ = 5;  // 默认等待时间为1秒
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<int>("duration") };
    }

    virtual BT::NodeStatus tick() override
    {
        int duration;
        if (getInput<int>("duration", duration))
        {
            wait_time_ = duration;
        }

        std::this_thread::sleep_for(std::chrono::seconds(wait_time_));
        return BT::NodeStatus::SUCCESS;
    }

private:
    int wait_time_;
};

