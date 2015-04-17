
#include <vector>
#include <string>
#include <boost/program_options.hpp>

namespace feature {

	namespace po = boost::program_options;

	struct device_feature {
		device_feature(){ hid::init(); };
		void help(po::options_description &desc) {
			std::cout << desc << std::endl;
		}

		void version() {
			std::cout << "program version is 0.1.0" << std::endl;
		}

		void list_usb() {
			wire::device_info_list list;
			list = wire::enumerate_connected_devices();
			if(list.size())
			{
				std::vector<wire::device_info>::iterator iter;
				for(iter = list.begin(); iter != list.end(); iter++) {
					std::cout << "path : " << iter->path << std::endl;  
					std::wcout << "serial number: " <<  iter->serial_number << std::endl;   
				}

			}
			else
			{
				LOG(INFO) << "no device found";

			}

		}

		void device_path(std::string path) {
			//std::cout << "path : " << path << std::endl;
			std::unique_ptr<wire::device_kernel> device(new wire::device_kernel(path.c_str())); 
			device->open();
			std::cout << "is opeinged !" << std::endl;
		}

		void test_screen(int time) {
			
		}

		~device_feature(){ hid::exit(); };

		private :
			
	};
}
