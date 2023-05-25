/* KallistiOS ##version##

   dc/maple/controller.h
   (C) 2000-2002 Jordan DeLong, Megan Potter
   (C) 2023 Falco Girgis

   Thanks to Marcus Comstedt for information on the controller.
*/

/** \file    dc/maple/controller.h
    \brief   Definitions for using the controller device.
    \ingroup controller

    This file contains the definitions needed to access the Maple controller
    device. Obviously, this corresponds to the MAPLE_FUNC_CONTROLLER function
    code.

    \author Jordan DeLong
    \author Megan Potter
    \author Falco Girgis
*/

#ifndef __DC_MAPLE_CONTROLLER_H
#define __DC_MAPLE_CONTROLLER_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <stdint.h>

/** \defgroup controller Controller
    \brief    Controller Maple Device API
    \ingroup  maple

    This module contains the public API for the controller
    maple driver.

    A standard, first-party Dreamcast controller has
    the following button configuration:

                                ___________
                               / |  __  |  \
           L trigger -----|   /  | |  | |   \  |----- R trigger
                         _|__/   | |__| |    \_|__
                        |  _      \____/   _   _ |
         Joystick   ----|-/ \             |X| |Y||
                        | \_/              _   _ |
                        |  _              |A| |B||
                        |_| |_             ¯   ¯ |
             D-Pad -----|_   _|                  |
                        | |_|       /\           |
                         \         /__\          /
                          \    _____|_______    /
                           \  /     |       \  /
                            \/      |        \/
                               Start button
*/

/** \defgroup controller_buttons  Button Masks
    \ingroup  controller

    The set of bitmasks for all valid maple controller button states.
    @{
*/
#define CONT_C              (1<<0)      /**< \brief C button Mask. */
#define CONT_B              (1<<1)      /**< \brief B button Mask. */
#define CONT_A              (1<<2)      /**< \brief A button Mask. */
#define CONT_START          (1<<3)      /**< \brief Start button Mask. */
#define CONT_DPAD_UP        (1<<4)      /**< \brief Main Dpad Up button Mask. */
#define CONT_DPAD_DOWN      (1<<5)      /**< \brief Main Dpad Down button Mask. */
#define CONT_DPAD_LEFT      (1<<6)      /**< \brief Main Dpad Left button Mask. */
#define CONT_DPAD_RIGHT     (1<<7)      /**< \brief Main Dpad right button Mask. */
#define CONT_Z              (1<<8)      /**< \brief Z button Mask. */
#define CONT_Y              (1<<9)      /**< \brief Y button Mask. */
#define CONT_X              (1<<10)     /**< \brief X button Mask. */
#define CONT_D              (1<<11)     /**< \brief D button Mask. */
#define CONT_DPAD2_UP       (1<<12)     /**< \brief Secondary Dpad Up button Mask. */
#define CONT_DPAD2_DOWN     (1<<13)     /**< \brief Secondary Dpad Down button Mask. */
#define CONT_DPAD2_LEFT     (1<<14)     /**< \brief Secondary Dpad Left button Mask. */
#define CONT_DPAD2_RIGHT    (1<<15)     /**< \brief Secondary Dpad Right button Mask. */
/** @} */

/** \brief   Controller buttons for standard reset action
    \ingroup controller
    
    Convenience macro combining together all of the masks for buttons
    used by most games as a reset mechanism:
        * CONT_A
        * CONT_B
        * CONT_X
        * CONT_Y
        * CONT_START
*/
#define CONT_RESET_BUTTONS  (CONT_A | CONT_B | CONT_X | CONT_Y | CONT_START)

/** \brief   Controller state structure.
    \ingroup controller

    This structure contains information about the status of the controller
    device and can be fetched by casting the result of maple_dev_status() to
    this structure.

    A 1 bit in the buttons' bitfield indicates that a button is pressed, and the
    joyx, joyy, joy2x, joy2 values are all 0 based (0 is centered).

    \sa maple_dev_status
*/
typedef struct cont_state {
    union {
        /** \brief  bit-packed controller button states
            \sa     controller_buttons
        */
        uint32_t buttons;
        struct {
            uint32_t c: 1;              /**< \brief C button value. */
            uint32_t b: 1;              /**< \brief B button value. */
            uint32_t a: 1;              /**< \brief A button value. */
            uint32_t start: 1;          /**< \brief Start button value. */
            uint32_t dpad_up: 1;        /**< \brief Main Dpad Up button value. */
            uint32_t dpad_down: 1;      /**< \brief Main Dpad Down button value. */
            uint32_t dpad_left: 1;      /**< \brief Main Dpad Left button value. */
            uint32_t dpad_right: 1;     /**< \brief Main Dpad Right button value. */
            uint32_t z: 1;              /**< \brief Z button value. */
            uint32_t y: 1;              /**< \brief Y button value. */
            uint32_t x: 1;              /**< \brief X button value. */
            uint32_t d: 1;              /**< \brief D button value. */
            uint32_t dpad2_up: 1;       /**< \brief Secondary Dpad Up button value. */
            uint32_t dpad2_down: 1;     /**< \brief Secondary Dpad Down button value. */
            uint32_t dpad2_left: 1;     /**< \brief Secondary Dpad Left button value. */
            uint32_t dpad2_right: 1;    /**< \brief Secondary Dpad Right button value. */
            uint32_t unused: 16;
        };
    };

    int ltrig;    /**< \brief Left trigger value (0-255). */
    int rtrig;    /**< \brief Right trigger value (0-255). */
    int joyx;     /**< \brief Main joystick x-axis value. (-128 - 127) */
    int joyy;     /**< \brief Main joystick y-axis value. */
    int joy2x;    /**< \brief Secondary joystick x-axis value (if applicable). */
    int joy2y;    /**< \brief Secondary joystick y-axis value (if applicable). */
} cont_state_t;

