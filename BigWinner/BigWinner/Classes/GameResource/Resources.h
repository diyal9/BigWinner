
#pragma once

#include <list>
#include <map>
#include <vector>

#include "SqliteDataProvider.h"
#include "cocos2d.h"
 
/**
 * The high level interface to the database. Through the storage you can access
 * all characters, guilds, worlds states, transactions, etc.
 */
class Resource
{
    public:
        ~Resource();
	
		static Resource * sharedResource();

        void open();
        void close();

		std::string getDbVersion();
		
private:
        
		Resource();
        Resource(const Resource &rhs);
        Resource &operator=(const Resource &rhs);

        SqLiteDataProvider *mDb; /**< the data provider */
		//

};


