/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCCommon.h"

#include <stdarg.h>
#include <stdio.h>

#import <UIKit/UIAlert.h>

#include <sys/socket.h> // Per msqr

#include <sys/sysctl.h>

#include <net/if.h>

#include <net/if_dl.h>
#include "Reachability.h"

NS_CC_BEGIN

void CCLog(const char * pszFormat, ...)
{
    printf("Cocos2d: ");
    char szBuf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
}

// ios no MessageBox, use CCLog instead
void CCMessageBox(const char * pszMsg, const char * pszTitle)
{
    NSString * title = (pszTitle) ? [NSString stringWithUTF8String : pszTitle] : nil;
    NSString * msg = (pszMsg) ? [NSString stringWithUTF8String : pszMsg] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: title
                                                          message: msg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
}

void CCLuaLog(const char * pszFormat)
{
    puts(pszFormat);
}

// add by zhangqiyun on 20130708
// Desc:获取设备的mac地址
std::string GetDeviceMacAddress()
{
    int mib[6];
    size_t len;
    char *buf;
    unsigned char *ptr;
    struct if_msghdr *ifm;
    struct sockaddr_dl *sdl;
    
    mib[0] = CTL_NET;
    
    mib[1] = AF_ROUTE;
    
    mib[2] = 0;
    
    mib[3] = AF_LINK;
    
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0)
    {
        
        printf("Error: if_nametoindex error/n");
        
        return "";
        
    }
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
    {
        
        printf("Error: sysctl, take 1/n");
        
        return "";
        
    }
    if ((buf = (char*)malloc(len)) == NULL)
    {
        
        printf("Could not allocate memory. error!/n");
        
        return "";
        
    }
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        
        printf("Error: sysctl, take 2");
        
        return "";
        
    }
    ifm = (struct if_msghdr *)buf;
    
    sdl = (struct sockaddr_dl *)(ifm + 1);
    
    ptr = (unsigned char *)LLADDR(sdl);
    
    NSString *outstring = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    
    //NSString *outstring = [NSString stringWithFormat:@"x:x:x:x:x:x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    
    //NSString *outstring = [NSString stringWithFormat:@"xxxxxx", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    
    free(buf);
    
    return [outstring cStringUsingEncoding:NSUTF8StringEncoding];
    
}

NetWorkType GetNetwrokType()
{
    NetWorkType nType;
    Reachability *r = [Reachability reachabilityWithHostName:@"www.baidu.com"];
    
    switch ([r currentReachabilityStatus])
    {
        case NotReachable:    // 没有网络连接
        {
            nType = NetWork_NULL;
        }
            break;
            
        case ReachableViaWWAN:// 3G 或 2G
        {
            nType = NetWork_WWAN;
        }
            break;
            
        case ReachableViaWiFi:// WiFi
        {
            nType = NetWork_WIFI;
        }
            break;
    }
    return nType;
}

// add end


NS_CC_END
