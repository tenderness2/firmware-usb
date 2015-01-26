#ifndef __TRAFFIC_H__
#define __TRAFFIC_H__

using namespace std;

class TrafficData {
	public :
		TrafficData(){
			cout << "TrafficData init" << endl;
		}
		void ScreenTest(uint32_t time){
				cout << "this is TrafficData test" << endl;
				time = 0;
		}
		~TrafficData() {
			cout << "TrafficData exit " << endl;
		}
	private :

};

#endif //__TRAFFIC_H__
