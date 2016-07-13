#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <curl/curl.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
int getUrl(const char *url);
int postUrl(const char *url, const char *paraments);
int uploadImage(std::string path , int mount , std::string time);
