//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_SETTINGS_H
#define MPL_SETTINGS_H

//------------------------------------------basic settings
#define  true                           1
#define  false                          0
//------------------------------------------main settings
#define  OS_BUILD                       "windows-10" //"linux-lite"
#define  LINUX_PLATFORM                  false
#define  WINDOWS_PLATFORM                true
#define  x86_OS                          true
#define  x64_OS                          false
#define  C_COMPILER_STD                  "C99"
#define  C_COMPILER_NAME                 "GCC"
#define  BUILD_DATE                      __DATE__
#define  BUILD_TIME                      __TIME__
//------------------------------------------define __func__ constant
#if __STDC_VERSION__ < 199901L
#if __GNUC__ >= 2
#define __func__ __FUNCTION__
#else
#define __func__ "<unknown>"
#endif
#endif


#endif //MPL_SETTINGS_H
