# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://analogdevicesinc.github.io/msdk/USERGUIDE/#build-system

#BOARD=FTHR_RevA
# ^ For example, you can uncomment this line to make the 
# project build for the "FTHR_RevA" board.

# **********************************************************

# Add your config here!

# If you have secure version of MCU, set SBT=1 to generate signed binary
# For more information on how sing process works, see
# https://www.analog.com/en/education/education-library/videos/6313214207112.html
SBT=0

IPATH += resources
VPATH += resources/tft_demo

FONTS = LiberationSans12x12 LiberationSans24x24 LiberationSans28x28 LiberationSans16x16

ifeq ($(BOARD),FTHR)
$(error ERR_NOTSUPPORTED: This project is not supported for the FTHR board)
endif

ifeq ($(BOARD),APARD)
$(error ERR_NOTSUPPORTED: This project is not supported for the AD-APARD32690 board)
endif