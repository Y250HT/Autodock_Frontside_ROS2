#include <rclcpp/rclcpp.hpp>
#include "autodock_msgs/srv/docking.hpp"
#include "autodock_msgs/msg/current_state.hpp"

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("docking_client");
  auto client = node->create_client<autodock_msgs::srv::Docking>("autodock_controller/docking_service");

  while (!client->wait_for_service(std::chrono::seconds(1)))
  {
    if (!rclcpp::ok())
    {
      RCLCPP_INFO(node->get_logger(), "docking_client was interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(node->get_logger(), "Service not available, waiting again...");
  }

  std::string docking_ = node->declare_parameter<std::string>("docking", "start");

  auto request = std::make_shared<autodock_msgs::srv::Docking::Request>();
  request->service = docking_;

  auto future_result = client->async_send_request(request);

  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, future_result) ==
      rclcpp::FutureReturnCode::SUCCESS)
  {
    if (future_result.get()->service_success)
      RCLCPP_INFO(node->get_logger(), "Send service request(docking:=%s) successfully.", docking_.c_str());
    else
      RCLCPP_ERROR(node->get_logger(), "Send service request(docking:=%s) failed.", docking_.c_str());
  }
  else
  {
    RCLCPP_ERROR(node->get_logger(), "docking_client was interrupted. Exiting.");
  }

  rclcpp::shutdown();
  return 0;
}

