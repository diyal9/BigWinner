//
//  JsonParser.h
//  JSONHttp
//
//  Created by 9Leaf on 11/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef JSONHttp_JsonParser_cpp
#define JSONHttp_JsonParser_cpp


#include "json.h"
#include <iostream>
#include <string.h>

class JsonParser
{
public:
	JsonParser();
	~JsonParser();
    
	/// ÅÐ¶Ïµ±Ç°parserÊÇ·ñÎªreadonly¡£ÎªreadonlyÊ±£¬²»ÄÜµ÷ÓÃParse½Ó¿Ú£»·ñÔòÊ§°Ü
	bool	IsReadyOnly();
    
	/// ½âÎöjsonÎÄ¼þ
	/*
     @param	pJsonContent	in£¬json×Ö·û´®£¬±ØÐëÎªutf8
     */
	bool	Parse(const char* pJsonContent);
    
	/// ¸ù¾ÝÃû×Ö»ñÈ¡×Ó½Úµã
	/*
     @param	pName	in£¬×Ó½ÚµãÃû×Ö
     */
	JsonParser	GetChild(const char* pName);
    
	/// ¸ù¾ÝÐòºÅ»ñÈ¡×Ó½Úµã
	/*
     @param	nIndex	in£¬×Ó½ÚµãÐòºÅ
     */
	JsonParser	GetChild(const int nIndex);
    
	/// ¸ù¾ÝÃû×Ö»ñÈ¡×Ó½Ú×Ö·û´®
	/*
     @param	pName	in£¬×Ó½ÚµãÃû×Ö
     */
	/*·µ»ØÖµÐèµ÷ÓÃReleaseDataÊÍ·Å*/
	char*	GetString(const char* pName);
    
	/// ¸ù¾ÝÐòºÅ»ñÈ¡×Ó½Úµã×Ö·û´®
	/*
     @param	nIndex	in£¬×Ó½ÚµãÐòºÅ
     */
	/*·µ»ØÖµÐèµ÷ÓÃReleaseDataÊÍ·Å*/
	char*	GetString(const int nIndex);
    
	/// ¸ù¾ÝÃû×Ö»ñÈ¡×Ó½Úµã²¼¶ûÖµ
	/*
     @param	pName	in£¬×Ó½ÚµãÃû×Ö
     */
	bool	GetBool(const char* pName);
    
	/// ¸ù¾ÝÐòºÅ»ñÈ¡×Ó½Úµã²¼¶ûÖµ
	/*
     @param	nIndex	in£¬×Ó½ÚµãÐòºÅ
     */
	bool	GetBool(const int nIndex);
    
	/// ¸ù¾ÝÃû×Ö»ñÈ¡×Ó½ÚµãÕûÊýÖµ
	/*
     @param	pName	in£¬×Ó½ÚµãÃû×Ö
     */
	int		GetInt(const char* pName);
    
	/// ¸ù¾ÝÐòºÅ»ñÈ¡×Ó½ÚµãÕûÊýÖµ
	/*
     @param	nIndex	in£¬×Ó½ÚµãÐòºÅ
     */
	int		GetInt(const int nIndex);
    
	/// ¸ù¾ÝÃû×Ö»ñÈ¡×Ó½Úµã·Ç·ûºÅÕûÊýÖµ
	/*
     @param	pName	in£¬×Ó½ÚµãÃû×Ö
     */
	unsigned int	GetUint(const char* pName);
    
	/// ¸ù¾ÝÐòºÅ»ñÈ¡×Ó½Úµã·Ç·ûºÅÕûÊýÖµ
	/*
     @param	nIndex	in£¬×Ó½ÚµãÐòºÅ
     */
	unsigned int	GetUint(const int nIndex);
    
	/// ¸ù¾ÝÃû×Ö»ñÈ¡×Ó½Úµã64Î»·Ç·ûºÅÕûÊýÖµ
	/*
     @param	pName	in£¬×Ó½ÚµãÃû×Ö
     */
	long long	GetUint64(const char* pName);
    
	/// ¸ù¾ÝÐòºÅ»ñÈ¡×Ó½Úµã64Î»·Ç·ûºÅÕûÊýÖµ
	/*
     @param	nIndex	in£¬×Ó½ÚµãÐòºÅ
     */
	long long	GetUint64(const int nIndex);
    
    
private:
	static JsonParser	GetReadOnlyReader(Json::Value* pRoot);
	Json::Value*		GetChildByName(std::string& strName);
	Json::Value*		GetChildByIndex(const int nIndex);
    
    
private:
	Json::Value*		m_pRoot;		///<  jsonÎÄ¼þ¸ù½Úµã
	bool				m_bReadOnly;	///<  ÊÇ·ñÖ»¶Á
	bool				m_bDeleteSelf;	///<  ÊÇ·ñÐèÒª×Ô¼º¹ÜÀíÄÚ´æÊÍ·Å
};


#endif