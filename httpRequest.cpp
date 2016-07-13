#include "httpRequest.h"

using namespace std;

#define CURLLogFILENAME "curlposttest.log"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
int uploadProgress(string file,string url);

int getUrl(const char *url)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent-007");
	curl = curl_easy_init();    // 初始化
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
		curl_easy_setopt(curl, CURLOPT_URL, url);

		res = curl_easy_perform(curl);   // 执行
		//if(res != 0)
		//{
		//printf("here\n");
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		//return true;
		// }
	}
	return 1;
}
int postUrl(const char *url, const char *paraments)
{
	CURL *curl;
	CURLcode res;
	//FILE *fp;
	//if ((fp = fopen(filename, "w")) == NULL)
		//return false;
	curl = curl_easy_init();
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paraments);    // 指定post内容
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
		curl_easy_setopt(curl, CURLOPT_URL, url);   // 指定url
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	//fclose(fp);
	return 1;
}

int uploadImage(string path , int mount , string time)
{
	DIR * dir;
	struct dirent * ptr;
	char file_list[100][40];
	int i=0;
	dir = opendir(path.c_str()); 
	while((ptr = readdir(dir)) != NULL) 
	{
		//printf("d_name : %s\n", ptr->d_name); 
		strcpy(file_list[i],ptr->d_name );
		if ( ++i>=100 ) break;
	}
	closedir(dir);
	
	int count = 0;
	for(int j=0;j<i;j++)
	{
		string url = "http://192.168.1.133:8080/SmartHome/UploadImage?time=\"" + time + "\"";
		if(count == 0)
		{
			url += "&flag=0";
		}
		else if(j == i-1 || count == mount)
		{
			url += "&flag=2";
		}
		else 
		{
			url += "&flag=1";
		}
		//cout<<url<<endl;
		if(file_list[j][0] == 'v')
		{
			string filename(file_list[j]);
			string file = path + "/" + filename;
			uploadProgress(file,url);
			count++;
			sleep(1);
		}
	}
	return 1;
	
}

int uploadProgress(string file , string url)
{
    //cout<<file<<endl;
    CURL *curl;
    CURLcode res;
    FILE *fptr;
    struct curl_slist *http_header = NULL;
    
    if ((fptr = fopen(CURLLogFILENAME, "w")) == NULL) {
        fprintf(stderr, "fopen file error: %s\n", CURLLogFILENAME);
        exit(1);
    }
    
    curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
    
    struct curl_httppost *formpost = 0;
    struct curl_httppost *lastptr  = 0;
    curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "reqformat", CURLFORM_PTRCONTENTS, "plain", CURLFORM_END);
    curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "file", CURLFORM_FILE, file.c_str(), CURLFORM_END);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    FILE *fptr = (FILE*)userp;
    fwrite(buffer, size, nmemb, fptr);
}  

/*int main(void)
{
	string head = "http://10.10.104.116/cgi-bin/PTZCamera.cgi?11&";
	string point;
	string back = "&admin&";
	cin >> point;
	string url = head + point + back;
	const char *u = url.data();
	getUrl(u);
	//getUrl("http://10.10.104.116/cgi-bin/PTZCamera.cgi?11&2&admin&");

	//sleep(20);
	// getUrl("http://10.10.104.116/cgi-bin/PTZControl.cgi?5&admin&");
	//postUrl("/tmp/post.html");
}*/
