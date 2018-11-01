#include <MPL/system.h>

//******************************************
String read_input()
{
	String ret = "";
	uint8 c = 0;
	for (;;) {
		c = fgetc(stdin);
		if (c == EOF || c == '\n'/*ENTER*/) break;
		ret = char_append(ret, c);
	}
	return ret;
}

//******************************************
String get_host_name()
{
	#if WINDOWS_PLATFORM == true
	return getenv("COMPUTERNAME");
	#elif LINUX_PLATFORM == true
	return getenv("HOSTNAME");
	#endif
	return "<unknown>";
}

//******************************************
long_int get_pid()
{
	#if WINDOWS_PLATFORM == true
	return (long_int) GetCurrentProcessId();
	#elif LINUX_PLATFORM == true
	//TODO
	#endif
	return 0;
}

//******************************************
String get_absolute_path(String path)
{
	//uint8 resolved_path[PATH_MAX+1];
	#if LINUX_PLATFORM == 1
	String abs_path=0;
	abs_path=realpath(path, 0);
	//printf("\n%s=>%s\n",path,abs_path);
	return abs_path;
	#elif WINDOWS_PLATFORM == 1
	char tmp[BufferSize];
	String abs_path = 0;
	GetFullPathName(path, BufferSize, tmp, 0);
	str_init(&abs_path, tmp);
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(abs_path) && GetLastError() == ERROR_FILE_NOT_FOUND) return 0;
	//printf("\nSSS::%s=>%s\n",path,abs_path);
	return abs_path;
	#endif
	return 0;
}

//******************************************
Boolean switch_bool(Boolean b)
{
	if (b) return false;
	return true;
}

//******************************************
void msg(String format, ...)
{
	String *argvs = (String *) &format;
	argvs++;
	uint32 c = 0;
	String str;
	uint32 u32;
	uint8 u8;
	while ((c = *format++) != 0) {
		if (c != '%') {
			//dbg_putc(c);
			printf("%c", c);
		}
		else {
			c = *format++;
			switch (c) {
				case 'c':
					u8 = *(String) argvs++;
					printf("%c", c);
					break;
				case 's':
					//str_init(&str,*argvs++);
					str = (String) argvs++;
					if (!str) str_init(&str, "[NULL]");
					printf("%s", str);

					break;
				case 'b':
					u32 = *(int *) argvs++;
					if (u32 == false)
						printf("FALSE");
					else
						printf("TRUE");
					break;
				case 'i':
					printf("%i", *(int32 *) (argvs++));
					break;
					//default:
					//printf();

			}
		}
	}
}

//******************************************
void longint_list_delete_first(longint_list *s, uint32 len)
{
	if (len == 0) {
		(*s) = 0;
		return;
	}
	longint_list tmp = 0;
	longint_list_init(&tmp, (*s), len);
	free((*s));
	(*s) = (longint_list) malloc((len - 1) * sizeof(longint_list));
	uint32 ind = 0;
	for (uint32 i = 1; i < len; i++) {
		//str_init(&(*s)[i],val[i]);
		(*s)[ind++] = tmp[i];
	}
	free(tmp);
}

//******************************************
void longint_list_append(longint_list *s, uint32 len, long_int n)
{
	longint_list tmp = 0;
	longint_list_init(&tmp, *s, len);
	free(*s);
	(*s) = (longint_list) malloc((len + 1) * sizeof(longint_list));
	if (*s == 0)return;
	for (uint32 i = 0; i < len; i++) {
		(*s)[i] = tmp[i];
	}
	(*s)[len] = n;
}

//*************************************************************
uint32 longint_list_join(longint_list *s, longint_list a1, uint32 a1_len, longint_list a2, uint32 a2_len)
{
	//if(s!=0)free(*s);
	uint32 ind = 0;
	(*s) = (longint_list) malloc((a1_len + a2_len) * sizeof(longint_list));
	if (*s == 0)return 0;
	for (uint32 i = 0; i < a1_len; i++) {
		(*s)[ind++] = a1[i];
	}
	for (uint32 i = 0; i < a2_len; i++) {
		(*s)[ind++] = a2[i];
	}
	return ind;
}

