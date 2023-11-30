#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CMD_SYSTEM_H
#define CMD_SYSTEM_H

   extern const char *GLOBAL_HASH;
#endif

   // Register system functions
   void register_system(void);
   void determineChipType(char chip_type[30]);
   const char *get_project_version();
   const char *get_project_build_date();

#ifdef __cplusplus
}
#endif
