/* 
 * Author:	xueyunlong@gmail.com
 * Date:	2014-08-15
 * 
 */
#ifndef _XTRACE_XCODES_H_
#define _XTRACE_XCODES_H_

#define	X1_SUCC	0
#define X2_SUCC	""
#define	X1_ERR	-1
#define X2_ERR	"General Error"

#define	X1_FILE_ERR	-100
#define X2_FILE_ERR	"FILE Operation Error"
#define	X1_FILE_ERR_READ	-101
#define X2_FILE_ERR_READ	"FILE read Error"
#define	X1_FILE_ERR_WRITE	-102
#define X2_FILE_ERR_WRITE	"FILE write Error"

#define	X1_KEY_EXIST	101
#define X2_KEY_EXIST	"The file include keyword( __X_ __XR_ __XL_ )"
#define	X1_NOT_XTRACE_FILE	105
#define X2_NOT_XTRACE_FILE  "Not xtrace file"

#define	X1_UNMATCH_PAIR	200
#define X2_UNMATCH_PAIR  "Can't match pair ()<>{}[]..."

#endif