//*************************************************************
void longint_list_init(longint_list *s, longint_list val, uint32 len)
{
	if (len == 0) {
		(*s) = 0;
		return;
	}
	(*s) = (longint_list) malloc(len * sizeof(longint_list));
	for (uint32 i = 0; i < len; i++) {
		//str_init(&(*s)[i],val[i]);
		(*s)[i] = val[i];
	}
}

//*************************************************************
String longint_list_print(longint_list s, uint32 len)
{
	if (len == 0) return "[NULL]";
	String ret = 0;
	ret = char_append(ret, '{');
	for (uint32 i = 0; i < len; i++) {
		//printf("%s,%i\n",s[i],str_length(s[i]));
		ret = str_append(ret, str_from_long_int(s[i]));
		if (i + 1 < len) ret = char_append(ret, ',');
	}
	ret = char_append(ret, '}');
	return ret;
}

//******************************************
double int32_power(double base, int32 power)
{
	double ret = 1;
	Boolean is_neg = false;
	if (power < 0) {
		is_neg = true;
		power *= -1;
	}
	for (uint32 b = 0; b < power; b++) {
		ret *= base;
	}
	if (is_neg) {
		ret = (double) 1 / ret;
	}
	return ret;
}

//******************************************
String get_current_datetime(uint8 type)
{
	time_t t = time(NULL);
	struct tm tim = *localtime(&t);
	//char s[64];
	String ret = 0;
	//date and time : 2018-6-23 3:45:12
	if (type == 1) {
		ret = malloc(6 * sizeof(int) + 5);
		sprintf(ret, "%i-%i-%i %i:%i:%i", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min,
		        tim.tm_sec);
	}
	else if (type == 2) {
		return str_from_long_int((long_int) t);
	}

	//strftime(s, sizeof(s), "%c", tm);
	return ret;
}
//******************************************

//******************************************
//******************************************
String print_str_list(str_list s, uint32 len)
{
	if (len == 0) return "[NULL]";
	String ret = 0;
	ret = char_append(ret, '{');
	for (uint32 i = 0; i < len; i++) {
		//printf("%s,%i\n",s[i],str_length(s[i]));
		ret = str_append(ret, s[i]);
		if (i + 1 < len) ret = char_append(ret, ',');
	}
	ret = char_append(ret, '}');
	return ret;
}

