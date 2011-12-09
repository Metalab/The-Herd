//
//  Clock.h
//  Testing
//
//  Created by Andreas Monitzer on 2011-12-09.
//  Copyright (c) 2011 Andreas Monitzer. All rights reserved.
//

#ifndef Testing_Clock_h
#define Testing_Clock_h

namespace Engine {
	class Clock {
	public:
		Clock(double scale = 1.0) : m_scale(scale), m_offset(0.0), m_lastIncrement(0.0) {}
		
		void setScale(double scale) {
			m_scale = scale;
		}
		double scale() const {
			return m_scale;
		}
		
		void tick(double increment) {
			m_offset += increment * m_scale;
			m_lastIncrement = increment;
		}
		double lastIncrement() const {
			return m_lastIncrement;
		}
		
		double offset() const {
			return m_offset;
		}
	protected:
		double m_scale;
		double m_offset;
		double m_lastIncrement;
	};
}

#endif
