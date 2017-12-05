/*
 * GRCCore.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCCORE_H_
#define GRCCORE_H_

#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include "GRCLogger.h"

#define GRC_INVALID_THREAD	(pthread_t)-1
#define GRC_INVALID_INDEX	(size_t)-1
#define GRC_INVALID_FD		(int)-1

#define GRC_ACTION_NONE
#define GRC_ACTION_RETURN		return
#define GRC_ACTION_RETVAL(x)	return (x)

#define GRC_CHECK_FUNC_ACTION(exp,func,action)	{if(!(exp)){GRCLogger::write(LOG_WARNING,"%s" " <%s,%s(%d),%s(%d)",#exp,__FUNCTION__,__FILE__,__LINE__,strerror(errno),errno);func;action;}}
#define GRC_CHECK_RETURN(exp)					GRC_CHECK_FUNC_ACTION(exp,;,GRC_ACTION_RETURN)
#define GRC_CHECK_RETVAL(exp,val)				GRC_CHECK_FUNC_ACTION(exp,;,GRC_ACTION_RETVAL(val))
#define GRC_CHECK_RETFALSE(exp)					GRC_CHECK_FUNC_ACTION(exp,;,GRC_ACTION_RETVAL(false))
#define GRC_CHECK_RETNULL(exp)					GRC_CHECK_FUNC_ACTION(exp,;,GRC_ACTION_RETVAL(NULL))
#define GRC_CHECK_RETZERO(exp)					GRC_CHECK_FUNC_ACTION(exp,;,GRC_ACTION_RETVAL(0))
#define GRC_CHECK_RETMINUS(exp)					GRC_CHECK_FUNC_ACTION(exp,;,GRC_ACTION_RETVAL(-1))
#define GRC_CHECK_FUNC_RETURN(exp,func)			GRC_CHECK_FUNC_ACTION(exp,func,GRC_ACTION_RETURN)
#define GRC_CHECK_FUNC_RETVAL(exp,func,val)		GRC_CHECK_FUNC_ACTION(exp,func,GRC_ACTION_RETVAL(val))
#define GRC_CHECK_FUNC_RETFALSE(exp,func)		GRC_CHECK_FUNC_ACTION(exp,func,GRC_ACTION_RETVAL(false))
#define GRC_CHECK_FUNC_RETNULL(exp,func)		GRC_CHECK_FUNC_ACTION(exp,func,GRC_ACTION_RETVAL(NULL))
#define GRC_CHECK_FUNC_RETZERO(exp,func)		GRC_CHECK_FUNC_ACTION(exp,func,GRC_ACTION_RETVAL(0))
#define GRC_CHECK_FUNC_RETMINUS(exp,func)		GRC_CHECK_FUNC_ACTION(exp,func,GRC_ACTION_RETVAL(-1))

#define GRC_CHECKV_FUNC_ACION(exp,func,action,fmt,...)	{if(!(exp)){GRCLogger::write(LOG_WARNING,"%s " fmt " <%s,%s(%d),%s(%d)",#exp, ##__VA_ARGS__,__FUNCTION__,__FILE__,__LINE__,strerror(errno),errno);func;action;}}
#define GRC_CHECKV_RETURN(exp,fmt,...)					GRC_CHECKV_FUNC_ACION(exp,;,GRC_ACTION_RETURN,fmt,__VA_ARGS__)
#define GRC_CHECKV_RETVAL(exp,val,fmt,...)				GRC_CHECKV_FUNC_ACION(exp,;,GRC_ACTION_RETVAL(val),fmt,__VA_ARGS__)
#define GRC_CHECKV_RETFALSE(exp,fmt,...)				GRC_CHECKV_FUNC_ACION(exp,;,GRC_ACTION_RETVAL(false),fmt,__VA_ARGS__)
#define GRC_CHECKV_RETNULL(exp,fmt,...)					GRC_CHECKV_FUNC_ACION(exp,;,GRC_ACTION_RETVAL(NULL),fmt,__VA_ARGS__)
#define GRC_CHECKV_RETZERO(exp,fmt,...)					GRC_CHECKV_FUNC_ACION(exp,;,GRC_ACTION_RETVAL(0),fmt,__VA_ARGS__)
#define GRC_CHECKV_RETMINUS(exp,fmt,...)				GRC_CHECKV_FUNC_ACION(exp,;,GRC_ACTION_RETVAL(-1),fmt,__VA_ARGS__)
#define GRC_CHECKV_FUNC_RETURN(exp,func,fmt,...)		GRC_CHECKV_FUNC_ACION(exp,func,GRC_ACTION_RETURN,fmt,__VA_ARGS__)
#define GRC_CHECKV_FUNC_RETVAL(exp,func,val,fmt,...)	GRC_CHECKV_FUNC_ACION(exp,func,GRC_ACTION_RETVAL(val),fmt,__VA_ARGS__)
#define GRC_CHECKV_FUNC_RETFALSE(exp,func,fmt,...)		GRC_CHECKV_FUNC_ACION(exp,func,GRC_ACTION_RETVAL(false),fmt,__VA_ARGS__)
#define GRC_CHECKV_FUNC_RETNULL(exp,func,fmt,...)		GRC_CHECKV_FUNC_ACION(exp,func,GRC_ACTION_RETVAL(NULL),fmt,__VA_ARGS__)
#define GRC_CHECKV_FUNC_RETZERO(exp,func,fmt,...)		GRC_CHECKV_FUNC_ACION(exp,func,GRC_ACTION_RETVAL(0),fmt,__VA_ARGS__)
#define GRC_CHECKV_FUNC_RETMINUS(exp,func,fmt,...)		GRC_CHECKV_FUNC_ACION(exp,func,GRC_ACTION_RETVAL(-1),fmt,__VA_ARGS__)

#define GRC_DEV(fmt,...)	GRCLogger::write(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define GRC_INFO(fmt,...)	GRCLogger::write(LOG_INFO, fmt, ##__VA_ARGS__)
#define GRC_WARN(fmt,...)	GRCLogger::write(LOG_WARNING, fmt, ##__VA_ARGS__)
#define GRC_ERR(fmt,...)	GRCLogger::write(LOG_ERR, fmt, ##__VA_ARGS__)

#define GRC_COUNT(cnt,logt,fmt,...)	{static int _count=0; if (_count<cnt) {_count++; GRCLogger::write(logt,"[%d/%d]" fmt,_count,cnt, ##__VA_ARGS__); }}
#define GRC_DEV_COUNT(cnt,fmt,...)	GRC_COUNT(cnt, LOG_DEBUG, fmt, __VA_ARGS__)
#define GRC_INFO_COUNT(cnt,fmt,...)	GRC_COUNT(cnt, LOG_INFO, fmt, __VA_ARGS__)

#endif /* GRCCORE_H_ */
