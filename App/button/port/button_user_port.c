#include "button_port.h"
#include "button_config.h"

/* Port (platform) includes */
#include "main.h"
#include "keyboard.h"

enum {
//    BTN_1_MASK = (1 << 3),
//    BTN_2_MASK = (1 << 2),
//    BTN_3_MASK = (1 << 1),
//    BTN_4_MASK = (1 << 0),
//    BTN_5_MASK = (1 << 7),
//    BTN_6_MASK = (1 << 6),
//    BTN_7_MASK = (1 << 5),
//    BTN_8_MASK = (1 << 4),
//    BTN_9_MASK = (1 << 11),
//    BTN_0_MASK = (1 << 10),
//    BTN_STAR_MASK = (1 << 9),
//    BTN_OK_MASK = (1 << 8)
	BTN_0_MASK = 0,
	BTN_1_MASK,
	BTN_2_MASK,
	BTN_3_MASK,
	BTN_4_MASK,
	BTN_5_MASK,
	BTN_6_MASK,
	BTN_7_MASK,
	BTN_8_MASK,
	BTN_9_MASK,
	BTN_DOT_MASK,
	BTN_EQUAL_MASK,
	BTN_ADD_MASK,
	BTN_SUBSTRACT_MASK,
	BTN_MULTIPLY_MASK,
	BTN_DIVIDE_MASK
} keyboard_btn_mask;

uint16_t buttons_state = 0;

void BTN_PL_Init(uint16_t id){
	buttons_state = 0;	/* All buttons is not pressed */
}

bool BTN_PL_Read(uint16_t id){
    switch(id){
        case BTN_1_ID: return (buttons_state & BTN_0_MASK)?(true):(false);
        case BTN_2_ID: return (buttons_state & BTN_1_MASK)?(true):(false);
        case BTN_3_ID: return (buttons_state & BTN_2_MASK)?(true):(false);
        case BTN_4_ID: return (buttons_state & BTN_3_MASK)?(true):(false);
        case BTN_5_ID: return (buttons_state & BTN_4_MASK)?(true):(false);
        case BTN_6_ID: return (buttons_state & BTN_5_MASK)?(true):(false);
        case BTN_7_ID: return (buttons_state & BTN_6_MASK)?(true):(false);
        case BTN_8_ID: return (buttons_state & BTN_7_MASK)?(true):(false);
        case BTN_9_ID: return (buttons_state & BTN_8_MASK)?(true):(false);
        case BTN_0_ID: return (buttons_state & BTN_9_MASK)?(true):(false);
        case BTN_DOT_ID: return (buttons_state & BTN_DOT_MASK)?(true):(false);
        case BTN_EQUAL_ID: return (buttons_state & BTN_EQUAL_MASK)?(true):(false);
        case BTN_ADD_ID: return (buttons_state & BTN_ADD_MASK)?(true):(false);
        case BTN_SUBSTRACT_ID: return (buttons_state & BTN_SUBSTRACT_MASK)?(true):(false);
        case BTN_MULTIPLY_ID: return (buttons_state & BTN_MULTIPLY_MASK)?(true):(false);
        case BTN_DIVIDE_ID: return (buttons_state & BTN_DIVIDE_MASK)?(true):(false);
        default:
            return false;
    }

    return false;
}

//uint32_t line_table[3] = {KBD_A_Pin, KBD_B_Pin, KBD_C_Pin};
//
//void KBD_PL_PerformScan(void){
//    uint16_t tmp = 0;
//
//    /* Scan keyboard in fast way */
//    for(uint16_t line = 0; line < 3; line++){
//        /* Reset all lines */
//        GPIOA->BSRR = KBD_A_Pin|KBD_B_Pin|KBD_C_Pin;
//
//        /* Select line */
//        GPIOA->BSRR = line_table[line] << 16;
//
//        /* Wait for a while */
//        for(volatile uint32_t wait = 0; wait < 1000; wait++);
//
//        /* Read keys state port */
//        tmp |= (GPIOB->IDR & (KBD_1_Pin|KBD_2_Pin|KBD_3_Pin|KBD_4_Pin)) >> 11;
//
//        if(line < 2){
//            tmp <<= 4;
//        }
//    }
//
//    /* Set all lines to idle */
//    GPIOA->BSRR = KBD_A_Pin|KBD_B_Pin|KBD_C_Pin;
//
//    keyboard_btn_state = ~tmp;
//}
