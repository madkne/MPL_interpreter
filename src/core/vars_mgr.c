//
// Created by amindelavar on 9/19/2018.
//

#include <MPL/system.h>


Boolean is_valid_name(String name, Boolean is_array) {
    //fmt.Println("WWWWWWW:", name)
    //var sp=[]byte{',','!','#','$','%','^','&','*','(',')','-','=','+','/','\\','.','?',':',';','~'}
    uint32 name_len = str_length(name);
    if (name_len < 1) {
        return false;
    }
    //******************************is mahdi keyword
    if (str_search(keywords, name, StrArraySize(keywords))) {
        return false;
    }
    //******************************split index , name
    String name_v = 0, index = 0, buf = 0, extra = 0;
    Boolean is_string = false, is_end = false;
    uint32 pars = 0;
    if (is_array) {
        for (uint32 i = 0; i < name_len; i++) {
            //------------------check is string
            if (name[i] == '\"' && (i == 0 || name[i - 1] != '\\')) {
                is_string = switch_bool(is_string);
            }
            //------------------count pars
            if (!is_string && name[i] == '[')
                pars++;
            else if (!is_string && name[i] == ']')
                pars--;

            //------------------set name_v
            if (!is_string && name[i] == '[' && name_v == 0 && pars == 1) {
                name_v = str_trim_space(buf);
                buf = 0;
                continue;
            }
            //------------------set index
            if (!is_string && name[i] == ']' && name_v != 0 && pars == 0) {
                index = str_trim_space(buf);
                buf = 0;
                is_end = true;
                continue;
                //break
            }
            //------------------append to buf
            buf = char_append(buf, name[i]);
        }
        //------------------set extra :k1[0]+3
        if (!is_string && is_end && buf != 0) {
            str_init(&extra, buf);
        }
        if (name_v == 0) {
            str_init(&name_v, buf);
        }

    } else {
        str_init(&name_v, name);
    }
    //msg("&%%%", name, name_v, index, extra)
    //******************************check for extra
    if (extra != 0) {
        return false;
    }
    //******************************check for name_v
    if (str_length(name_v) > 0 &&
        ((name_v[0] >= '0' && name_v[0] <= '9') || (name_v[0] == '_' && str_equal(interpreter_level, "parse"))
        )) {
        return false;
    }
    for (uint32 i = 0; i < str_length(name_v); i++) {
        if ((name_v[i] >= '0' && name_v[i] <= '9') || (name_v[i] >= 'A' && name_v[i] <= 'Z') ||
            (name_v[i] >= 'a' && name_v[i] <= 'z') || name_v[i] == '_' || (name_v[i] == '@' && i == 0)) {
            continue;
        } else {
            return false;
        }
    }
    //******************************check for index
    if (is_array && index == 0) {
        //return false
    }
    return true;
}


//*********************************************************
void return_name_index_var(String s, Boolean is_empty_index, String *name, String *index) {
    /**
    error messages:
    - array dems is larger than max_array_dimension
    */
    //**************************define variables
    String word = 0;
    uint8 bra = 0;
    *name = 0, *index = 0;
    if (!is_empty_index) {
        *index = "0";
    }

    //**************************start analyzing
    uint32 len = str_length(s);
    //----------------finding name and index of var
    for (uint32 i = 0; i < len; i++) {
        if (s[i] == '[')bra++;
        else if (s[i] == ']')bra--;
        //----------
        if ((s[i] == '[' && bra == 1) || (s[i] == ']' && bra == 0) || i + 1 == len) {
            if (s[i] == '[' || i + 1 == len && (*name) == 0) {
                if (i + 1 == len) {
                    word = char_append(word, s[i]);
                }
                str_init(&*(name), word);
            } else if (name != 0 && s[i] == ']') {
                str_init(&(*index), word);
                break;
            }
            word = 0;
        } else {
            word = char_append(word, s[i]);
        }
    }
    (*index)=str_trim_space(*index);
    //printf("@@@2:%s=>%s#,%s#\n", s, *name, *index);
}

