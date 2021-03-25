#include "jsonhelper.h"
#include "curlhelper.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <json-c/json.h>

int get_images_from_json(char* cachedir, char* jsonfile, int maxresults)
{
    int fd = open(jsonfile, O_RDONLY, 0);
    
    json_object* new_obj  = json_object_from_fd_ex(fd, 20);

    close(fd);
    
    json_object_object_foreach(new_obj, key, val)
    {
        
        //orig_count++;
        if (strcmp(key, "results") == 0)
        {

            int ii = 0;
            int looplimit = 0;
            looplimit = json_object_array_length(val);
            if (maxresults < looplimit)
            {
                looplimit = maxresults;
            }
            for (ii = 0; ii < looplimit; ii++)
            {
                json_object* obj = json_object_array_get_idx(val, ii);

                const char* img_src = NULL;
                const char* title = NULL;
                json_object_object_foreach(obj, key2, val2)
                {
                    
                    if (strcmp(key2, "img_src") == 0)
                    {
                        img_src = ( char* ) json_object_get_string(val2);
                    }
                    if (strcmp(key2, "title") == 0)
                    {
                        title = ( char* ) json_object_get_string(val2);
                    }
                }

                if (img_src != NULL && title != NULL)
                {
                    char* img_name = ( char* ) malloc(1 + strlen(cachedir) + strlen("/") + strlen(title) + strlen(".jpg"));
                    strcpy(img_name, cachedir);
                    strcat(img_name, "/");
                    strcat(img_name, title);
                    strcat(img_name, ".jpg");

                    http_return_file_to_disk(img_src, NULL, img_name);

                    free(img_name);
                }

            }
        }
    }

    if (new_obj)
    {
      json_object_put(new_obj);
    }
    return 0;
}
