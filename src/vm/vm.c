#include "./vm.h"
#include <stdio.h>

void handle_input_output(uint16_t instruction){
    switch((instruction & 0x0FC0) >> 6){
        case OUTPUT:
            switch(instruction & 0x0030){
                case INTEGER:
                    printf("%d", stack[SP-1]);
                    break;
            }
            --SP;
            break;
        case ESEQ:
            switch(instruction & 0x0030){
                case NEWLINE:
                    putchar('\n');
                    break;
                case RETURN_CARRIAGE:
                    putchar('\r');
                    break;
            }
            break;
    }
}

void handle_arithmetic(uint16_t instruction){
    switch(instruction & 0x0FFF){
        case ADD:
            stack[SP-2] = (stack[SP-1] + stack[SP-2]);
            break;
        case SUB:
            stack[SP-2] = (stack[SP-1] - stack[SP-2]);
            break;
        case MULT:
            stack[SP-2] = (stack[SP-1]*stack[SP-2]);
            break;
        case DIV:
            stack[SP-2] = (uint16_t) (stack[SP-1]/stack[SP-2]);
            break;
    }
}

void handle_reg_storage(uint16_t reg_index){
    switch(reg_index){
        case Ra:
        case Rb:
        case Rc:
        case Rip:
        case Rcbindx:
            reg_data[reg_index] = stack[SP--];
            break;
        case Rhlt:
        case Rerr:
        case Rcom:
            reg_data[Rerr] = ILLEGAL_PARAMETER;
            break;
    }
}

void execute_instruction(uint16_t instruction){

    switch(OPCODE(instruction)){
        case PUSH:
            stack[SP++] = (instruction & 0x0FFF);
            break;
        case POP:
            reg_data[Ra] = stack[--SP];
            break;
        case DUP:
            stack[SP] = stack[SP-1];
            ++SP;
            break;
        case ARITH:
            handle_arithmetic(instruction);
            --SP;
            break;
        case AND:
            stack[SP-2] = (stack[SP-1] & stack[SP-2]);
            --SP;
            break;
        case OR:
            stack[SP-2] = (stack[SP-1] | stack[SP-2]);
            --SP;
            break;
        case NOT:
            stack[SP-1] = ~stack[SP-1];
            break;
        case STORER:
            handle_reg_storage(instruction & 0x0FFF);
            break;
        case LOADR:
            stack[SP++] = reg_data[instruction & 0xFFF];
            break;
        case IO:
            handle_input_output(instruction);
            break;
        case HALT:
            reg_data[Rhlt] = TRUE;
            break;
    }

    if ((reg_data[Rcbindx] + ++IP) >= MEM_CELL_COUNT/2){
        reg_data[Rhlt] = TRUE;
    }

}

void run_machine(){
    while (MACHINE_IS_RUNNING){
        execute_instruction(code_store[CODE_BASE_INDEX + IP]);
    }
}
