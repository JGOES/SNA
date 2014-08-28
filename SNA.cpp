#include "SNA.hpp"

SNA::SNA() {}
SNA::~SNA() {}

//!< main application loop.
void SNA::start() {
	// application loop
	for(;;) {

		// read input from command line, fill in data members, find type of op
		getInput();
		processInput();
		clearInput();
	}
}

//! Get input from console.
/*!	Pre: input commands having the following format
 		username -> message
		username
		username follows username
		username wall

	Post: fill data members (username, op, optional, current_operation) 
		  with input
*/
void SNA::getInput() {
	std::cout << "> ";

	// read in entire command from user
	getline(std::cin, input);

	// extract individual pieces of data
	std::istringstream tokens(input);
	tokens >> username >> op;
	getline(tokens >> std::ws, optional);	// skip first whitespace character

	// set the current_operation as one of 4 pre-defined types
	if(op.compare("->") == 0)
		current_operation = Operations::POSTING;
	if(op.compare("follows") == 0)
		current_operation = Operations::FOLLOWING;
	if(op.compare("wall") == 0)
		current_operation = Operations::WALL;
	if(op.compare("") == 0)
		current_operation = Operations::READING;
}

//! Find user in User list.
/*!	
	\param username name of the user.
	\return index of user in User, -1 if not found
*/
int SNA::findUser(std::string username) {
	for(int i = 0; i < Users.size(); i++)
		if(Users[i].name == username)
			return i;

	return -1;
}

//! Process input 
/*!	
	Performs a specific operation depending on current_operation
*/
void SNA::processInput() {
	if(current_operation == Operations::POSTING)  {
		//std::cout << "#new message" << std::endl;
		// check if user already exists
		int userId = findUser(username);

		// if user does not exist then create user
		if(userId == -1) {
			//std::cout << "new user\n";
			User u;
			u.name = username;
			u.message_log.push_back({optional, std::chrono::system_clock::now()});
			Users.push_back(u);
		} else {	// add message to user message log
			Users[userId].message_log.push_back({optional, std::chrono::system_clock::now()});
		}
	}

	if(current_operation == Operations::FOLLOWING)  {
		//std::cout << "#follow" << std::endl;
		int userId = findUser(username);
		Users[userId].following.push_back(optional);
	}

	if(current_operation == Operations::WALL)  {
		//std::cout << "#wall" << std::endl;
		int userId = findUser(username);

		if(userId != -1) {
			/////// user's own messages
			auto end = std::chrono::system_clock::now();
		
			for(int i = Users[userId].message_log.size()-1; i >= 0; i--) {
				auto time = std::chrono::duration_cast<std::chrono::seconds>(end-Users[userId].message_log[i].time).count();

				if(time < 60)
					std::cout << username << " - " << Users[userId].message_log[i].msg << " (" 
						<< time << " seconds ago)" << std::endl;
				else {
					auto time_in_minutes = time/60;
					std::cout << username << " - " << Users[userId].message_log[i].msg << " (" 
						<< time_in_minutes << " minute" << (time_in_minutes == 1 ? "" : "s") << " ago)" << std::endl;
				}
			}
			///////

			/////// all follower's messages
			for(int j = Users[userId].following.size()-1; j >= 0 ; j--) {
				//std::cout << "following " << Users[userId].following[i] << std::endl;
				int followingUserId = findUser(Users[userId].following[j]);
				//std::cout << followingUserId << std::endl;

				for(int i = Users[followingUserId].message_log.size()-1; i >= 0 ; i--) {
					auto time = std::chrono::duration_cast<std::chrono::seconds>(end-Users[followingUserId].message_log[i].time).count();

					if(time < 60)
						std::cout << Users[userId].following[j] << " - " << Users[followingUserId].message_log[i].msg << " (" 
							<< time << " seconds ago)" << std::endl;
					else {
						auto time_in_minutes = time/60;
						std::cout << Users[userId].following[j] << " - " << Users[followingUserId].message_log[i].msg << " (" 
							<< time_in_minutes << " minute" << (time_in_minutes == 1 ? "" : "s") << " ago)" << std::endl;
					}
				}

			}
			///////
		}
	}

	if(current_operation == Operations::READING)  {
		//std::cout << "#read" << std::endl;
		int userId = findUser(username);
		if(userId == -1) {
			//std::cout << "new user\n";
			User u;
			u.name = username;
			Users.push_back(u);
		} else {
			auto end = std::chrono::system_clock::now();
			
			for(int i = 0; i < Users[userId].message_log.size(); i++) {
				auto time = std::chrono::duration_cast<std::chrono::seconds>(end-Users[userId].message_log[i].time).count();

				if(time < 60)
					std::cout << Users[userId].message_log[i].msg << " (" 
						<< time << " seconds ago)" << std::endl;
				else {
					auto time_in_minutes = time/60;
					std::cout << Users[userId].message_log[i].msg << " (" 
						<< time_in_minutes << " minute" << (time_in_minutes == 1 ? "" : "s") << " ago)" << std::endl;
				}
			}
		}
	}

}

//! Clear input 
/*!	
	Clears the data members for next command
*/
void SNA::clearInput() {
	username = "";
	op = "";
	optional = "";
}

