//
// Created by amindelavar on 12/23/2018.
//

#include <MPL/modules/fs.h>

String fs_mod__LibVersion() {
  return FS_LIB_VERSION;
}
//*****************************
Boolean fs_mod__mkdir(String path) {
  return __syscall_mkdir(path,false);
}