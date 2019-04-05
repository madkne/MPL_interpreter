//
// Created by amindelavar on 4/3/2019.
//

#include <MPL/system.h>

//-----------------------------------vars
FILE *fs_file_instance[MAX_OPENED_FILES] = {0};

//***********************************
/**
 * open a file (if exist) by its mode and then save its handle to fs_file_instance
 * @param path
 * @param mode => w (write only) or  a (write append), r (read only) , b(binary)
 * @return int32
 */
int32 _FS_TYPE__fopen(String path, String mode) {
/**
 * modes:
 * w                            [OK]
 * r                            [OK]
 * a (write only append)        [OK]
 * w|r (write&read text)        [OK]
 * w|b (write only binary)      [OK]
 * r|b (read only binary)       [OK]
 * a|b (append binary)          [OK]
 * a|r (write & read append)    [OK]
 * w|r|b (write & read binary)  [OK]
 * a|r|b (append &read binary)  [OK]
 */
  //----------------init vars
  String final_mode = 0;
  str_list md = 0;
  uint32 md_len = 0;
  //=>check mode and set final_mode
  if (str_ch_equal(mode, 'w') || str_ch_equal(mode, 'r') || str_ch_equal(mode, 'a'))str_init(&final_mode, mode);
    //=>else split mode by '|'
  else {
    md_len = char_split(mode, '|', &md, true);
    Boolean writeonly = false, readonly = false, append = false, binary = false;
    for (uint32 i = 0; i < md_len; i++) {
      if (str_ch_equal(md[i], 'w'))writeonly = true;
      else if (str_ch_equal(md[i], 'r'))readonly = true;
      else if (str_ch_equal(md[i], 'a'))append = true;
      else if (str_ch_equal(md[i], 'b'))binary = true;
    }
    //=>if append and writeonly both are true
    if (writeonly && append)goto check_final_mode;
    //=>w|r
    if (writeonly && readonly && !binary)str_init(&final_mode, "w+");
      //=>w|b
    else if (writeonly && binary && !readonly)str_init(&final_mode, "wb");
      //=>r|b
    else if (readonly && binary && !writeonly && !append)str_init(&final_mode, "rb");
      //=>a|b
    else if (append && binary && !readonly)str_init(&final_mode, "ab");
      //=>a|r
    else if (append && readonly && !binary)str_init(&final_mode, "a+");
      //=>w|r|b
    else if (writeonly && readonly && binary)str_init(&final_mode, "wb+");
      //=>a|r|b
    else if (append && readonly && binary)str_init(&final_mode, "ab+");
  }
  check_final_mode:
  //=>check if final_mode is null
  if (final_mode == 0) {
    exception_user_handler(ERROR_ID, "bad_open_mode", "mode for open file is invalid", "fopen");
    return 0;
  }
  //=>handle to open file
  FILE *fp = NULL;
  #if LINUX_PLATFORM == 1
  //TODO:utf8 fopen for linux
  #elif WINDOWS_PLATFORM == 1
  wchar_t wfile[PATH_MAX - 1];
  wchar_t wmode[32];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, wfile, 260);
  MultiByteToWideChar(CP_UTF8, 0, final_mode, -1, wmode, 32);
  fp = _wfopen(wfile, wmode);
  #endif
  //=>check if fp is null
  if (fp == NULL) {
    exception_user_handler(ERROR_ID, "not_open_file", "can not open file", "fopen");
    return 0;
  }
  //=>set fp in fs_file_instance array and return its index
  for (uint32 i = 1; i < MAX_OPENED_FILES; i++) {
    if (fs_file_instance[i] == 0) {
      fs_file_instance[i] = fp;
      return i;
    }
  }
  //=>set error for full array file handles
  exception_user_handler(ERROR_ID, "max_opened_files", "can not handle another opened file", "fopen");
  return 0;
}
//***********************************
FILE *_FS_TYPE_get_file_pointer(int32 fp, String func_name) {
  //=>check if fp is -1
  if (fp < 1) {
    exception_user_handler(ERROR_ID, "invalid_file_pointer", "file pointer is invalid for close a file", func_name);
    return NULL;
  }
  //=>find FILE * from fs_file_instance
  for (uint32 i = 1; i < MAX_OPENED_FILES; i++) {
    if (i == fp && fs_file_instance[i] != 0) {
      return fs_file_instance[i];
    }
  }
  exception_user_handler(ERROR_ID, "invalid_file_pointer", "file pointer is invalid for close a file", func_name);
  return NULL;
}
//***********************************
Boolean _FS_TYPE__fclose(int32 fp) {

  //=>get real file pointer
  FILE *fpp = _FS_TYPE_get_file_pointer(fp, "fclose");
  //=>check fp is valid and exist
  if (fpp == NULL)return false;
  //=>check if close file is succeed
  if (fclose(fpp) == 0)return true;
  exception_user_handler(ERROR_ID, "not_close_file", "can not close the file", "fclose");
  return false;
}

//***********************************
int32 _FS_TYPE__fwrite(int32 fp, String s) {
  //=>get real file pointer
  FILE *fpp = _FS_TYPE_get_file_pointer(fp, "fwrite");
  //=>check fp is valid and exist
  if (fpp == NULL)return -1;

  #if WINDOWS_PLATFORM == true
  return fwprintf(fpp, L"%hs", s);
  #elif LINUX_PLATFORM == true
  //TODO
  #endif
  return -1;
}
//***********************************