//******************************************
void print_struct(uint8 which)
{
	if (which == 0 || which == PRINT_IMPORT_ST) {
		imin *tmp1 = entry_table.import_start;
		printf("=====Print import_inst_struct :\n");
		for (;;) {
			printf("Active:%i,id:%li,type:%i,line:%i,source:[%i]\n", tmp1->is_active, tmp1->id, tmp1->type, tmp1->line,
			       tmp1->source_id);
			utf8_str_print("path:", tmp1->path, true);
			utf8_str_print("source:", source_paths[tmp1->source_id], true);
			printf("\n");
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_EXCEPTIONS_ST) {
		exli *tmp1 = entry_table.exli_start;
		printf("=====Print exceptions_list_struct :\n");
		for (;;) {
			printf("id:%i,type:%i,group:%s,lbl:%s,text:%s\n", tmp1->id, tmp1->type, exceptions_group[tmp1->group],
			       tmp1->lbl, tmp1->text);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_UTF8_ST) {
		utst *tmp1 = entry_table.utst_start;
		if (tmp1 == 0) return;
		printf("=====Print utf8_strings_struct :\n");
		for (;;) {
			printf("id:%li,max_bytes:%i, ", tmp1->id, tmp1->max_bytes_per_char);
			utf8_str_print("string", tmp1->utf8_string, true);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_MAIN_SOURCE_ST) {
		soco *tmp1 = entry_table.soco_main_start;
		if (tmp1 == 0) return;
		printf("=====Print source_code_struct :\n");
		for (;;) {
			printf("line:%i,code:%s\n", tmp1->line, tmp1->code);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_TOKENS_SOURCE_ST) {
		soco *tmp1 = entry_table.soco_tokens_start;
		if (tmp1 == 0) return;
		printf("=====Print source_code_tokens_struct :\n");
		for (;;) {
			printf("id:%i,code:%s\n", tmp1->line, tmp1->code);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_FUNC_ST) {
		blst *tmp1 = entry_table.blst_func_start;
		if (tmp1 == 0) return;
		printf("=====Print func_block_struct :\n");
		for (;;) {
			printf("id:%li,name:%s,params:%s\n", tmp1->id, tmp1->lbl, print_str_list(tmp1->params, tmp1->params_len));
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_INSTRU_ST) {
		instru *tmp1 = entry_table.instru_start;
		if (tmp1 == 0) return;
		printf("=====Print instructions_struct :\n");
		for (;;) {
			printf("(id:%li,fid:%li,sid:%li,order:%li,type:%i,line:%i),code:%s\n", tmp1->id, tmp1->func_id,
			       tmp1->stru_id, tmp1->order, tmp1->type, tmp1->line, tmp1->code);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_STRUCT_ST) {
		datas *tmp1 = entry_table.datas_start;
		if (tmp1 == 0) return;
		printf("=====Print struct_struct :\n");
		for (;;) {
			printf("(id:%li,fid:%li,len:%i),name:%s,params:%s\n", tmp1->id, tmp1->fid, tmp1->params_len, tmp1->name,
			       print_str_list(tmp1->params, tmp1->params_len));
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_STRU_ST) {
		blst *tmp1 = entry_table.blst_stru_start;
		if (tmp1 == 0) return;
		printf("=====Print block_structure_struct :\n");
		for (;;) {
			printf("id:%li,lbl[%i]:%s,(fid:%li,sid:%li),argvs:%s\n", tmp1->id, tmp1->type, tmp1->lbl, tmp1->func_id,
			       tmp1->stru_id, print_str_list(tmp1->params, tmp1->params_len));
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_STRUCT_DES_ST) {
		stde *tmp1 = entry_table.stde_start;
		if (tmp1 == 0) return;
		printf("=====Print struct descriptor :\n");
		for (;;) {
			printf("%i(%s):\n", tmp1->id, tmp1->type);
			print_vaar(tmp1->st);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
	else if (which == 0 || which == PRINT_MAGIC_MACROS_ST) {
		mama *tmp1 = entry_table.mama_start;
		if (tmp1 == 0) return;
		printf("=====Print magic macros :\n");
		for (;;) {
			printf("[id:%i,type:%i]%c;%s:%s\n", tmp1->id, tmp1->type, tmp1->sub_type, tmp1->key, tmp1->value);
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
		printf("=====End printed\n");
	}
}

//*************************************************************
uint32 source_paths_search(str_utf8 path)
{
	for (uint32 i = 0; i < entry_table.source_counter; i++) {
		if (utf8_str_equal(path, source_paths[i]))
			return i;
	}
	return 0;
}

//*************************************************************
utst return_utf8_string_value(String s)
{
	utst nil = {0, 0, 0};
	uint32 len = str_length(s);
	String val = 0;
	if (len > 2 && s[0] == '\"' && s[len - 1] == '\"') {
		val = str_substring(s, 1, len - 1);
	}
	else {
		str_init(&val, s);
	}
	//------------
	// printf("OOOOO:%s,%s,%i\n", s, val, str_is_int32(val));
	if (str_is_int32(val)) {
		int32 utst_id = str_to_int32(val);
		//printf("OOOOOO:%i\n", utst_id);
		utst ut = get_utst((long_int) utst_id);//TODO:
		return ut;
		//printf("UTT:%s=>%s,%i\n", s, utf8_to_unicode_str(ut.utf8_string), ut.max_bytes_per_char);
		//utf8_str_print("UTF8", ut.utf8_string, false);
	}
	else
		return nil;

}

//*************************************************************
void str_list_append(str_list *s, String s1, uint32 len)
{
	//printf("CCCC:%s,%i\n",s1,len);
	str_list tmp = 0;
	str_list_init(&tmp, *s, len);
	//if(*s!=NULL)free(*s);
	(*s) = (str_list) malloc((len + 1) * sizeof(str_list));
	if (*s == 0)return;
	for (uint32 i = 0; i < len; i++) {
		(*s)[i] = tmp[i];
	}
	(*s)[len] = str_append(0, s1);
	//printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}

//*************************************************************
void str_list_init(str_list *s, str_list val, uint32 len)
{
	if (len == 0) {
		(*s) = 0;
		return;
	}
	free(*s);
	(*s) = (str_list) malloc(len * sizeof(str_list));
	for (uint32 i = 0; i < len; i++) {
		//str_init(&(*s)[i],val[i]);
		uint32 le = str_length(val[i]);
		(*s)[i] = (String) malloc((le + 1) * sizeof(String));
		for (uint32 ii = 0; ii < le; ii++) (*s)[i][ii] = val[i][ii];
		(*s)[i][le] = 0;
	}
}

//*************************************************************
void str_list_delete_first(str_list *s, uint32 len)
{
	if (len == 0) {
		(*s) = 0;
		return;
	}
	str_list tmp = 0;
	str_list_init(&tmp, (*s), len);
	free((*s));
	(*s) = (str_list) malloc((len - 1) * sizeof(str_list));
	uint32 ind = 0;
	for (uint32 i = 1; i < len; i++) {
		//str_init(&(*s)[i],val[i]);
		str_init(&(*s)[ind++], tmp[i]);
	}
	free(tmp);
}

//*************************************************************
uint32 str_list_join(str_list *s, str_list a1, uint32 a1_len, str_list a2, uint32 a2_len)
{
	//if(s!=0)free(*s);
	uint32 ind = 0;
	(*s) = (str_list) malloc((a1_len + a2_len) * sizeof(str_list));
	if (*s == 0)return 0;
	for (uint32 i = 0; i < a1_len; i++) {
		str_init(&(*s)[ind++], a1[i]);
	}
	for (uint32 i = 0; i < a2_len; i++) {
		str_init(&(*s)[ind++], a2[i]);
	}
	return ind;
}

//*************************************************************
Boolean str_list_equal(str_list s1, uint32 len1, str_list s2, uint32 len2)
{
	if (len1 != len2) return false;
	if (len1 == 0)return true;
	for (uint32 i = 0; i < len1; ++i) {
		if (!str_equal(s1[i], s2[i]))return false;
	}
	return true;
}

//*************************************************************
void utf8_str_list_append(utf8_str_list *s, str_utf8 s1, uint32 len)
{
	//printf("CCCC:%s,%i\n",s1,len);
	utf8_str_list tmp = 0;
	utf8_str_list_init(&tmp, *s, len);
	free(*s);
	(*s) = (utf8_str_list) malloc((len + 1) * sizeof(utf8_str_list));
	if (*s == 0)return;
	for (uint32 i = 0; i < len; i++) {
		(*s)[i] = tmp[i];
	}
	(*s)[len] = utf8_str_append(0, s1);
	//printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}

//*************************************************************
String remove_incorrect_pars(String str, int32 *invalid_pars)
{
	String final = 0;
	str_init(&final, str);
	Boolean is_string = false;
	int32 pars = 0;
	for (uint32 i = 0; i < str_length(str); i++) {
		if (str[i] == '\"' && (i == 0 || str[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		if (!is_string) {
			switch (str[i]) {
				case '(':
					pars++;
					break;
				case ')':
					pars--;
					break;
			}
		}
	}
	//*********************
	uint32 final_len = str_length(final);
	if (pars > 0 && pars < final_len) {
		final = str_substring(final, pars, 0);
	}
	else if (pars < 0 && (-pars) < final_len) {
		final = str_substring(final, 0, final_len - (-pars));
	}
	//msg("&***", pars)
	(*invalid_pars) = pars;
	return final;

}

//*************************************************************
FILE *utf8_file_open(String filename, String mode)
{
	#if LINUX_PLATFORM == 1
	//TODO:utf8 fopen for linux
	#elif WINDOWS_PLATFORM == 1
	wchar_t wfile[260];
	wchar_t wmode[32];
	MultiByteToWideChar(CP_UTF8, 0, filename, -1, wfile, 260);
	MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, 32);
	return _wfopen(wfile, wmode);
	#endif
	return NULL;
}

//*************************************************************
String replace_in_expression(String exp, String rep, int32 start, int32 end, Boolean remove_pars, Boolean is_trim)
{
	//msg("&@@@", exp, rep, start, end)
	String s1 = 0, s2 = 0, expression = 0;
	if (is_trim) {
		s1 = str_trim_space(str_substring(exp, 0, start));
		s2 = str_trim_space(str_substring(exp, end, 0));
	}
	else {
		s1 = str_substring(exp, 0, start);
		s2 = str_substring(exp, end, 0);
	}
	uint32 s1_len = str_length(s1);
	if (remove_pars && s1_len > 0 && s1[s1_len - 1] == '(' && s2[0] == ')') {
		s1 = str_substring(s1, 0, s1_len - 1);
		s2 = str_substring(s2, 1, 0);
	}
	expression = str_multi_append(s1, rep, s2, 0, 0, 0);
	return expression;
}

//*************************************************************
String make_valid_double(String s)
{
	uint32 s_len = str_length(s);
	if (s_len == 0 || (s_len == 1 && s[0] == '.'))return "0";
	if (s_len > 1 && s[0] == '.')return str_multi_append("0", s, 0, 0, 0, 0);
	if (s_len > 1 && s[s_len - 1] == '.')return str_substring(s, 0, s_len - 1);
	return s;
}

//*************************************************************
String limit_decimal_huge_numbers(String s)
{
	uint32 s_len = str_length(s);
	uint32 ind = 0, limit = 0;
	Boolean is_point = false;
	for (uint32 i = 0; i < s_len; i++) {
		if (s[i] == '.') {
			is_point = true;
			continue;
		}
		if (is_point) {
			limit++;
			if (limit == max_decimal_has_huge) {
				ind = i + 1;
				break;
			}
		}
	}
	if (ind == 0)ind = s_len;
	return str_substring(s, 0, ind);
}

//*************************************************************
Boolean is_equal_arrays_indexes(String s1, String s2)
{
	//printf("#WWWWW:%s@@%s\n",s1,s2);
	if ((s1 == 0 && str_equal(s2, "0")) || (s2 == 0 && str_equal(s1, "0")))return true;
	str_list sl1 = 0, sl2 = 0;
	uint32 sl1_len = char_split(s1, ',', &sl1, true);
	uint32 sl2_len = char_split(s2, ',', &sl2, true);
	if (sl1_len != sl2_len)return false;
	for (uint32 i = 0; i < sl1_len; ++i) {
		if (str_ch_equal(sl1[i], '?') || str_ch_equal(sl2[i], '?'))continue;
		if (!str_equal(sl1[i], sl2[i]))return false;
	}
	return true;
}
//*************************************************************
Boolean has_two_limiting(String s, uint8 l1, uint8 l2, Boolean ignore_space)
{
	if (ignore_space)s = str_trim_space(s);
	uint32 len = str_length(s);
	if (len < 2)return false;
	if (s[0] == l1 && s[len - 1] == l2)return true;
	return false;
}