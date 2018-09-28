//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_SETTINGS_H
#define MPL_SETTINGS_H

#define OS_BUILD                       "windows-10-x86_64"
#define LINUX_PLATFORM                  0
#define WINDOWS_PLATFORM                1
#define x86_OS                          1
#define x64_OS                          0
//------------------------------------------
#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif


#endif //MPL_SETTINGS_H
