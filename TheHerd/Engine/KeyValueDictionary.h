/*
 *  KeyValueDictionary.h
 *  KeyValueDictionary
 *
 *  Created by Martin Linklater on 20/09/2009.
 *  Copyright 2009 Martin Linklater. All rights reserved.
 *
 */

#ifndef KEYVALUEDICTIONARY_H
#define KEYVALUEDICTIONARY_H

/*
 
 Build notes:
 
 The default behaviour of KeyValueDictionary is to fail gracefully on error and
 return the calling code an error enum. If you set 'KEYVALUEDICTIONARY_ASSERTONERROR'
 as a precompiler definition KeyValueDictionary will call 'assert()' on all errors.
 If you set 'KEYVALUEDICTIONARY_THROWONERROR' KeyValueDictionary will throw an
 XXXXX excpetion on all errors.
 
 The default behaviour is also to not be thread safe. If you require thread safety
 set 'KEYVALUEDICTIONARY_THREADSAFE' as a preprocessor definition. This will use
 pthreads to implement thread safety. You could replace pthreads with your own
 mechanism if you so wish.

 TODO
 
	exceptions
	asserts
	thread safety
	unit test
 
 */

// STL includes

#include <map>
#include <list>
#include <string>

namespace Engine {
	class KeyValueDictionary {
	public:
		KeyValueDictionary();
		~KeyValueDictionary();

		// Return values

		enum eReturnValue {
			kOK = 0,
			kExists,
			kNotExists,
			kErrorLocked,
			kErrorKeyNotPresent,
			kErrorNotificationNotFound,
			kErrorDuplicateNotification,
			kErrorInternal
		};

		// Type for notification callbacks
		
		typedef	void(^pNotification)( void* pNewValue );

		// Clear all internals and reset to initialized state
		
		void Reset( void );

		// Set methods
		
		template<class T> eReturnValue	Set( const std::string &strKey, T newValue )
		{
			unsigned int hashKey = Hash( strKey );

			// add - key, value, length
			if( Exists( hashKey ) == kExists )
			{
				// existing key - only update if it is new
				
				const void* pOldValue = m_map[ hashKey ].value.data();
				if( memcmp( pOldValue, &newValue, sizeof(T)) != 0 )
				{
					// check this new value isn't currently locked

					if( m_map[ hashKey ].lock )
						return kErrorLocked;
					
					// value has changed, so assign the new value
					
					m_map[ hashKey ].value.assign( (char*)&newValue, sizeof(T) );
									
					// lock
					m_map[ hashKey ].lock = true;
					
					// send notifications
					
					NotificationList &noteList = m_map[ hashKey ].notifications;
					for( NotificationListIter iter = noteList.begin() ; iter != noteList.end() ; ++iter )
					{
						(iter->notification)( &newValue );
					}
					
					// unlock
					m_map[ hashKey ].lock = false;
				}
			}
			else 
			{
				// new entry
				
				Value value;
				value.value.assign( (char*)&newValue, sizeof(T) );	// get the value as data
				value.notifications.clear();
				value.lock = false;
				
				m_map[ hashKey ] = value;
			}
			
			return kOK;
		}

		// Get methods
		
		template<class T> eReturnValue	Get( const std::string &strKey, T* pOutput )
		{
			unsigned int hashKey = Hash( strKey );
			
			if( Exists( hashKey ) != kExists )
				return kErrorKeyNotPresent;

			memcpy( pOutput, m_map[ hashKey ].value.data(), sizeof(T) );

			return kOK;
		}

		// Exists
		
		eReturnValue	Exists( const std::string &strKey )
		{
			return Exists( Hash( strKey ) );
		}

		// Delete

		eReturnValue	Delete( const std::string &strKey );
		
		// Notifications

		struct Notification {
			~Notification();
			pNotification	notification;
		};
		
		typedef std::list< Notification >	NotificationList;
		typedef NotificationList::iterator	NotificationListIter;

		eReturnValue	AddNotification( pNotification func, const std::string &strKey, NotificationListIter *handle = NULL );
		eReturnValue	RemoveNotification( NotificationListIter handle, const std::string &strKey );
		
	private:
		
		// Main store
		
		struct Value {
			std::string			value;
			NotificationList	notifications;
			bool				lock;	// Stops value being set whilst processing
										// its own notifications.
		};

		typedef std::pair< unsigned int, Value > KeyValuePair;
		
		typedef std::map< unsigned int, Value, std::less< unsigned int > > StoreType;
		typedef StoreType::iterator StoreTypeIter;

		StoreType		m_map;
		
		eReturnValue	Exists( unsigned int hashKey );
		
		unsigned int	Hash( const std::string &stringIn );
	};
}

#endif // KEYVALUEDICTIONARY_H
