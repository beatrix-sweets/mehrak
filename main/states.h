/**
 * @brief state index
 */
typedef enum {
    STATE_SLIDESHOW = -1,   /* slideshow all images */
    STATE_IMAGE_0 = 0,     /* show first image */
    STATE_IMAGE_1 = 1,     /* show 2nd image */
    STATE_IMAGE_2 = 2,     /* show 3rd image */
    STATE_IMAGE_3 = 3,     /* show respective image */
    STATE_IMAGE_4 = 4,     /* show respective image */
    STATE_IMAGE_5 = 5,     /* show respective image */
    STATE_IMAGE_6 = 6,     /* show respective image */
    STATE_IMAGE_7 = 7,     /* show respective image */
    STATE_IMAGE_8 = 8,     /* show respective image */
    STATE_IMAGE_MAX = 6,     /* max image index in image array - change this based on the number of images you have */
} states_idx_t;

/**
 * @brief button states
 */
typedef enum {
    BUTTON_IDLE = 1,     /* button is not pressed */
    BUTTON_PRESSED = 0,  /* button is being pressed */
} button_state_t;