//********************************************
uint8 define_vars_analyzing(String inst, def_var_s vars_store[]) {
    /**
    1- num x1,x2,str x3 								---OK---
    2- num x1,x2=(3+5)*x3 								---OK---
    3- num x1,x2,str x3=0x3A4D,(5+7)/2,("we"*10)  		---OK---
    4- num x1=45 										---OK---
    5- bool x1=true&&(f==45) 							---OK---
    6- num x1[2,2]={{5,7.8},{1,7}}	 					---OK---
    7- num x1=x2[1,2]*2 								---OK---
    8- str st2[2]={"Amin",st1[0]*2+"!"} 				---OK---
    9- str q1[?,5] 										---OK---
    ---------------------SAMPLES---------------------
    //str ss,num b1,b2="WE",-(2 ^(-3.0 +1))*3,0x345+3  //b1=-(2 ^(-3 +1))*3 =-0.75
    //num x1,x2,str x3=0x3A4D,(5+7)/2,"Hello "+("Amin "*3)+"!"
    //str x1, x3="Hello "+("Amin "*3)+"!",{"Amin","Ali","Reza"}
    //__define["x1"]="Hello "+("Amin "*3)+"!"
    //num zz[2,3]={{4+6,7.84,(9^2)-1},{8,888,1000}}
    //bool x1=true&&!((45-1)>=45) //x1=true
    */
    //printf("DDDDDDDDDDD:%s\n",inst);
    //*******************************************is null
    inst=str_trim_space(inst);
    if(str_is_empty(inst)) return 0;
    String word = 0, last_type = 0, data_type = 0;
    Boolean is_string = false, is_equal = false;
    int16 vars_counter = -1, vals_counter = -1, pars = 0, bra = 0;
    //*******************************************determine variables with their values
    uint32 len = str_length(inst);
    for (uint32 i = 0; i < len; i++) {
        //------------------check is string
        if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')) {
            is_string = switch_bool(is_string);
        }
        //------------------continue if ' '
        if (!is_string && i + 1 < len && inst[i] == ' ' && (inst[i + 1] == '(' || inst[i + 1] == '[')) {
            continue;
        }
        //------------------count bra
        if (!is_string) {
            switch (inst[i]) {
                case '[':
                    bra++;
                    break;
                case ']':
                    bra--;
                    break;
            }
        }
        //------------------determine data type
        if (!is_string && !is_equal && word != 0 && inst[i] == ' ' &&
            (is_valid_name(word, false) || str_equal(word, "vars") ||
             str_equal(word, "num") ||
             str_equal(word, "str") ||
             str_equal(word, "bool"))) {
            str_init(&data_type, word);
            word = 0;
        }
        //------------------store a variable
        if (!is_string && !is_equal && pars == 0 && (inst[i] == ',' || inst[i] == '=' || i + 1 == len) &&
            data_type != 0 && bra == 0) {
            if (i + 1 == len) {
                word = char_append(word, inst[i]);
            }
            word = str_trim_space(word);
            if (!is_valid_name(word, true)) {
                print_error(entry_table.line_number, "invalid_name_var", entry_table.cur_ascii_source_path, word, "",
                            "define_vars_analyzing");
                return 0;
            }

            if (word != 0) {

                //add new record to ret
                vars_counter++;
                return_name_index_var(word, false, &vars_store[vars_counter].name_var,
                                      &vars_store[vars_counter].index_var);
                str_init(&vars_store[vars_counter].main_type, data_type);
                //printf("IIIII:%s[%i]=>%s\n",word,vars_counter,data_type);
                vars_store[vars_counter].sub_type = 0;
                vars_store[vars_counter].value_var = 0;
                vars_store[vars_counter].sid = entry_table.current_sid;
                vars_store[vars_counter].fid = entry_table.current_fid;
                //printf("&SDSS:%s,%s,%s=>\n", word, data_type,ret.name_var);
                str_init(&last_type, data_type);
            }
        }
        //------------------allocate values to variables
        //msg("VVVV:", word, "|MMM:", pars)
        if (is_equal && !is_string && (word != 0 || i + 1 == len) &&
            ((inst[i] == ',' && pars == 0) || (i + 1 == len && pars < 2)) && bra == 0) {
            if (i + 1 == len) {
                word = char_append(word, inst[i]);
            }
            /*if vals_counter > vars_counter {
                //msg("&SSS", vals_counter, vars_counter)
                exception_handler("wrong_def_var", "define_vars_analyzing", word, "")
            } else */
            if (inst[i] == '}') pars--;
            vals_counter++;
            //printf("UUU:%s,%s,%i\n", word,vars_store[vals_counter].main_type,vals_counter);
            word = str_trim_space(word);
            String main_type = vars_store[vals_counter].main_type;
            if (word[0] == '{' && !str_equal(main_type, "bool") && !str_equal(main_type, "str")) {
                String buf = 0;
                uint8 sub = INT_SUB_TYPE_ID;
                for (uint32 j = 0; j < str_length(word); j++) {
                    if (buf != 0 && (word[j] == '{' || word[j] == '}' || word[j] == ',')) {
                        uint8 tmp = determine_sub_type_var(&buf, main_type);
                        if (tmp == HUGE_SUB_TYPE_ID) {
                            sub = tmp;
                            break;
                        } else if (tmp > sub)sub = tmp;
                        buf = 0;
                        continue;
                    }
                    buf = char_append(buf, word[j]);
                }
                vars_store[vals_counter].sub_type = sub;
            } else {
                vars_store[vals_counter].sub_type = determine_sub_type_var(&word, vars_store[vals_counter].main_type);
            }
            str_init(&vars_store[vals_counter].value_var, word);
            word = 0;

        }

        //------------------append to word
        Boolean empty_word = false;
        if (!is_string && !is_equal && (inst[i] == ' ' || inst[i] == '=' || inst[i] == ',')) {
            empty_word = true;
        } else if (!is_string && is_equal && pars == 0 && inst[i] == ',') {
            empty_word = true;
        }
        if (empty_word && bra == 0) {
            word = 0;
            //------------------is_equal
            if (inst[i] == '=') is_equal = true;
        } else {
            word = char_append(word, inst[i]);
        }
        //------------------count pars
        if (!is_string && is_equal) {
            switch (inst[i]) {
                case '{':
                    pars++;
                    break;
                case '}':
                    pars--;
                    break;
            }
        }
    }
    //**************alloc one value to many variables
    //TODO:
    if (vals_counter == 0 && vars_counter > 0) {
        for (uint8 i = 1; i <= vars_counter; i++) {
            /*if vars_store[0].type_var != vars_store[i].type_var {
                exception_handler("not_alloc_val", "define_vars_analyzing", vars_store[i].name_var, vars_store[0].value_var)
                return -1, vars_store
            }*/
            //str_init(&vars_store[i].value_var , vars_store[0].value_var);
        }
    }
    //**************if values is less than vars
    /*if (vals_counter>0&&vars_counter>vals_counter){
        for (uint8 i=0;i<=vars_counter;i++){
            if vars_store[i].value_var==""{
                exception_handler("not_equal_vals_vars", "define_vars_analyzing:433","","")
                return -1, vars_store
            }
        }
    }*/
    //TODO:
    //**************if values is more than vars
    /*if is_runtime&&vars_counter<vals_counter{
        for i:=0;i<=vals_counter;i++{
            if vars_store[i].name_var==""{
                exception_handler("not_equal_vals_vars", "define_vars_analyzing:442","","")
                return -1, vars_store
            }
        }
    }*/
    //**************return
    //printf("CCCC:%i:%s\n",vars_counter,vars_store[0].name_var);
    for (uint8 i = 0; i < vars_counter + 1; i++) {
        def_var_s tmp3 = vars_store[i];
        if(tmp3.sub_type==0) tmp3.sub_type=determine_sub_type_var(&tmp3.value_var,tmp3.main_type);
        //printf("WWWSSS(%i):%s(%i)-%s[%s]=%s\n", i, tmp3.main_type, tmp3.sub_type, tmp3.name_var,tmp3.index_var, tmp3.value_var);
    }
    return vars_counter + 1;
}


