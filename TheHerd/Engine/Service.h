//
//  Service.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_Service_h
#define Testing_Service_h

#include <string>

namespace Engine {
	class Clock;
	
	class Service {
	public:
		Service() {}
		virtual ~Service() {}
		
		virtual void startup() = 0;
		virtual void shutdown() = 0;
		
		virtual void tick() {}
		
		void setName(const std::string &name) {
			m_name = name;
		}
		const std::string &name() const {
			return m_name;
		}
	private:
		std::string m_name;
	};
}

#endif
