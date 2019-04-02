//
// Created by madkne on 12/23/2018.
//

#ifndef FS_FS_H
#define FS_FS_H

#include "../system.h"
//*******************************constants
#define FS_LIB_VERSION                     "1.1"
//*******************************functions
String fs_mod__LibVersion();
Boolean fs_mod__mkdir(String path);

#endif //FS_FS_H
