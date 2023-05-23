/* KallistiOS ##version##

   dc/maple/controller.h
   (C)2000-2002 Jordan DeLong, Megan Potter
   (C)2023 Falco Girgis

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
    \ingroup maple

    This module contains the public API for the controller
    maple driver.

    A standard, first-party Dreamcast controller has
    the following button configuration:

                           ___________
                          / |  __  |  \
       L button -----|   /  | |  | |   \  |----- R button
                    _|__/   | |__| |    \_|__
                   |  _      \____/   _   _ |
    Analog Pad ----|-/ \             |X| |Y||
                   | \_/              _   _ |
                   |  _              |A| |B||
                   |_| |_             ¯   ¯ |
        D-Pad -----|_   _|                  |
                   | |_|      /\            |
                    \        /__\          /
                     \    ____|________   /
                      \  /    |       \  /
                       \/     |        \/
                              |
                          Start button
*/

/** \defgroup controller_buttons  Controller Button Masks
    \ingroup  controller

    The set of bitmasks for all valid maple controller button states.
    @{
*/
#define CONT_C              (1<<0)      /** \brief C button Mask. */
#define CONT_B              (1<<1)      /** \brief B button Mask. */
#define CONT_A              (1<<2)      /** \brief A button Mask. */
#define CONT_START          (1<<3)      /** \brief Start button Mask. */
#define CONT_DPAD_UP        (1<<4)      /** \brief Left Dpad Up button Mask. */
#define CONT_DPAD_DOWN      (1<<5)      /** \brief Left Dpad Down button Mask. */
#define CONT_DPAD_LEFT      (1<<6)      /** \brief Left Dpad Left button Mask. */
#define CONT_DPAD_RIGHT     (1<<7)      /** \brief Left Dpad right button Mask. */
#define CONT_Z              (1<<8)      /** \brief Z button Mask. */
#define CONT_Y              (1<<9)      /** \brief Y button Mask. */
#define CONT_X              (1<<10)     /** \brief X button Mask. */
#define CONT_D              (1<<11)     /** \brief D button Mask. */
#define CONT_DPAD2_UP       (1<<12)     /** \brief Right Dpad Up button Mask. */
#define CONT_DPAD2_DOWN     (1<<13)     /** \brief Right Dpad Down button Mask. */
#define CONT_DPAD2_LEFT     (1<<14)     /** \brief Right Dpad Left button Mask. */
#define CONT_DPAD2_RIGHT    (1<<15)     /** \brief Right Dpad Right button Mask. */
/** @} */

/** \brief   Controller state structure.
    \ingroup controller

    This structure contains information about the status of the controller
    device and can be fetched by casting the result of maple_dev_status() to
    this structure.

    A 1 bit in the buttons' bitfield indicates that a button is pressed, and the
    joyx, joyy, joy2x, joy2 values are all 0 based (0 is centered).

    \headerfile dc/maple/controller.h
*/
typedef struct cont_state {
    union {
        /** \brief  bit-packed controller button states
            \sa     controller_buttons
        */
        uint32_t buttons;
        struct {
            uint32_t c: 1;              /** \brief C button value. */
            uint32_t b: 1;              /** \brief B button value. */
            uint32_t a: 1;              /** \brief A button value. */
            uint32_t start: 1;          /** \brief Start button value. */
            uint32_t dpad_up: 1;        /** \brief Left Dpad Up button value. */
            uint32_t dpad_down: 1;      /** \brief Left Dpad Down button value. */
            uint32_t dpad_left: 1;      /** \brief Left Dpad Left button value. */
            uint32_t dpad_right: 1;     /** \brief Left Dpad Right button value. */
            uint32_t z: 1;              /** \brief Z button value. */
            uint32_t y: 1;              /** \brief Y button value. */
            uint32_t x: 1;              /** \brief X button value. */
            uint32_t d: 1;              /** \brief D button value. */
            uint32_t dpad2_up: 1;       /** \brief Right Dpad Up button value. */
            uint32_t dpad2_down: 1;     /** \brief Right Dpad Down button value. */
            uint32_t dpad2_left: 1;     /** \brief Right Dpad Left button value. */
            uint32_t dpad2_right: 1;    /** \brief Right Dpad Right button value. */
            uint32_t unused: 16;
        };
    };

    int ltrig;    /** \brief Left trigger value. */
    int rtrig;    /** \brief Right trigger value. */
    int joyx;     /** \brief Main joystick x-axis value. */
    int joyy;     /** \brief Main joystick y-axis value. */
    int joy2x;    /** \brief Secondary joystick x-axis value (if applicable). */
    int joy2y;    /** \brief Secondary joystick y-axis value (if applicable). */
} cont_state_t;

/** \defgroup controller_caps Controller capability bit masks
    \ingroup  controller

    These bits will be set in the function_data for the controller's deviceinfo
    if the controller supports the corresponding feature.

    @{
*/
#define CONT_CAPABILITY_C               (1<<24)
#define CONT_CAPABILITY_B               (1<<25)
#define CONT_CAPABILITY_A               (1<<26)
#define CONT_CAPABILITY_START           (1<<27)
#define CONT_CAPABILITY_DPAD_UP         (1<<28)
#define CONT_CAPABILITY_DPAD_DOWN       (1<<29)
#define CONT_CAPABILITY_DPAD_LEFT       (1<<30)
#define CONT_CAPABILITY_DPAD_RIGHT      (1<<31)
#define CONT_CAPABILITY_Z               (1<<16)
#define CONT_CAPABILITY_Y               (1<<17)
#define CONT_CAPABILITY_X               (1<<18)
#define CONT_CAPABILITY_D               (1<<19)
#define CONT_CAPABILITY_DPAD2_UP        (1<<20)
#define CONT_CAPABILITY_DPAD2_DOWN      (1<<21)
#define CONT_CAPABILITY_DPAD2_LEFT      (1<<22)
#define CONT_CAPABILITY_DPAD2_RIGHT     (1<<23)
#define CONT_CAPABILITY_RTRIG           (1<<8)
#define CONT_CAPABILITY_LTRIG           (1<<9)
#define CONT_CAPABILITY_ANALOG_X        (1<<10)
#define CONT_CAPABILITY_ANALOG_Y        (1<<11)
#define CONT_CAPABILITY_ANALOG2_X       (1<<12)
#define CONT_CAPABILITY_ANALOG2_Y       (1<<13)
/** @} */

/* Aggregate Capability Categories */
#define CONT_CAPABILITY_STANDARD
#define CONT_CAPABILITY_RIGHT_DPAD
#define CONT_CAPABILITY_RIGHT_ANALOG
#define CONT_CAPABILITY_EXTRA_BUTTONS

int cont_has_standard_capabilities(const maple_device_t *cont);

int cont_has_right_dpad(const maple_device_t *cont);

int cont_has_right_analog(const maple_device_t *cont);

int cont_has_extra_buttons(const maple_device_t *cont);

/** \brief  Controller automatic callback type.

    Functions of this type can be set with cont_btn_callback() to respond
    automatically to the specified set of buttons being pressed. This can be
    used, for instance, to implement the standard A+B+X+Y+Start method of ending
    the program running.
*/
typedef void (*cont_btn_callback_t)(uint8_t addr, uint32_t btns);

/** \brief  Set an automatic button press callback.

    This function sets a callback function to be called when the specified
    controller has the set of buttons given pressed.

    \param  addr            The controller to listen on. This value can be
                            obtained by using maple_addr().
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

