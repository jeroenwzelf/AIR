#pragma once

class global_info {
	public:
		void setIsDebugging(bool b) { DEBUG = b; }
		bool getIsDebugging() { return DEBUG; }

		void setIsOnline(bool b) { ISONLINE = b; }
		bool getIsOnline() { return ISONLINE; }

		static global_info& getInstance() {
			static global_info instance;
			return instance;
		}
		
		global_info(global_info const&) = delete;
		void operator=(global_info const&) = delete;
	private:
		global_info() { DEBUG = false; ISONLINE = false; }
		bool DEBUG;
		bool ISONLINE;
};