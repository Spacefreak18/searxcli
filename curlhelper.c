#include "curlhelper.h"

#include <curl/curl.h>
#include <string.h>


static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}

int http_return_file_to_disk(const char* post_url, char* post_fields_string, char* page_filename)
{

    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, post_url);


        if (post_fields_string != NULL)
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields_string);
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        FILE* pagefile;
        pagefile = fopen(page_filename, "wb");

        if (pagefile)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);

            res = curl_easy_perform(curl);
            fclose(pagefile);

            if(res != CURLE_OK)
            {
                //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
        }

        curl_easy_cleanup(curl);


    }
    curl_global_cleanup();
    return 0;
}
