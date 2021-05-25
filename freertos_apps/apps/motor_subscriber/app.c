#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <my_custom_message/msg/my_custom_message.h>
#include <sensor_msgs/msg/joint_state.h>
#include <std_msgs/msg/int16.h>
#include <std_msgs/msg/string.h>
#include <stdio.h>

#include "FreeRTOS.h"

#define ARRAY_LEN 200
#define JOINT_DOUBLE_LEN 20

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return 1;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

rcl_subscription_t joint_states_subscriber;
sensor_msgs__msg__JointState joint_states_msg;

my_custom_message__msg__MyCustomMessage modbus_msg;
rcl_publisher_t modbus_msg_publihser;

rcl_publisher_t string_publisher;
std_msgs__msg__String string_msg;
//std_msgs__msg__Int16 int_msg;

/* motor var here */
//double motor_pos = 0;
//double output_pusle = 0;
//double motor_exp = 0;
//uint32_t loop_pusle = 10000;
/* motor var end */

void timer_callback(rcl_timer_t * timer, int64_t last_call_time){
    RCLC_UNUSED(last_call_time);
    if(timer != NULL){
        char *test_word = "I microros app";
        string_msg.data.data = test_word;

        modbus_msg.percyclepulse = 123;
        modbus_msg.errornumber = 456;
        modbus_msg.temperature = 234;
        modbus_msg.alarm = 567;
        modbus_msg.errorcode = 345;
        rcl_publish(&modbus_msg_publihser, (const void *)&modbus_msg, NULL);
        rcl_publish(&string_publisher, (const void *)&string_msg, NULL);
        printf("test timer callback \r\n");
    }
}

void subscription_joint_state_callback(const void *msgin){
	const sensor_msgs__msg__JointState *msg = (const sensor_msgs__msg__JointState *)msgin;
	printf("I get joint_state topic msg.pos: %d \r\n", (int)(msg->position.data[0] * 100));

	motor_set_axis_exp(1, msg->position.data[0] * 57.3);
	motor_set_axis_exp(2, msg->position.data[1] * 57.3);
	motor_set_axis_exp(3, msg->position.data[1] * 57.3);
	motor_set_axis_exp(4, msg->position.data[1] * 57.3);

//	output_pusle = (motor_exp - (motor_pos / 5000.0 / 120 * 3.14));       
//	if(output_pusle >= 0){
//		X4_MotorDir(0);
//		PWM_LocationSet(output_pusle);	
//		motor_pos += output_pusle;	
//	} else {
//		X4_MotorDir(1);
//		PWM_LocationSet(output_pusle);
//		motor_pos += output_pusle;
//	}
//	printf("output_pusle: %d \r\n", (int)(output_pusle * 100));	
}

int appMain(void *argument)
{

  	rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;

	// create init_options
	RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
	
	// create node
	rcl_node_t node;
	RCCHECK(rclc_node_init_default(&node, "string_node", "", &support));

	// create joint_state subscriber
	RCCHECK(rclc_subscription_init_default(
		&joint_states_subscriber,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
		"/joint_states_low_rate"));
   
    // create string_msg publisher 
    RCCHECK(rclc_publisher_init_best_effort(
        &string_publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        "/test_string"));

    // create modbus_msg publisher
    RCCHECK(rclc_publisher_init_best_effort(
        &modbus_msg_publihser,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(my_custom_message, msg, MyCustomMessage),
        "/A6_modbus_msg"));

    // Create a 3 seconds ping timer timer,
	rcl_timer_t timer;
	RCCHECK(rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(1000), timer_callback));

	// create executor
	rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
	RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
	RCCHECK(rclc_executor_add_subscription(&executor, &joint_states_subscriber, &joint_states_msg, &subscription_joint_state_callback, ON_NEW_DATA));
    RCCHECK(rclc_executor_add_timer(&executor, &timer));

    // string msg init
    char string_test_data[ARRAY_LEN];
    string_msg.data.data = string_test_data;
    string_msg.data.size = 20;
    string_msg.data.capacity = ARRAY_LEN;

    // A6_modbus msg init
    modbus_msg.percyclepulse = 0;
    modbus_msg.errornumber = 0;
    modbus_msg.temperature = 0;
    modbus_msg.alarm = 0;
    modbus_msg.errorcode = 0;

//    modbus_msg.data.size = 8;
//    modbus_msg.data.capacity = 8;

    // Joint states msg init
	char joint_states_msg_buffer[ARRAY_LEN];
	joint_states_msg.header.frame_id.data = joint_states_msg_buffer;
	joint_states_msg.header.frame_id.size = 20;
	joint_states_msg.header.frame_id.capacity = ARRAY_LEN;

	rosidl_runtime_c__String string_buffer[JOINT_DOUBLE_LEN];
	joint_states_msg.name.data = string_buffer;
	joint_states_msg.name.size = 0;
	joint_states_msg.name.capacity = JOINT_DOUBLE_LEN;

	for(int i = 0; i < JOINT_DOUBLE_LEN; i++){
		joint_states_msg.name.data[i].data = (char*) malloc(ARRAY_LEN);
		joint_states_msg.name.data[i].size = 0;
		joint_states_msg.name.data[i].capacity = ARRAY_LEN;
	}

	double joint_states_position_buffer[JOINT_DOUBLE_LEN];
	joint_states_msg.position.data = joint_states_position_buffer;
	joint_states_msg.position.size= 7;
	joint_states_msg.position.capacity = JOINT_DOUBLE_LEN;

	double joint_states_velocity_buffer[JOINT_DOUBLE_LEN];	
	joint_states_msg.velocity.data = joint_states_velocity_buffer;
	joint_states_msg.velocity.size = 7;
	joint_states_msg.velocity.capacity = JOINT_DOUBLE_LEN;
	
	double joint_states_effort_buffer[JOINT_DOUBLE_LEN];	
	joint_states_msg.effort.data = joint_states_effort_buffer;
	joint_states_msg.effort.size = 7;
	joint_states_msg.effort.capacity = JOINT_DOUBLE_LEN;
	
//	rclc_executor_spin(&executor);
	rcl_ret_t ret = 0;
	while(1){
		ret = rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
		usleep(10000);
	}

	RCCHECK(rcl_subscription_fini(&joint_states_subscriber, &node));
	RCCHECK(rcl_node_fini(&node));
}
