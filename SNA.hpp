#ifndef _SNA_HPP
#define _SNA_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>

//!  Message. 
/*!
  Message structure holds individual message and time point.
*/
struct Message {
	std::string msg;		/*!< msg holds the message */
	std::chrono::time_point<std::chrono::system_clock> time;	/*!< time holds time point when message was posted */
};

//!  User. 
/*!
  User structure holds name of user, a list of other users the user is following,
  and a list of messages that the user posted
*/
struct User {
	std::string name;		/*!< name holds the name of user */
	std::vector<std::string> following;	/*!< following holds the list of users being followed */
	std::vector<Message> message_log; /*!< message_log holds all messages posted by user */
};

//!  Social Networking Application (SNA) class. 
/*!
  This class handles the input, process and output of data via a console
*/
class SNA {
public:
	SNA();
	~SNA();

	//!< main application loop.
	void start();

	//!< gets input from console and sets current_operation.
	void getInput();

	//!< searches through the list of existing users, returns index of user in Users, -1 otherwise.
	int findUser(std::string username);
	
	//!< performs a specific operation depending on current_operation
	void processInput();

	//!< clears the data members for next command
	void clearInput();
private:
	std::vector<User> Users;
	enum class Operations {POSTING,READING,FOLLOWING,WALL};
	Operations current_operation;
	std::string input;
	std::string username;
	std::string op;
	std::string optional;
};

#endif
