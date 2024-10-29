#include <unistd.h>
#include "rclcpp/rclcpp.hpp"
#include "autodock_msgs/srv/docking.hpp"
#include "autodock_msgs/msg/current_state.hpp"
#include <behaviortree_cpp_v3/action_node.h>
std::string docking_state, action_state;

void DockingStateCallback(const autodock_msgs::msg::CurrentState::SharedPtr msg)
{
    docking_state = msg->docking_state;
    action_state = msg->action_state;
}

class AutodockClient: public BT::SyncActionNode
{
public:
    AutodockClient(const std::string& name, const BT::NodeConfiguration& config)
        : BT::SyncActionNode(name, config)
    {
        node_ = rclcpp::Node::make_shared("AutodockClient");
        client_ = node_->create_client < autodock_msgs::srv::Docking > ("autodock_controller/docking_service");
        sub_ = node_->create_subscription < autodock_msgs::msg::CurrentState > (
            "/autodock_controller/current_state", 1000, &DockingStateCallback);
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort < std::string > ("action") };
    }

    virtual BT::NodeStatus tick() override
    {
        std::string desired_action;

        if (!getInput < std::string > ("action", desired_action)) {
            throw BT::RuntimeError("missing required input [action]");
        }

        // Waiting for service /save
        while (!client_->wait_for_service(std::chrono::seconds(1))) {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(node_->get_logger(), "Interrupted while waiting for service /autodock_controller/docking_service.");
                return BT::NodeStatus::FAILURE;
            }
            RCLCPP_INFO(node_->get_logger(), "Waiting for service /autodock_controller/docking_service to appear...");
        }
        sleep(3); // To stablize the camera
        auto request = std::make_shared < autodock_msgs::srv::Docking::Request > ();
        request->service = desired_action;
        auto result_future = client_->async_send_request(request);

        if (rclcpp::spin_until_future_complete(node_, result_future) != rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_ERROR(node_->get_logger(), "Unable to call /autodock_controller/docking_service");
            return BT::NodeStatus::FAILURE;
        } else if (!result_future.get()->service_success) {
            RCLCPP_ERROR(node_->get_logger(), "autodock_controller docking_service failed.");
            return BT::NodeStatus::FAILURE;
        }

        RCLCPP_INFO(node_->get_logger(), "Waiting for result of apriltag docking...");

        while (docking_state != "docked" || action_state != "") {
            rclcpp::spin_some(node_);
        }
        return BT::NodeStatus::SUCCESS;
    }

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Client < autodock_msgs::srv::Docking > ::SharedPtr client_;
    rclcpp::Subscription < autodock_msgs::msg::CurrentState > ::SharedPtr sub_;
};
