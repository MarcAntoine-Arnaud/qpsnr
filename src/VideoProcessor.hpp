
#ifndef _QPSNR_VIDEO_PROCESSOR_HPP_
#define _QPSNR_VIDEO_PROCESSOR_HPP_

#include "mt.h"
#include "qav.h"

class VideoProcessor : public mt::Thread {
public:
	VideoProcessor(
		int& frame,
		std::vector<unsigned char>& buf,
		qav::qvideo& video,
		bool& __exit,
		bool& __skip)
	: _frame(frame)
	, _buf(buf)
	, _video(video)
	, _exit(__exit)
	, _skip(__skip)
	{
	}

	virtual void run(void)
	{
		while(!_exit)
		{
			if (!_video.get_frame(_buf, &_frame, _skip))
				_frame = -1;
		}
	}
private:
	int& _frame;
	std::vector<unsigned char>& _buf;
	qav::qvideo& _video;
	bool& _exit;
	bool& _skip;
};