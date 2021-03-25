#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <curl/curl.h>

#include "parameters.h"
#include "dirhelper.h"
#include "curlhelper.h"
#include "jsonhelper.h"

int create_dir(char* dir)
{

    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        mkdir(dir, 0700);
    }

}

char* create_user_dir(char* dirtype)
{

    char* home_dir_str = gethome();
    char* config_dir_str = ( char* ) malloc(1 + strlen(home_dir_str) + strlen(dirtype) + strlen("searxcli/"));
    strcpy(config_dir_str, home_dir_str);
    strcat(config_dir_str, dirtype);
    strcat(config_dir_str, "searxcli");

    create_dir(config_dir_str);
    free(config_dir_str);
}

int main(int argc, char** argv)
{


    struct PARAMETERS* p = malloc(sizeof(struct PARAMETERS));

    create_user_dir("/.config/");

    char* home_dir_str = gethome();
    char* config_file_str = ( char* ) malloc(1 + strlen(home_dir_str) + strlen("/.cache/") + strlen("searxcli/searxcli.config"));
    strcpy(config_file_str, home_dir_str);
    strcat(config_file_str, "/.config/");
    strcat(config_file_str, "searxcli/searxcli.config");

    parameter_parse_error ppe = getParameters(argc, argv, p, config_file_str);
    //printf("error: %s", errordesc[ppe].message);
    if (ppe == E_SUCCESS_AND_EXIT)
    {
        goto cleanup_final;
    }
    free(config_file_str);

    create_user_dir("/.cache/");
    char* searchquerymd5 = str2md5(p->query_string, strlen(p->query_string));

    char* cache_dir_str = ( char* ) malloc(1 + strlen(home_dir_str) + strlen("/.cache/") + strlen("searxcli/"));
    strcpy(cache_dir_str, home_dir_str);
    strcat(cache_dir_str, "/.cache/");
    strcat(cache_dir_str, "searxcli/");
    bool directory_exists = does_directory_exist(cache_dir_str, searchquerymd5);

    char* search_dir_str = ( char* ) malloc(1 + strlen(home_dir_str) + strlen("/.cache/") + strlen("searxcli/") + strlen(searchquerymd5));
    strcpy(search_dir_str, home_dir_str);
    strcat(search_dir_str, "/.cache/");
    strcat(search_dir_str, "searxcli/");
    strcat(search_dir_str, searchquerymd5);

    if (directory_exists == true)
    {
        if (p->use_cache == true)
        {
            goto cleanup_partial;
        }
        else
        {
            delete_dir(search_dir_str);
        }
    }

    restrict_folders_to_cache(cache_dir_str, p->max_cache);
    create_dir(search_dir_str);

    char* page_filename = ( char* ) malloc(1 + strlen(search_dir_str) + strlen("/results.json"));
    strcpy(page_filename, search_dir_str);
    strcat(page_filename, "/results.json");

    char* str1 = "q=";
    char* str2 = "&category_images=1&language=en-US&format=json";
    char* output = curl_easy_escape(output,  p->query_string, 15);
    char* post_fields_string = ( char* ) malloc(1 + strlen(str1) + strlen(str2) + strlen(output));

    strcpy(post_fields_string, str1);
    strcat(post_fields_string, output);
    strcat(post_fields_string, str2);
    curl_free(output);


    http_return_file_to_disk(p->server_url, post_fields_string, page_filename);


    get_images_from_json(search_dir_str, page_filename, p->max_results);

cleanup_full:
    free(page_filename);
    free(post_fields_string);
cleanup_partial:
    printf("%s", search_dir_str);
    free(search_dir_str);
    free(searchquerymd5);
    free(cache_dir_str);
cleanup_final:
    free((char*)p->server_url);
    free(p);
    exit(0);
}


