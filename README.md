# Initial setup
	1. Setup Ubuntu 22.10 on VirtualBox
	2. sudo apt install git
	3. mkdir ~/projects
	4. cd ~/projects
	5. git clone https://github.com/COVESA/vsomeip
	6. git checkout origin/maintain/3.1
	7. Install following packages
		a. sudo apt install build-essential cmake libboost-all-dev 
	8. Read the documentation in the following link for better understanding. Just read, but follow the steps written below:-
		a. https://github.com/COVESA/vsomeip/wiki/vsomeip-in-10-minutes
		
# Build the vsomeip libraries and install
	1. But follow these steps to build & install the vsomeip libraries
		a. cd vsomeip/
		b. git checkout origin/maintain/3.1 
		c. mkdir build
		d. cd build/
		e. cmake ..
		f. make -j10
		g. sudo make install

# Build the server
	1. cd ../..
	2. mkdir vsomeip-server
	3. Create a file named "service-example.cpp" with following contents:
		#include <vsomeip/vsomeip.hpp>
		
		#define SAMPLE_SERVICE_ID 0x1234
		#define SAMPLE_INSTANCE_ID 0x5678
		
		std::shared_ptr< vsomeip::application > app;
		
		int main() {
		
		    app = vsomeip::runtime::get()->create_application("World");
		    app->init();
		    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
		    app->start();
		}
	4. Create a file named "CMakeLists.txt" with following contents:
		cmake_minimum_required (VERSION 2.8)
		
		set (CMAKE_CXX_FLAGS "-g -std=c++0x")
		
		find_package (vsomeip3 2.6.0 REQUIRED)
		find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )
		
		include_directories (
		    ${Boost_INCLUDE_DIR}
		    ${VSOMEIP_INCLUDE_DIRS}
		)
		
		add_executable(service-example ../service-example.cpp)
		target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})
	5. mkdir build
	6. cd build
	7. cmake ..
	8. export LD_LIBRARY_PATH=../../vsomeip/build/
	9. make
	10. You should successfully build "service-example" Linux executable file

# Build the client
	1. Create new tab in shell (or press Ctrl+Shift+T)
	2. cd ../..
	3. mkdir vsomeip-client
	4. Create a file named "client-example.cpp" with following contents:
		#include <iomanip>
		#include <iostream>
		
		#include <vsomeip/vsomeip.hpp>
		
		#define SAMPLE_SERVICE_ID 0x1234
		#define SAMPLE_INSTANCE_ID 0x5678
		
		std::shared_ptr< vsomeip::application > app;
		
		void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
		    std::cout << "Service ["
		            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
		            << "] is " << (_is_available ? "available." : "NOT available.")  << std::endl;
		}
		
		int main() {
		
		    app = vsomeip::runtime::get()->create_application("Hello");
		    app->init();
		    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
		    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
		    app->start();
		}
	5. Create a file named "CMakeLists.txt" with following contents:
		cmake_minimum_required (VERSION 2.8)
		
		set (CMAKE_CXX_FLAGS "-g -std=c++0x")
		
		find_package (vsomeip3 2.6.0 REQUIRED)
		find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )
		
		include_directories (
		    ${Boost_INCLUDE_DIR}
		    ${VSOMEIP_INCLUDE_DIRS}
		)
		
		add_executable(client-example ../client-example.cpp)
		target_link_libraries(client-example vsomeip3 ${Boost_LIBRARIES})
	6. mkdir build
	7. cd build
	8. cmake ..
	9. export LD_LIBRARY_PATH=../../vsomeip/build/
	10. make
	11. You should successfully build "client-example" Linux executable file


# Make connections
	1. Go to vsomeip-server folder
	2. run ./service-example
	3. Then go to vsomeip-client folder
	4. run ./client-example
	5. On the client terminal you should see following lines:
		[info] Application/Client 101 (Hello) is registered.
		[info] ON_AVAILABLE(0101): [1234.5678:0.0]
		Service [1234.5678] is available.
	6. On the server terminal you should see the following lines:
		[info] Application/Client 0101 is registering.
		[info] Client [100] is connecting to [101] at /tmp/vsomeip-101
		[info] REGISTERED_ACK(0101)
		
	7. After this read the other examples given in https://github.com/COVESA/vsomeip/wiki/vsomeip-in-10-minutes and try to build them, particularly the "Request / Response" example and explain me how the connection works.
		
		
	

