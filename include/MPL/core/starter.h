//
// Created by amindelavar on 9/21/2018.
//

#ifndef MPL_STARTER_H
#define MPL_STARTER_H

Boolean start_runtime();

int8 APP_CONTROLLER();
int8 INSTRUCTION_EXECUTOR(long_int index);
Boolean init_global_vars();
void exit_runtime(int32 i);
#endif //MPL_STARTER_H
