//
// Created by amindelavar on 9/19/2018.
//
#include <MPL/system.h>

//******************************************************
Boolean import_all_files() {
  //-----------------------analyzing source path
  String t_src = stdin_source_path;
  //printf("fgg:%s,%i\n",source,source);
  if (stdin_source_path == 0) {
    print_error(0, "not_exist_file", 0, t_src, 0, "import_all_files");
    return false;
  }
  //printf(print_str_list(program_argvs,argvs_len));

  //-----------------------append main source path to imin
  str_to_utf8(&entry_table.cur_source_path, "stdin");
  str_utf8 main_path = 0;
  str_to_utf8(&main_path, stdin_source_path);
  imin tmp1 = {entry_table.import_id++, IMPORT_FILE, true, main_path, 1, 0, 0, STDIN_SOURCE_PATH};
  append_imin(tmp1);

  //-----------------------loop until not exist any import instruction
  while (entry_table.import_inst_count > 0) {
    clear_soco(1);
    imin ret = get_first_active_imin(); //get first active import instruction imin struct
    Boolean ret1 = false;
//    printf("path:[%i]%s,%i\n", ret.type, utf8_to_bytes_string(ret.path), entry_table.sources_list_len);
    if (ret.type == IMPORT_FILE) {
      str_list_append(&entry_table.sources_list, utf8_to_bytes_string(ret.path), entry_table.sources_list_len++);
      ret1 = open_mpl_file(ret);
    } else if (ret.type == IMPORT_MODULE) {
      ret1 =
          load_module_file(utf8_to_bytes_string(ret.path), ret.line, utf8_to_bytes_string(source_paths[ret.source_id]));
    }
    entry_table.cur_source_path = ret.path;
    //utf8_str_print("XXXXX",ret.path,false);
//    print_struct(PRINT_MAIN_SOURCE_ST);
    //print_struct(PRINT_UTF8_ST);
    //----------
    if (ret1) analyze_source_code();
    delete_imin(ret.id, false); //inactive import record and not delete it
    //print_struct(PRINT_IMPORT_ST);
    //break;
  }
  return true;
}

