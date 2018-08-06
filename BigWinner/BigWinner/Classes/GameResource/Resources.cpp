#pragma once
#include <cassert>
#include <time.h>
#include <string>
#include "Resources.h"
#include "DBException.h"

//定义随机数 by atom
#include<stdlib.h>
#define Random(x) (rand() % x)

using namespace std;

// defines the supported db version
static const char *SUPPORTED_DB_VERSION = "1";

/*
 * MySQL specificities:
 *     - TINYINT is an integer (1 byte) type defined as an extension to
 *       the SQL standard.
 *     - all integer types can have an optional (non-standard) attribute
 *       UNSIGNED (http://dev.mysql.com/doc/mysql/en/numeric-types.html)
 *
 * SQLite3 specificities:
 *     - any column (but only one for each table) with the exact type of
 *       'INTEGER PRIMARY KEY' is taken as auto-increment.
 *     - the supported data types are: NULL, INTEGER, REAL, TEXT and BLOB
 *       (http://www.sqlite.org/datatype3.html)
 *     - the size of TEXT cannot be set, it is just ignored by the engine.
 *     - IMPORTANT: foreign key constraints are not yet supported
 *       (http://www.sqlite.org/omitted.html). Included in case of future
 *       support.
 *
 * Notes:
 *     - the SQL queries will take advantage of the most appropriate data
 *       types supported by a particular database engine in order to
 *       optimize the server database size.
 *
 * TODO: Fix problem with PostgreSQL null primary key's.
 */

//gaoyuan 修改处
//表名称
static const char* ANTS_VERSION_TBL_NAME = "ants_loadingTips";//loading时的随机字

static Resource * resource = NULL;

Resource * Resource::sharedResource()
{
	if (!resource)
		resource = new Resource();
	return resource;
}

/**
 * Constructor.
 */
Resource::Resource()
        : mDb(new SqLiteDataProvider())
{
}

/**
 * Destructor.
 */
Resource::~Resource()
{
    if (mDb->isConnected())
        close();

    delete mDb;
}

/**
 * Connect to the database and initialize it if necessary.
 */
void Resource::open()
{
    // Do nothing if already connected.
    if (mDb->isConnected())
        return;

    try {
        // open a connection to the database.
        mDb->connect();

        // check database version here
		std::string dbversion = getDbVersion();
        if (dbversion.compare(SUPPORTED_DB_VERSION))
        {
            std::ostringstream errmsg;
            errmsg << "Database version is not supported. " <<
                "Needed version: '" << SUPPORTED_DB_VERSION <<
                "', current version: '" << dbversion << "'";
            throw errmsg.str(); 
        }
    }
    catch (const DbConnectionFailure& e) {
        std::ostringstream errmsg;
        errmsg << "(Resource::open #1) Unable to connect to the database: "
            << e.what();
        throw errmsg.str();
    }
}

/**
 * Disconnect from the database.
 */
void Resource::close()
{
    mDb->disconnect();
}

std::string Resource::getDbVersion()
{
	try
	{
		std::ostringstream query;
		query << "SELECT version "
			<< "  FROM " << ANTS_VERSION_TBL_NAME << ";";

		const RecordSet &info = mDb->execSql(query.str());

		if (!info.isEmpty()) return info(0, 0);
	}
	catch (const DbSqlQueryExecFailure &e)
	{
		std::ostringstream error;
		error << "(Resource::getDbVersion) SQL query failure: " << e.what();
		CCLOGERROR(error.str().c_str());
	}

	return std::string("0");
}
//SBaseAttrEntry Resource::getBaseAttrInfoByJobSex_DB( int job,int sex )
//{
//	string_to<int> toInt;
//	int nCol = 0;
//	SBaseAttrEntry ret;
//	try
//	{
//		std::ostringstream query;
//		query << "SELECT * " << " FROM " << ANTS_BASEATTR_TBL_NAME <<" WHERE jobType = "<< job <<" AND sexType = "<< sex << ";";
//		const RecordSet &info = mDb->execSql(query.str());
//		if (!info.isEmpty()) 
//		{
//			ret.AttrID = toInt(info(0,nCol++));										//属性编号
//			ret.strName = info(0,nCol++);									     //佣兵名称
//			ret.Job = toInt(info(0,nCol++));										//职业
//			ret.Sex = toInt(info(0,nCol++));										//性别
//			ret.Element = toInt(info(0,nCol++));									//五行属性
//			ret.ModeId = toInt(info(0,nCol++));									//模型ID
//			ret.Quality = toInt(info(0,nCol++));									//品质
//			ret.AttrType = toInt(info(0,nCol++));									//主角佣兵区分
//			ret.AttAI = toInt(info(0,nCol++));										//攻击AI
//			ret.DefAI = toInt(info(0,nCol++));											//防御AI
//			ret.HoldAI= toInt(info(0,nCol++));											//待机AI
//			int i= 0;
//			for (;i<MAX_SKILL_MONSTER ;++i)
//			{
//				ret.Skill[i] = toInt(info(0,nCol++));
//			}
//			i = 0;
//			for (; i < 13 ;++i )
//			{
//				for (int j =0;j < 3; ++j)
//				{
//					ret.AttrValue[i][j] = toInt(info(0,nCol++));
//				}			
//			}
//			return ret;
//		}
//	}
//	catch (const DbSqlQueryExecFailure &e) 
//	{
//		std::ostringstream error;
//		error << "(Resource::GetSkill) SQL query failure: " << e.what();
//		CCLOGERROR(error.str().c_str());
//	}
//	return ret;
//}
//
//
//
//bool Resource::getDesitnyInfo(int itemId,std::string &icon,std::string &desc,std::string &name,int &sellPrice)
//{
//	try
//	{
//		std::ostringstream query;
//		query << "SELECT *" << " FROM " << ANTS_ITEMS_TBL_NAME <<" WHERE id = "<< itemId <<";";
//
//		const RecordSet &info = mDb->execSql(query.str());
//		if (!info.isEmpty()) 
//		{
//			
//			string_to<int> toInt;
//			name = info(0,"name");
//			icon = info(0,"icon");
//			desc = info(0,"descrip");
//			sellPrice = toInt(info(0,"buyPrice"));
//			
//			return true;
//		}
//	}
//	catch (const DbSqlQueryExecFailure &e) 
//	{
//		std::ostringstream error;
//		error << "(Resource::getSCommonItemInfo) SQL query failure: " << e.what();
//		CCLOGERROR(error.str().c_str());
//	}
//
//	return true;
//}
//
//
//