/** \defgroup controller_caps Capability Masks
    \ingroup  controller

    These bits will be set in the function_data for the controller's deviceinfo
    if the controller supports the corresponding feature.

    @{
*/
#define CONT_CAPABILITY_C               (1<<24)     /**< \brief C button capability mask. */
#define CONT_CAPABILITY_B               (1<<25)     /**< \brief B button capability mask. */
#define CONT_CAPABILITY_A               (1<<26)     /**< \brief A button capability mask. */
#define CONT_CAPABILITY_START           (1<<27)     /**< \brief Start button capability mask. */
#define CONT_CAPABILITY_DPAD_UP         (1<<28)     /**< \brief First Dpad up capability mask. */
#define CONT_CAPABILITY_DPAD_DOWN       (1<<29)     /**< \brief First Dpad down capability mask. */
#define CONT_CAPABILITY_DPAD_LEFT       (1<<30)     /**< \brief First Dpad left capability mask. */
#define CONT_CAPABILITY_DPAD_RIGHT      (1<<31)     /**< \brief First Dpad right capability mask. */
#define CONT_CAPABILITY_Z               (1<<16)     /**< \brief Z button capability mask. */
#define CONT_CAPABILITY_Y               (1<<17)     /**< \brief Y button capability mask. */
#define CONT_CAPABILITY_X               (1<<18)     /**< \brief X button capability mask. */
#define CONT_CAPABILITY_D               (1<<19)     /**< \brief D button capability mask. */
#define CONT_CAPABILITY_DPAD2_UP        (1<<20)     /**< \brief Second Dpad up capability mask. */
#define CONT_CAPABILITY_DPAD2_DOWN      (1<<21)     /**< \brief Second Dpad down capability mask. */
#define CONT_CAPABILITY_DPAD2_LEFT      (1<<22)     /**< \brief Second Dpad left capability mask. */
#define CONT_CAPABILITY_DPAD2_RIGHT     (1<<23)     /**< \brief Second Dpad right capability mask. */
#define CONT_CAPABILITY_RTRIG           (1<<8)      /**< \brief Right trigger capability mask. */
#define CONT_CAPABILITY_LTRIG           (1<<9)      /**< \brief Left trigger capability mask. */
#define CONT_CAPABILITY_ANALOG_X        (1<<10)     /**< \brief First analog X axis capability mask. */
#define CONT_CAPABILITY_ANALOG_Y        (1<<11)     /**< \brief First analog Y axis capability mask. */
#define CONT_CAPABILITY_ANALOG2_X       (1<<12)     /**< \brief Second analog X axis capability mask. */
#define CONT_CAPABILITY_ANALOG2_Y       (1<<13)     /**< \brief Second analog Y axis capability mask. */


#define CONT_CAPABILITY_STANDARD_BUTTONS    (CONT_CAPABILITY_A | \
                                             CONT_CAPABILITY_B | \
                                             CONT_CAPABILITY_X | \
                                             CONT_CAPABILITY_Y | \
                                             CONT_CAPABILITY_START)

#define CONT_CAPABILITY_DPAD                (CONT_CAPABILITY_DPAD_UP   | \
                                             CONT_CAPABILITY_DPAD_DOWN | \
                                             CONT_CAPABILITY_DPAD_LEFT | \
                                             CONT_CAPABILITY_DPAD_RIGHT)

#define CONT_CAPABILITY_ANALOG              (CONT_CAPABILITY_ANALOG_X | \ 
                                             CONT_CAPABILITY_ANALOG_Y) 

#define CONT_CAPABILITY_TRIGGERS            (CONT_CAPABILITY_LTRIG | \
                                             CONT_CAPABILITY_RTRIG)

