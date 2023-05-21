/* KallistiOS ##version##

   dc/maple/vmu.h
   Copyright (C)2000-2002 Jordan DeLong, Megan Potter
   Copyright (C)2008 Donald Haase
   Copyright (C)2023 Falco Girgis

*/

/** \file    dc/maple/vmu.h
    \brief   Definitions for using the VMU device.
    \ingroup vmu

    This file contains the definitions needed to access the Maple VMU device.
    This includes all of the functionality of memory cards, including the
    MAPLE_FUNC_MEMCARD, MAPLE_FUNC_LCD, and MAPLE_FUNC_CLOCK function codes.

    \author Jordan DeLong
    \author Megan Potter
    \author Donald Haase
    \author Falco Girgis
*/

#ifndef __DC_MAPLE_VMU_H
#define __DC_MAPLE_VMU_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <dc/maple.h>

#include <stdint.h>
#include <time.h>

/** \defgroup vmu Visual Memory Unit
    \brief    VMU/VMS Maple Peripheral API

    @{
*/

/**
    \brief Pixel width of VMU screen
*/
#define VMU_SCREEN_WIDTH    48

/**
    \brief Pixel height of VMU screen
*/
#define VMU_SCREEN_HEIGHT   32

/** \brief   Enable custom color of a VMU

    This function enables/disables the custom color of a specific VMU. 
    This color is only displayed in the Dreamcast's file manager.

    \param  dev             The device to enable custom color.
    \param  enable          Values other than 0 enables. Equal to 0 disables.\

    \retval 0               On success
    \retval -1              On failure

    \sa vmu_set_custom_color
*/
int vmu_use_custom_color(maple_device_t *dev, int enable);

