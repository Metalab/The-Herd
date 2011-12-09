/*
 *  KeyValueDictionary.cpp
 *  KeyValueDictionary
 *
 *  Created by Martin Linklater on 20/09/2009.
 *  Copyright 2009 Martin Linklater. All rights reserved.
 *
 */

#include "KeyValueDictionary.h"
#include <Block.h>

namespace Engine {
	//------------------------------------------------------------------------------

	KeyValueDictionary::KeyValueDictionary()
	{
		Reset();
	}

	//------------------------------------------------------------------------------

	KeyValueDictionary::~KeyValueDictionary()
	{
	}

	//------------------------------------------------------------------------------

	void
	KeyValueDictionary::Reset( void )
	{
		m_map.clear();
	}

	//------------------------------------------------------------------------------

	KeyValueDictionary::eReturnValue
	KeyValueDictionary::Exists( unsigned int hashKey )
	{
		if( m_map.find( hashKey ) == m_map.end() )
			return kNotExists;
		else 
			return kExists;
	}

	//------------------------------------------------------------------------------

	KeyValueDictionary::eReturnValue
	KeyValueDictionary::Delete( const std::string &strKey )
	{
		unsigned int hashKey = Hash( strKey );
		
		if( Exists( hashKey ) == kNotExists )
			return kErrorKeyNotPresent;

		m_map.erase( hashKey );

		// Check the entry really has been deleted
		if( Exists( hashKey ) == kExists )
			return kErrorInternal;

		return kOK;
	}

	//------------------------------------------------------------------------------

	KeyValueDictionary::eReturnValue	
	KeyValueDictionary::AddNotification( pNotification func, const std::string &strKey, void* pUserData, NotificationListIter *handle )
	{
		int hashKey = Hash( strKey );
		
		// check exists

		if( Exists( Hash( strKey ) ) == kErrorKeyNotPresent )
		{
			return kErrorKeyNotPresent;
		}
		
		// TODO - check for duplicated callback

		NotificationList &noteList = m_map[ hashKey ].notifications;
		
		for( NotificationListIter iter = noteList.begin() ; iter != noteList.end() ; ++iter )
		{
			if( iter->notification == func )
			{
				return kErrorDuplicateNotification;
			}
		}

		// build notification

		Notification newNote;
		
		newNote.notification = Block_copy(func);
		newNote.pUserData = pUserData;

		// add notification to list
		
		NotificationList &nl = m_map[ hashKey ].notifications;
		nl.push_back( newNote );
		*handle = nl.end();
		--(*handle);

		return kOK;
	}

	//------------------------------------------------------------------------------

	KeyValueDictionary::eReturnValue	
	KeyValueDictionary::RemoveNotification( NotificationListIter handle, const std::string &strKey )
	{
		int hashKey = Hash( strKey );
		
		// check exists
		
		if( Exists( Hash( strKey ) ) == kErrorKeyNotPresent )
			return kErrorKeyNotPresent;

		// go through notifications and delete the one which matches the func
		
		NotificationList &noteList = m_map[ hashKey ].notifications;
		
		noteList.erase(handle);
		return kOK;
	}

	//------------------------------------------------------------------------------

	unsigned int	
	KeyValueDictionary::Hash( const std::string &stringIn )
	{
		// djb2 hashing function ( http://www.cse.yorku.ca/~oz/hash.html )
		
		const char* pChar = stringIn.c_str();
		unsigned int hash = 5381;
		int	c;
		
		while((c = *pChar++))
			hash = ((hash << 5) + hash) + c;
		
		return hash;
	}

	KeyValueDictionary::Notification::~Notification() {
		Block_release(notification);
	}
}
