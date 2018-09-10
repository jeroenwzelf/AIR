#pragma once

class global_info {
	public:
		void setIsDebugging(bool b) { DEBUG = b; }
		bool getIsDebugging() { return DEBUG; }

		void setDiscordEnabled(bool b) { DISCORDENABLED = b; }
		bool getDiscordEnabled() { return DISCORDENABLED; }

		static global_info& getInstance() {
			static global_info instance;
			return instance;
		}
		
		global_info(global_info const&) = delete;
		void operator=(global_info const&) = delete;
	private:
		global_info() { DEBUG = false; DISCORDENABLED = false; }
		bool DEBUG;
		bool DISCORDENABLED;
};