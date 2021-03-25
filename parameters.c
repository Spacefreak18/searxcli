#include "parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libconfig.h>

#include <argtable2.h>

parameter_parse_error getParameters(int argc, char** argv, struct PARAMETERS* p, char* configfile)
{

    parameter_parse_error exitcode = E_SOMETHING_BAD;

    // set return structure defaults
    p->max_results         = 0;
    p->max_cache           = 20;
    p->use_cache           = true;
    p->server_url          = "";

    // setup argument handling structures
    const char* progname = "searxcli";
    struct arg_str* arg_search      = arg_strn(NULL, NULL, "<pattern>", 1, argc, NULL);
    struct arg_str* arg_server      = arg_strn("s", "server","example.com",0,argc+1,"Searx server endpoint");
    struct arg_int* arg_max_results = arg_int0("r", "max_results",NULL,"limit search results to n (defaults to unlimited (0))");
    struct arg_int* arg_max_cache   = arg_int0("c", "max_cache",NULL,"limit cached searches to n (enabled by default and set to 20)");
    struct arg_lit* arg_nouse_cache = arg_litn(NULL, "nocache", 0, 1, "do not use cache, will disregard all settings of cache for current search");
    struct arg_lit* help            = arg_litn(NULL,"help", 0, 1, "print this help and exit");
    struct arg_lit* vers            = arg_litn(NULL,"version", 0, 1, "print version information and exit");
    struct arg_end* end             = arg_end(20);
    void* argtable[]                = {arg_search,arg_server,arg_max_results,arg_max_cache,arg_nouse_cache,help,vers,end};
    int nerrors;

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",progname);
        //exitcode=1;
        goto cleanup;
    }

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    // this will always be needed
    p->query_string = arg_search->sval[0];
    // interpret some special cases before we go through trouble of reading the config file
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        printf("Print certain system information.  With no options, same as -s.\n\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        printf("\nReport bugs to <caesium137@rocketmail.com>.\n");
        exitcode = E_SUCCESS_AND_EXIT;
        goto cleanup;
    }

    if (vers->count > 0)
    {
        printf("'%s' console based searx search client.\n",progname);
        printf("March 2021, Paul Dino Jones\n");
        exitcode = E_SUCCESS_AND_EXIT;
        goto cleanup;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,progname);
        printf("Try '%s --help' for more information.\n",progname);
        exitcode = E_SOMETHING_BAD;
        goto cleanup;
    }

    exitcode = E_SUCCESS_AND_DO;
    // config file is read first,
    config_t cfg;
    config_init(&cfg);
    if (!config_read_file(&cfg, configfile))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
    }
    else
    {
        config_lookup_int(&cfg, "max_results", &p->max_results);
        config_lookup_int(&cfg, "max_cache", &p->max_cache);
        const char* str;
        config_lookup_string(&cfg, "server_url", &str);
        p->server_url = strdup(str);
    }

    // if a switch is given, modify value in return structure
    if (arg_max_results->count > 0)
    {
        p->max_results = arg_max_results->ival[0];
    }
    if (arg_max_cache->count > 0)
    {
        p->max_cache = arg_max_cache->ival[0];
    }
    if (arg_server->count > 0)
    {
        p->server_url = arg_server->sval[0];
    }
    if (arg_nouse_cache->count > 0)
    {
        p->use_cache = false;
    }

cleanup1:
    config_destroy(&cfg);

cleanup:
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
    return exitcode;

}
