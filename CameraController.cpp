
#include "httpRequest.h"
#include "vlc/vlc.h" 
#include <stdio.h>
#include "vlc/libvlc_media_player.h"
#include "SmartHome.h"
#include <unistd.h>

void useCameraPreSet(string num)
{
	string head = "http://192.168.1.132/cgi-bin/PTZCamera.cgi?11&";
	string back = "&admin&";
	string url = head + num + back;
	const char *u = url.data();
	getUrl(u);
}

void takeCameraSnap(int mount, int interval , const char* filePath)
{
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	libvlc_time_t length;
	int width;
	int height;
	int wait_time = 3;
	inst = libvlc_new(0, NULL);
	m = libvlc_media_new_location(inst, "rtsp://192.168.1.132/mpeg4");
	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_media_player_play(mp);  
	sleep(wait_time);
	for (int i = 0; i < mount; i++)
	{
		libvlc_video_take_snapshot(mp, 0, filePath, 1289, 720);
		usleep(interval*1000);
	}
	// Stop playing  
       libvlc_media_player_stop (mp);  
   
     // Free the media_player  
       libvlc_media_player_release (mp);
	return;  
}