//*********************************************************
uint8 determine_sub_type_var(String *value, String main_type) {
    //----------------------------------if main_type is str or bool
    if (str_equal(main_type, "str")) return STR_SUB_TYPE_ID;
    else if (str_equal(main_type, "bool")) return BOOL_SUB_TYPE_ID;
    //-----------------------------------if main_type is num
    Boolean is_point = false;
    //-----------------if defined sub type
    String val = 0;
    str_init(&val, (*value));
    uint32 len = str_length(val);
    if (len > 2)
        for (uint8 i = 2; i < StrArraySize(sub_types); i++) {
            uint32 size = str_length(sub_types[i]);
            if (len - size == 0) continue;
            String word = str_substring(val, len - size, 0);
            //printf("DDDD:%s=>%s,%s,%i\n",val,word,sub_types[i],len-size);
            if (word != 0 && len - size - 1 > 0 && val[len - size - 1] == '.' && str_equal(word, sub_types[i])) {
                (*value) = str_substring(val, 0, len - size - 1);
                return i + 1;
            }
        }
    //-----------------check num attrs
    for (uint32 i = 0; i < len; i++) {
        if (val[i] == '.') {
            is_point = true;
            break;
        }
    }

    if (!is_point && len < MAX_INT_LEN) return INT_SUB_TYPE_ID;
    if (is_point && len < MAX_FLOAT_LEN) return FLOAT_SUB_TYPE_ID;

    return HUGE_SUB_TYPE_ID;
}