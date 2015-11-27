#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include <3ds.h>

PrintConsole topScreen, bottomScreen;

Result http_download(httpcContext *context)//This error handling needs updated with proper text printing once ctrulib itself supports that.
{
	Result ret=0;
	//u8* framebuf_top;
	u32 statuscode=0;
	//u32 size=0;
	u32 contentsize=0;
	u8 *buf;

	ret = httpcBeginRequest(context);
	if(ret!=0)return ret;

	ret = httpcGetResponseStatusCode(context, &statuscode, 0);
	if(ret!=0)return ret;

	//printf("http status code: %i\n", statuscode);

	if(statuscode!=200){
		printf("status code not 200, it was %i", statuscode);
		gfxFlushBuffers();
		return -2;
	}

	ret=httpcGetDownloadSizeState(context, NULL, &contentsize);
	if(ret!=0)return ret;
	unsigned char *buffer = (unsigned char*)malloc(contentsize+1);

	consoleSelect(&topScreen);

	printf("HTTP status code: %i\n", statuscode);
	printf("%i bytes\n", contentsize);
	gfxFlushBuffers();

	buf = (u8*)malloc(contentsize);
	if(buf==NULL)return -1;
	memset(buf, 0, contentsize);


	ret = httpcDownloadData(context, buffer, contentsize, NULL);
	if(ret!=0)
	{
		free(buf);
		return ret;
	}

	/*size = contentsize;
	if(size>(240*400*3*2))size = 240*400*3*2;

	framebuf_top = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memcpy(framebuf_top, buf, size);

	gfxFlushBuffers();
	gfxSwapBuffers();

	framebuf_top = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memcpy(framebuf_top, buf, size);

	gfxFlushBuffers();
	gfxSwapBuffers();
	gspWaitForVBlank();*/

	consoleSelect(&bottomScreen);
	printf("%s", buffer);

	free(buf);

	return 0;
}

int main()
{
	Result ret=0;
	httpcContext context;

	gfxInitDefault();

	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleSelect(&topScreen);
	printf("Koopa Web Cruiser alpha\n");
	printf("Press START to exit.\n\n");
	gfxFlushBuffers();

	httpcInit();

	//Change this to your own URL.
	char *url = "http://jsa.paperplane.io/index.html";

	printf("Loading %s\n\n",url);
	gfxFlushBuffers();

	ret = httpcOpenContext(&context, url , 0);
	//printf("return from httpcOpenContext: %"PRId32"\n",ret);
	gfxFlushBuffers();

	if(ret==0)
	{
		ret=http_download(&context);
		//printf("return from http_download: %"PRId32"\n",ret);
		gfxFlushBuffers();
		httpcCloseContext(&context);
	}

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		// Your code goes here

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	// Exit services
	httpcExit();
	gfxExit();
	return 0;
}
