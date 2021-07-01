This is the source code of a possible GUI for HTPA UDP modules.
It shows how to control and read out the HTPA UDP modules.
The project was written in Embarcaderos c++Builder XE.
Please make a safety copy of your winspool.h and VFW.h and replace them by the ones, which can be found in this archive.
If you have newer versions of these two files, than please search for these two defines and add them in a similar way into your exisiting files.

#define DONT_USE_WINSPOOL_SETPORTA
#define DISABLE_TOHEX_IN_VFWh

Best regards,

Bodo Forg