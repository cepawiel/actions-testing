/* KallistiOS ##version##

   dc/maple/vmu.h
   Copyright (C) 2000-2002 Jordan DeLong, Megan Potter
   Copyright (C) 2008 Donald Haase
   Copyright (C) 2023 Falco Girgis

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

    \todo
        - flesh out overall module documentation
        - prevent broadcasting an icon from sending to rear VMUs
        - implement timer for beeping waveform
        - implement capability checks
        - add submodules for different stuff VMU can do
        - implement root block queries
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
    \param  enable          Values other than 0 enables. Equal to 0 disables.

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

    \sa vmu_get_custom_color, vmu_use_custom_color
*/
int vmu_set_custom_color(maple_device_t *dev, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

/** \brief  Get custom color of a VMU

    This function gets the custom color of a specific VMU. This color is only
    displayed in the Dreamcast's file manager. This function also returns whether
    the custom color is currently enabled.

    \param  dev             The device to change the color of.
    \param  red             The red component. 0-255
    \param  green           The green component. 0-255
    \param  blue            The blue component. 0-255
    \param  alpha           The alpha component. 0-255; 100-255 Recommended

    \retval 1               On success: custom color is enabled
    \retval 0               On success: custom color is disabled
    \retval -1              On failure

    \sa vmu_set_custom_color, vmu_use_custom_color
*/
int vmu_get_custom_color(maple_device_t *dev, uint8_t *red, uint8_t *green, uint8_t *blue, uint8_t *alpha);

/** \brief  Set icon shape of a VMU

    This function sets the icon shape of a specific VMU. The icon shape is a
    VMU icon that is displayed on the LCD screen while navigating the Dreamcast
    BIOS menu and is the GUI representation of the VMU in the menu's file manager.
    The Dreamcast BIOS provides a set of 124 icons to choose from.

    \note
    When a custom file named "ICONDATA_VMS" is present on a VMU, it overrides this
    icon by providing custom icons for both the DC BIOS menu and the VMU's LCD screen.

    \param  dev             The device to change the icon shape of.
    \param  icon_shape      One of the values found in \ref{vmu_icons}.

    \retval 0               On success
    \retval -1              On failure

    \sa vmu_icons, vmu_get_icon_shape
*/
int vmu_set_icon_shape(maple_device_t *dev, uint8_t icon_shape);

/** \brief  Get icon shape of a VMU

    This function gets the icon shape of a specific VMU. The icon shape is a
    VMU icon that is displayed on the LCD screen while navigating the Dreamcast
    BIOS menu and is the GUI representation of the VMU in the menu's file manager.
    The Dreamcast BIOS provides a set of 124 icons to choose from.

    \note
    When a custom file named "ICONDATA_VMS" is present on a VMU, it overrides this
    icon by providing custom icons for both the DC BIOS menu and the VMU's LCD screen.

    \param  dev             The device to change the icon shape of.
    \param  icon_shape      One of the values found in \ref{vmu_icons}.

    \retval 0               On success
    \retval -1              On failure

    \sa vmu_icons, vmu_set_icon_shape
*/
int vmu_get_icon_shape(maple_device_t *dev, uint8_t *icon_shape);

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

    \sa vmu_draw_lcd, vmu_set_icon
*/
int vmu_draw_lcd_xbm(maple_device_t *dev, const char *vmu_icon);

/** \brief  Display a Xwindows XBM on all VMUs.

    This function takes in a Xwindows XBM and displays the image on all VMUs.

    \note
    This is a convenience function for vmu_draw_lcd() to broadcast across all VMUs.

    \param  vmu_icon        The icon to set.

    \sa vmu_draw_lcd_xbm
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

/** \brief  Make a VMU beep (low-level).

    This function sends a raw beep to a VMU, causing the speaker to emit a tone
    noise. See http://dcemulation.org/phpBB/viewtopic.php?f=29&t=97048 for the
    original information about beeping.

    \warning
    This function is submitting raw, encoded values to the VMU. For a more
    user-friendly API built around generating simple tones, see vmu_beep().

    \param  dev             The device to attempt to beep.
    \param  beep            The tone to generate. Byte values are as follows:
                                1. period of square wave 1
                                2. duty cycle of square wave 1
                                3. period of square wave 2(ignored by
                                   standard mono VMUs)
                                4. duty cycle of square wave 2 (ignored by
                                   standard mono VMUs) 

    \retval MAPLE_EOK       On success.
    \retval MAPLE_EAGAIN    If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.

    \sa vmu_beep
*/
int vmu_beep_raw(maple_device_t* dev, uint32_t beep);

/** \brief  Play VMU Buzzer tone.

    Sends two different square waves to generate tone(s) on the VMU. Each
    waveform is configured as shown by the following diagram. On a standard
    VMU, there is only one piezoelectric buzzer, so waveform 2 is ignored; 
    however, the parameters do support dual-channel stereo in case such a 
    VMU ever does come along. 

               Period
        +--------------------+
        |                    |
                   __________            __________
                  |          |          |          |
                  |          |          |          |
        __________|          |__________|          |

                  |          |
                  +----------+        
                   Duty Cycle

                              WAVEFORM

    To stop an active tone, one can simply generate a flat wave, such as by 
    submitting both values as 0s.

    \warning
    Any submitted waveform which has a duty cycle of greater than or equal to 
    its period will result in an invalid waveform being generated and is 
    going to mute or end the tone.

    \note
    Note that there are no units given for the waveform, so any 3rd party VMU 
    is free to use any base clock rate, potentially resulting in different 
    frequencies (or tones) being generated for the same parameters on different 
    devices.

    \note
    On the VMU-side, this tone is generated using the VMU's Timer1 peripheral
    as a pulse generator, which is then fed into its piezoelectric buzzer. The 
    calculated range of the standard VMU, given its 6MHz CF clock running with a 
    divisor of 6 is driving the Timer1 counter, is approximately 3.9KHz-500Khz;
    however, due to physical characteristics of the buzzer, not every frequency
    can be produced at a decent volume, so it's recommended that you test your
    values, using the KOS example found at `/examples/dreamcast/vmu/beep`.

    \param  dev                 The VMU device to play the tone on
    \param  period1             The period or total interval of the first waveform
    \param  duty_cycle1         The duty cycle or active interval of the first waveform 
    \param  period2             The period or total interval of the second waveform
                                (ignored by standard first-party VMUs).
    \param  duty_cycle2         The duty cycle or active interval of the second waveform
                                (ignored by standard first-party VMUs).

    \retval MAPLE_EOK           On success.
    \retval MAPLE_EAGAIN        If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT      If the command timed out while blocking.
*/
int vmu_beep_waveform(maple_device_t *dev, uint8_t period1, uint8_t duty_cycle1, uint8_t period2, uint8_t duty_cycle2);

/** \brief  Set the date and time on the VMU.

    This function sets the VMU's date and time values to
    the given standard C Unix timestamp.

    \param  dev             The device to write to.
    \param  time            Seconds since Unix epoch

    \retval MAPLE_EOK       On success.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
    \retval MAPLE_EFAIL     On errors other than timeout.

    \sa vmu_get_datetime
*/
int vmu_set_datetime(maple_device_t *dev, time_t time);

/** \brief  Get the date and time on the VMU.

    This function gets the VMU's date and time values
    as a single standard C Unix timestamp.

    \note
    This is the VMU equivalent of calling `time(NULL)`.

    \param  dev             The device to write to.
    \param  time            Seconds since Unix epoch

    \retval MAPLE_EOK       On success.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
    \retval MAPLE_EFAIL     On errors other than timeout.

    \sa vmu_set_datetime
*/
int vmu_get_datetime(maple_device_t *dev, time_t *time);

/** \defgroup vmu_buttons VMU Buttons
    \brief    VMU button masks
    \ingroup  vmu

    VMU's button state/cond masks, same as capability masks

    @{
*/
#define VMU_DPAD_UP    (1<<0)   /**< \brief Up Dpad button on the VMU */
#define VMU_DPAD_DOWN  (1<<1)   /**< \brief Down Dpad button on the VMU */
#define VMU_DPAD_LEFT  (2<<1)   /**< \brief Left Dpad button on the VMU */
#define VMU_DPAD_RIGHT (3<<1)   /**< \brief Right Dpad button on the VMU */
#define VMU_A          (4<<1)   /**< \brief 'A' button on the VMU */
#define VMU_B          (5<<1)   /**< \brief 'B' button on the VMU */
#define VMU_MODE       (6<<1)   /**< \brief Mode button on the VMU */
#define VMU_SLEEP      (7<<1)   /**< \brief Sleep button on the VMU */
/** @} */

/** \brief VMU's raw condition data: 0 = PRESSED, 1 = RELEASED */
typedef uint8_t vmu_cond_t;
/** \brief  VMU's "civilized" state data: 0 = RELEASED, 1 = PRESSED */
typedef vmu_cond_t vmu_state_t;

/** \brief  Enable/Disable polling for VMU input

    This function is used to either enable or disable polling the
    VMU buttons' states for input each frame.

    \note
    These buttons are not usually accessible to the player; however,
    several devices, such as the ASCII pad, the arcade pad, and
    the Retro Fighters controller leave the VMU partially exposed,
    so that these buttons remain accessible, allowing them to be used
    as extended controller inputs.

    \note
    Polling for VMU input is disabled by default.

    \sa vmu_get_buttons_enabled
*/
void vmu_set_buttons_enabled(maple_device_t *dev, int enable);

/** \brief  Check whether polling for VMU input has been enabled

    This function is used to check whether per-frame polling of
    the VMU's button states has been enabled in the driver.

    \note
    Polling for VMU input is disabled by default.

    \sa vmu_set_buttons_enabled
*/
int vmu_get_buttons_enabled(void);

/** @} */

/* \cond */
/* Init / Shutdown -- Managed internally by KOS */
int vmu_init(void);
void vmu_shutdown(void);
/* \endcond */

__END_DECLS

#endif  /* __DC_MAPLE_VMU_H */