#define CONT_CAPABILITY_EXTENDED_BUTTONS    (CONT_CAPABILITY_C | \
                                             CONT_CAPABILITY_Z)

#define CONT_CAPABILITY_SECONDARY_DPAD      (CONT_CAPABILITY_DPAD2_UP   | \ 
                                             CONT_CAPABILITY_DPAD2_DOWN | \
                                             CONT_CAPABILITY_DPAD2_LEFT | \
                                             CONT_CAPABILITY_DPAD2_RIGHT)

#define CONT_CAPABILITY_SECONDARY_ANALOG    (CONT_CAPABILITY_ANALOG2_X | \
                                             CONT_CAPABILITY_ANALOG2_Y)

#define CONT_CAPABILITY_DUAL_DPAD           (CONT_CAPABILITIES_DPAD | \
                                             CONT_CAPABILITIES_SECONDARY_DPAD)

#define CONT_CAPABILITY_DUAL_ANALOG         (CONT_CAPABILITIES_ANALOG | \
                                             CONT_CAPABILITIES_SECONDARY_ANALOG)

/** @} */

#define CONT_TYPE_STANDARD_CONTROLLER       (CONT_CAPABILITY_STANDARD_BUTTONS | \
                                             CONT_CAPABILITY_TRIGGERS | \
                                             CONT_CAPABLITY_DPAD | \
                                             CONT_CAPABILITY_ANALOG)

#define CONT_TYPE_DUAL_ANALOG_CONTROLLER    (CONT_CAPABILITY_STANDARD_BUTTONS | \
                                             CONT_CAPABILITY_TRIGGERS | \
                                             CONT_CAPABLITY_DPAD | \
                                             CONT_CAPABILITY_DUAL_ANALOG)

#define CONT_TYPE_ASCII_PAD                 (CONT_CAPABILITY_STANDARD_BUTTONS | \
                                             CONT_CAPABILITY_EXTENDED_BUTTONS | \
                                             CONT_CAPABILITY_DPAD)

#define CONT_TYPE_ARCADE_STICK              (CONT_CAPABILITY_STANDARD_BUTTONS | \
                                             CONT_CAPABILITY_EXTENDED_BUTTONS | \
                                             CONT_CAPABILITY_DPAD)

#define CONT_TYPE_TWIN_STICK                (CONT_CAPABILITY_STANDARD_BUTTONS | \ 
                                             CONT_CAPABILITY_EXTENDED_BUTTONS | \
                                             CONT_CAPABILITY_DUAL_DPAD)

// Require research
#define CONT_TYPE_RACING_CONTROLLER
#define CONT_TYPE_MARACAS
#define CONT_TYPE_FISHING_ROD
#define CONT_TYPE_POP_N_MUSIC
#define CONT_TYPE_DENSHA_DE_GO

struct maple_device;

// Conroller has EXACTLY the given capabilties
int cont_is_type(const struct maple_device_t *cont, uint32_t type);

// Controller has at LEAST the given capabilities
int cont_has_capabilities(const struct maple_device_t *cont, uint32_t capability_mask);

/** \brief   Controller automatic callback type.
    \ingroup controller

    Functions of this type can be set with cont_btn_callback() to respond
    automatically to the specified set of buttons being pressed. This can be
    used, for instance, to implement the standard A+B+X+Y+Start method of ending
    the program running.

    \warning
    Your callback will be invoked within a context with interrupts disabled.
    See cont_btn_callback for more information.

    \sa cont_btn_callback
*/
typedef void (*cont_btn_callback_t)(uint8_t addr, uint32_t btns);

/** \brief   Set an automatic button press callback.
    \ingroup controller

    This function sets a callback function to be called when the specified
    controller has the set of buttons given pressed.

    \note 
    The callback gets invoked for the given maple port; however, providing
    an address of '0' will cause it to be invoked for any port with a 
    device pressing the given buttons. Since you are passed back the address 
    of this device, You are free to implement your own filtering logic within
    your callback.

    \warning
    The provided callback function is invoked within a context which has
    interrupts disabled. This means that you should not do any sort of complex
    processing or make any API calls which depend on interrupts to complete,
    such as Maple or etherent processing whichy rely on packet transmission,
    any sleeping or threading calls, blocking on any sort of file I/O, etc. 
    This mechanism is typically used to quickly terminate the application
    and should be used with caution.

    \param  addr            The controller to listen on (or 0 for all ports). 
                            This value can be obtained by using maple_addr().
    \param  btns            The buttons bitmask to match.
    \param  cb              The callback to call when the buttons are pressed.
*/
void cont_btn_callback(uint8_t addr, uint32_t btns, cont_btn_callback_t cb);

/* \cond */
/* Init / Shutdown */
int cont_init(void);
void cont_shutdown(void);
/* \endcond */

__END_DECLS

#endif  /* __DC_MAPLE_CONTROLLER_H */