//******************************************************
Boolean open_mpl_file(imin s) {
  // printf("VVV:%s\n",path);
  String ascii_path = utf8_to_bytes_string(s.path);
  String src_path = utf8_to_bytes_string(source_paths[s.source_id]);
//  printf("DONE!:%s\n",ascii_path);
  //----------------check if file exist and open it
  FILE *fp = utf8_file_open(ascii_path, "r");
  if (fp == NULL) {
    print_error(s.line, "not_open_file", src_path, ascii_path, "", "open_mpl_file");
    return false;
  }
  //printf("WWWWWWWW:%s\n",ascii_path);
  int32 status = 1;
  long_int line_number = 0;
  Boolean is_multiple_comment = false;
  Boolean is_multiple_str = false;
  str_utf8 glob_buffer = 0, glob_collect = 0;
  while (status >= 0) {
    //----------------init vars
    str_utf8 chars;
    uint32 size = 0;
    String optimize = 0;
    Boolean is_str = false;
    str_utf8 buffer = 0;
    str_utf8 collect = 0;
    uint8 max_bytes = 1;
    //----------------restore buffer,collect,is_str
    if (is_multiple_str) {
      is_multiple_str = false;
      is_str = true;
      utf8_str_init(&buffer, glob_buffer);
      utf8_str_init(&collect, glob_collect);
    }
    //----------------reading line by line from file in utf8
    status = utf8_read_line(fp, &chars, &size);

    //utf8_str_print("RRTT",chars,true);
    if (status == -10) {
      print_error(s.line, "file_not_support_utf8", (String) source_paths[s.source_id], ascii_path, "",
                  "open_mpl_file");
      return false;
    }

    //----------------trim line and check is empty
    line_number++;
    str_utf8 code_line = utf8_str_trim_space(chars);
    if (code_line == 0) {
      continue;
    }
    uint32 line_size = utf8_str_length(code_line);
    //----------------basic analyzing every line
    for (uint32 i = 0; i < line_size; i++) {
      //----------------check is multi line str
      if (is_str && i + 1 == line_size && code_line[i] == '\\') {
        is_multiple_str = true;
        glob_buffer = buffer;
        glob_collect = collect;
        break;
      }
      //----------------check is line comment
      if (!is_str && i + 1 < line_size && code_line[i] == '/' && code_line[i + 1] == '/') {
        break;
      }
      //----------------check is multi line comments
      if (!is_str && i + 1 < line_size) {
        if (code_line[i] == '/' && code_line[i + 1] == '*')is_multiple_comment = true;
        else if (code_line[i] == '*' && code_line[i + 1] == '/') {
          is_multiple_comment = false;
          i += 1;
          continue;
        }
      }
      //----------------
      if (is_multiple_comment) {
        continue;
      }
      //----------------check is str
      if (code_line[i] == '\"' && (i == 0 || code_line[i - 1] != '\\')) {
        is_str = switch_bool(is_str);
        if (is_str) {
          collect = 0;
          buffer = utf8_char_append(buffer, (uint32) '\"');
        } else {
          //utf8_str_print("gggg",collect,true);
          uint8 max_bytes = utf8_str_max_bytes(collect, true);
          //printf("XXXX:%i\n",max_bytes);
          if (max_bytes > 1) {
            utst tmp1 = {++entry_table.utf8_strings_id, collect, max_bytes};
            append_utst(tmp1);
            collect = 0;
            str_utf8 ss;
            str_to_utf8(&ss,
                        str_multi_append(UTF8_ID_LABEL, str_from_long_int(entry_table.utf8_strings_id), 0, 0, 0,
                                         0));
            buffer = utf8_str_append(buffer, ss);
            // printf("OOOO:%s\n",str_from_long_int(entry_table.utf8_strings_id));
          } else {
            buffer = utf8_str_append(buffer, collect);
            collect = 0;
          }
        }
      }
      //printf("WW@@@:%c\n",code_line[i]);
      //----------------add char to buffer
      if (is_str && (collect != 0 || code_line[i] != '\"')) {
        collect = utf8_char_append(collect, code_line[i]);
        uint8 tmp12 = utf8_need_bytes((uint8) code_line[i]);
      }
      if (!is_str && i > 0 && code_line[i - 1] == ' ' && code_line[i] == ' ') {
        continue;
      } else if (!is_str) {
        buffer = utf8_char_append(buffer, code_line[i]);
      }
    }
    if (!is_multiple_str) {
      //trim again buffer
      buffer = utf8_str_trim_space(buffer);
      if (buffer == 0) {
        continue;
      }
      //----------------convert buffer to String
      for (uint32 i = 0; i < utf8_str_length(buffer); i++) {
        if (!IS_ONE_BYTE(buffer[i])) {
          // printf("ERR:%i\n", buffer[i]);
          print_error(line_number, "bad_place_using_utf8", ascii_path, utf8_to_bytes_string(buffer), "",
                      "open_mpl_file");
          //printf("CXXXX:%s\n",);
          if (is_programmer_debug >= 2) {
            print_struct(PRINT_MAIN_SOURCE_ST);
            print_struct(PRINT_UTF8_ST);
          }
          return false;
        }
        optimize = char_append(optimize, (uint8) buffer[i]);
      }
      // optimize=utf8_to_str(buffer);
      //----------------store code line to source_code struct
      //utf8_str_print("Optimize",buffer,true);
      soco tmp1 = {(uint32) line_number, optimize, 0};
      //printf("optimize:%s\n",optimize);
      append_soco(1, tmp1);

    }
    //----------------free memory
    free(chars);
  }
  free(glob_buffer);
  free(glob_collect);
  //print_struct(3);
  //print_struct(4);
  fclose(fp);
  //printf("ENDDDDD:%s\n",ascii_path);
  return true;
}
//******************************************************
void convert_single_quotations_to_std() {
  //init vars
  Boolean is_string = false, is_qu = false, is_change = false;
  String inst = 0;

  //-------------------
  for (uint32 ii = 0; ii < entry_table.soco_main_count; ii++) {
    is_change = false;
    soco line = get_soco(1, ii);
    inst = line.code;
    for (uint32 i = 0; i < str_length(inst); i++) {
      //----------------check is str
      if (!is_qu && inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')) {
        is_string = switch_bool(is_string);
      }
      //----------------check is \'
      if (!is_string && inst[i] == '\'' && (i == 0 || inst[i - 1] != '\\')) {
        inst[i] = '\"';
        is_change = true;
        is_qu = switch_bool(is_qu);
      }
    }
    //----------------if is change
    if (is_change) {
//      printf("#%i:%s\n", line.line, inst);
      edit_soco(1, line.line, inst);
    }

  }
}
