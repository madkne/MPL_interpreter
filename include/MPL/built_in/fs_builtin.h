//
// Created by amindelavar on 4/3/2019.
//

#ifndef MPL_FS_BUILTIN_H
#define MPL_FS_BUILTIN_H

//-----------------------------------vars
FILE * fs_file_instance[MAX_OPENED_FILES];
//-----------------------------------funcs
int32 _FS_TYPE__fopen(String path,String mode);
FILE *_FS_TYPE_get_file_pointer(int32 fp,String func_name);
Boolean _FS_TYPE__fclose(int32 fp);
int32 _FS_TYPE__fwrite(int32 fp,String s);

#endif //MPL_FS_BUILTIN_H
