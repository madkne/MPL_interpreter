//
// Created by amindelavar on 1/20/2019.
//

#ifndef MPL_BUILDER_H
#define MPL_BUILDER_H

void start_builder();
Boolean builder_check_exist();
Boolean builder_generate_code();
Boolean builder_export_app();
Boolean builder_generate_resource();
String __generate_blst_codes(uint8 type);
String __generate_instru_codes();
String __generate_utst_codes();
String __generate_datas_codes();
String __generate_modules_codes();
String __generate_str_list(str_list s, uint32 slen, String name);
String __generate_header_codes();
#endif //MPL_BUILDER_H
