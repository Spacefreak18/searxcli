#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include <stdbool.h>

struct PARAMETERS
{
    int   max_results;
    int   max_cache;
    bool  use_cache;
    const char  *server_url;
    const char *query_string;
};


enum _config_error
{
    E_SUCCESS_AND_EXIT = 0,
    E_SUCCESS_AND_DO   = 1,
    E_SOMETHING_BAD    = 2
};

typedef enum _config_error parameter_parse_error;

parameter_parse_error getParameters(int argc, char **argv, struct PARAMETERS *p, char *configfile);

struct _errordesc {
    int  code;
    char *message;
} static errordesc[] = {
    { E_SUCCESS_AND_EXIT, "No error and exiting" },
    { E_SUCCESS_AND_DO,   "No error and continuing" },
    { E_SOMETHING_BAD,    "Something bad happened" },
};

#endif