/** \brief  Set custom color of a VMU

    This function sets the custom color of a specific VMU. This color is only
    displayed in the Dreamcast's file manager. This function also enables the 
    use of the custom color. Otherwise it wouldn't show up.

    \param  dev             The device to change the color of.
    \param  red             The red component. 0-255
    \param  green           The green component. 0-255
    \param  blue            The blue component. 0-255
    \param  alpha           The alpha component. 0-255; 100-255 Recommended

    \retval 0               On success
    \retval -1              On failure
*/
int vmu_set_custom_color(maple_device_t *dev, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

/** \brief  Set icon shape of a VMU

    This function sets the icon shape of a specific VMU. The icon shape is a 
    vmu icon that is displayed on the LCD screen while navigating the Dreamcast
    BIOS menu and is the GUI representation of the VMU in the menu's file manager. 
    The Dreamcast BIOS provides a set of 124 icons to choose from.

    \param  dev             The device to change the icon shape of.
    \param  icon_shape      One of the values found in \ref{vmu_icons}.

    \retval 0               On success
    \retval -1              On failure

    \sa vmu_icons
*/
int vmu_set_icon_shape(maple_device_t *dev, uint8_t icon_shape);

/** \brief  Make a VMU beep.

    This function sends a raw beep to a VMU, causing the speaker to emit a tone
    noise. See http://dcemulation.org/phpBB/viewtopic.php?f=29&t=97048 for the
    original information about beeping.

    \warning
    This function is now deprecated. Prefer vmu_beep, which handles the known
    values correctly for parameters.

    \param  dev             The device to attempt to beep.
    \param  beep            The tone to generate. Actual parameters unknown.

    \retval MAPLE_EOK       On success.
    \retval MAPLE_EAGAIN    If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.

    \sa vmu_beep
*/
int vmu_beep_raw(maple_device_t* dev, uint32_t beep) __attribute__((deprecated));

/** \brief  Play VMU Buzzer tone.

    Send a waveform to be played by the VMU's piezoelectric buzzer. The waveform
    is a square-wave configured with the following parameters:

                   Period
            +--------------------+            
            |                    |
                       __________            __________
                      |          |          |          |
                      |          |          |          |
            __________|          |__________|          |

            |         |
            +---------+        WAVEFORM
        Inverse Pulse Width

    \warning
    Due to physical characteristics of the VMU's buzzer, it is unable to produce
    tones in all frequency ranges with full volume. The recommended range is
    170 Hz to 2.7 kHz.

    \note
    On the VMU-side, this tone is generated using the VMU's Timer1 peripheral
    as a pulse generator.

    \param  period             The total period of the waveform
    \param  inversePulseWidth  The width of the inactive segment of the waveform

    \retval MAPLE_EOK           On success.
    \retval MAPLE_EAGAIN        If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT      If the command timed out while blocking.
*/
int vmu_beep(maple_device_t *dev, uint8_t period, uint8_t inversePulseWidth);

/** \brief  Display a 1bpp bitmap on a VMU screen.

    This function sends a raw bitmap to a VMU to display on its screen. This
    bitmap is 1bpp, and is 48x32 in size.

    \param  dev             The device to draw to.
    \param  bitmap          The bitmap to show.

    \retval MAPLE_EOK       On success.
    \retval MAPLE_EAGAIN    If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.

    \sa vmu_draw_lcd_xbm, vmu_set_icon
*/
int vmu_draw_lcd(maple_device_t *dev, void *bitmap);

/** \brief  Display a Xwindows XBM image on a VMU screen.

    This function takes in a Xwindows XBM, converts it to a raw bitmap, and sends 
    it to a VMU to display on its screen. This XBM image is 48x32 in size.

    \param  dev             The device to draw to.
    \param  vmu_icon        The icon to set.

    \retval MAPLE_EOK       On success.
    \retval MAPLE_EAGAIN    If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.

    \sa vmu_draw_lcd
*/
int vmu_draw_lcd_xbm(maple_device_t *dev, const char *vmu_icon);

/** \brief  Display a Xwindows XBM on all VMUs.

    This function takes in a Xwindows XBM and displays the image on all VMUs.

    \note
    This is a convenience function for vmu_draw_lcd() to broadcast across all VMUs.

    \param  vmu_icon        The icon to set.

    \sa vmu_draw_lcd
*/
void vmu_set_icon(const char *vmu_icon);

/** \brief  Read a block from a memory card.

    This function reads a raw block from a memory card. 

    \note
    You most likely will not ever use this directly, but rather will
    probably use the fs_vmu stuff.

    \param  dev             The device to read from.
    \param  blocknum        The block number to read.
    \param  buffer          The buffer to read into (512 bytes).

    \retval MAPLE_EOK       On success.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
    \retval MAPLE_EFAIL     On errors other than timeout.

    \sa vmu_block_write
*/
int vmu_block_read(maple_device_t *dev, uint16_t blocknum, uint8_t *buffer);

/** \brief  Write a block to a memory card.

    This function writes a raw block to a memory card.

    \note
    You most likely will not ever use this directly, but rather will
    probably use the fs_vmu stuff.

    \param  dev             The device to write to.
    \param  blocknum        The block number to write.
    \param  buffer          The buffer to write from (512 bytes).

    \retval MAPLE_EOK       On success.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
    \retval MAPLE_EFAIL     On errors other than timeout.

    \sa vmu_block_read
*/
int vmu_block_write(maple_device_t *dev, uint16_t blocknum, const uint8_t *buffer);


int vmu_set_datetime(maple_device_t *dev, time_t time);
int vmu_get_datetime(maple_device_t *dev, time_t *time);

/* VMU's button state/cond values, same as capability values */
#define VMU_DPAD_UP    (1<<0)
#define VMU_DPAD_DOWN  (1<<1)
#define VMU_DPAD_LEFT  (2<<1)
#define VMU_DPAD_RIGHT (3<<1)
#define VMU_A          (4<<1)
#define VMU_B          (5<<1)
#define VMU_MODE       (6<<1)
#define VMU_SLEEP      (7<<1)

/* VMU's raw condition data: 0 = PRESSED, 1 = RELEASED */
typedef uint8_t vmu_cond_t;
/* VMU's "civilized" state data: 0 = RELEASED, 1 = PRESSED */
typedef vmu_cond_t vmu_state_t;

void vmu_set_buttons_enabled(maple_device_t * dev, int enable);
int vmu_get_buttons_enabled(void);

/** @} */

/* \cond */
/* Init / Shutdown -- Managed internally by KOS */
int vmu_init(void);
void vmu_shutdown(void);
/* \endcond */

__END_DECLS

#endif  /* __DC_MAPLE_VMU_H */

