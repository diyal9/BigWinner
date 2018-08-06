//
//  JsonParser.cpp
//  JSONHttp
//
//  Created by 9Leaf on 11/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

/*
#include "JsonParser.h"
using namespace std;

JsonParser::JsonParser()
{
	m_pRoot = NULL;
	m_bReadOnly = false;
	m_bDeleteSelf = false;
}

JsonParser::~JsonParser()
{
	if(m_bDeleteSelf)
	{
		delete m_pRoot;
	}
}

bool JsonParser::IsReadyOnly()
{
	return m_bReadOnly;
}

bool JsonParser::Parse(const char* pJsonContent)
{
	if(!pJsonContent || m_bReadOnly)
	{
		return false;
	}
    
	Json::Reader reader;
	Json::Value  *pRoot = new Json::Value();
    
	if( !reader.parse(string(pJsonContent), (*pRoot), false) )
	{
		delete pRoot;
		return false;
	}
    
	m_pRoot = pRoot;
	m_bReadOnly = true;
	m_bDeleteSelf = true;
	return true;
}

Json::Value* JsonParser::GetChildByName(std::string& strName)
{
	if (strName.empty())
	{
		return NULL;
	}
    
	Json::Value *pChild = (&(*m_pRoot)[strName.c_str()]);
    
	return pChild;
}



JsonParser JsonParser::GetChild(const char* pName)
{
	if(!pName || (m_pRoot == NULL))
	{
		return JsonParser();
	}
    
	Json::Value *pChild = GetChildByName(string(pName));
	return GetReadOnlyReader(pChild);
}

JsonParser JsonParser::GetChild(const int nIndex)
{
	if ((nIndex < 0) || (m_pRoot == NULL))
	{
		return JsonParser();
	}
    
	Json::Value *pChild = GetChildByIndex(nIndex);
	return GetReadOnlyReader(pChild);
}

char* JsonParser::GetString(const char* pName)
{
	if(!pName || (m_pRoot == NULL))
	{
		return NULL;
	}
    
	Json::Value *pChild = GetChildByName(string(pName));
    
	if (pChild == NULL)
	{
		return NULL;
	}
    
    char* pString = new char[pChild->asString().length() + 1];
    strncpy_s(pString,pChild->asString().length() + 1,pChild->asString().c_str(), pChild->asString().length());
	return pString;
}

char* JsonParser::GetString(const int nIndex)
{
	if((nIndex < 0) || (m_pRoot == NULL))
	{
		return NULL;
	}
    
	Json::Value *pChild = GetChildByIndex(nIndex);
    
	if (pChild == NULL)
	{
		return NULL;
	}
    
	char* pString = new char[pChild->asString().length() + 1];
	strncpy_s(pString,pChild->asString().length() + 1,pChild->asString().c_str(), pChild->asString().length());
	return pString;
}

bool JsonParser::GetBool(const char* pName)
{
	if(!pName || (m_pRoot == NULL))
	{
		return false;
	}
    
	Json::Value *pChild = GetChildByName(string(pName));
	if( pChild  == NULL)
	{
		return false;
	}
    
	return pChild->asBool();
}

bool JsonParser::GetBool(const int nIndex)
{
	if((nIndex < 0) || (m_pRoot == NULL))
	{
		return false;
	}
    
	Json::Value *pChild = GetChildByIndex(nIndex);
	if( pChild  == NULL)
	{
		return false;
	}
    
	return pChild->asBool();
}

int JsonParser::GetInt(const char* pName)
{
	if(!pName|| (m_pRoot == NULL))
	{
		return 0;
	}
    
	Json::Value *pChild = GetChildByName(string(pName));
	if( pChild  == NULL)
	{
		return 0;
	}
    
	return pChild->asInt();
}

int JsonParser::GetInt(const int nIndex)
{
	if((nIndex < 0) || (m_pRoot == NULL))
	{
		return 0;
	}
    
	Json::Value *pChild = GetChildByIndex(nIndex);
	if( pChild  == NULL)
	{
		return 0;
	}
    
	return pChild->asInt();
}

unsigned int JsonParser::GetUint(const char* pName)
{
	if(!pName || (m_pRoot == NULL))
	{
		return 0;
	}
    
	Json::Value *pChild = GetChildByName(string(pName));
	if( pChild  == NULL)
	{
		return 0;
	}
    
	return pChild->asUInt();
}

unsigned int JsonParser::GetUint(const int nIndex)
{
	if((nIndex < 0) || (m_pRoot == NULL))
	{
		return 0;
	}
    
	Json::Value *pChild = GetChildByIndex(nIndex);
	if( pChild  == NULL)
	{
		return 0;
	}
    
	return pChild->asDouble();
}

long long JsonParser::GetUint64(const char* pName)
{
	if(!pName || (m_pRoot == NULL))
	{
		return 0;
	}
    
	Json::Value *pChild = GetChildByName(string(pName));
	if( pChild  == NULL)
	{
		return 0;
	}
    
	//todo
	return pChild->asDouble();
}

long long JsonParser::GetUint64(const int nIndex)
{
	if((nIndex < 0) || (m_pRoot == NULL))
	{
		return 0;
	}
    
	Json::Value *pChild = GetChildByIndex(nIndex);
	if( pChild  == NULL)
	{
		return 0;
	}
    
	//todo
	return pChild->asUInt();
}

JsonParser JsonParser::GetReadOnlyReader(Json::Value* pRoot)
{
	if (pRoot == NULL)
	{
		return JsonParser();
	}
    
	JsonParser oReader;
	oReader.m_pRoot = pRoot;
	oReader.m_bReadOnly = true;
	oReader.m_bDeleteSelf = false;
    
	return oReader;
}


Json::Value* JsonParser::GetChildByIndex( const int nIndex )
{
	if( !m_pRoot->isValidIndex(nIndex))
	{
		return NULL ;
	}
    
	Json::Value *pChild = (&(*m_pRoot)[nIndex]);
    
	return pChild;
}*/