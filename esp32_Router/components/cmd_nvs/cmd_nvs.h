#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

   // Register NVS functions
   void register_nvs(void);
   int erase_ns(int argc, char **argv);

#ifdef __cplusplus
}
#endif
