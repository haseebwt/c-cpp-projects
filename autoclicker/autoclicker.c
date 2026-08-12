/* 
Project by: Haseeb Khan (haseebwt) aka Beeb
GNU Formatting convention
Only works on Linux as of right now and requires sudo to execute without errors
*/

#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <linux/input-event-codes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
        // Create a fake device first
        struct libevdev *dev = libevdev_new ();
        libevdev_set_name (dev, "Haseeb's Mouse");

        // movement
        libevdev_enable_event_type (dev, EV_REL);
        libevdev_enable_event_code (dev, EV_REL, REL_X, NULL);
        libevdev_enable_event_code (dev, EV_REL, REL_Y, NULL);

        // buttons
        libevdev_enable_event_type (dev, EV_KEY);
        libevdev_enable_event_code (dev, EV_KEY, BTN_LEFT, NULL);
        libevdev_enable_event_code (dev, EV_KEY, BTN_MIDDLE, NULL);
        libevdev_enable_event_code (dev, EV_KEY, BTN_RIGHT, NULL);

        // syn
        libevdev_enable_event_type (dev, EV_SYN);
        libevdev_enable_event_code (dev, EV_SYN, SYN_REPORT, NULL);

        struct libevdev_uinput *uidev; // input device

        int err = libevdev_uinput_create_from_device (
            dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev); // using fake device

        if (err != 0)
                {
                        perror ("Error");
                        exit (-1);
                }

        printf ("Starting in 3...\n"); // Frankly, idk if the usleep() below is
                                       // 3 seconds or miliseconds
        // Wait for eveyrthing to init
        usleep (3 * 1000000);
        printf ("Started!\n");
        while (1)
                {
                        // Click
                        libevdev_uinput_write_event (uidev, EV_KEY, BTN_LEFT,
                                                     1);
                        libevdev_uinput_write_event (uidev, EV_SYN, SYN_REPORT,
                                                     0);
                        usleep (500);

                        // Release
                        libevdev_uinput_write_event (uidev, EV_KEY, BTN_LEFT,
                                                     0);
                        libevdev_uinput_write_event (uidev, EV_SYN, SYN_REPORT,
                                                     0);
                        // Wait...
                        usleep (10000);
                }

        return EXIT_SUCCESS;
}
