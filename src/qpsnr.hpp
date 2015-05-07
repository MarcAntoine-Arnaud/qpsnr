
#ifndef _QPSNR_QPSNR_H_
#define _QPSNR_QPSNR_H_

#include "qav.h"
#include "stats.h"

#include <vector>
#include <map>

class Qpsnr
{

public:
	Qpsnr(const std::string& reference, const size_t width, const size_t height);
	~Qpsnr();

	void addVideo( const std::string& video );

	void initAnalyser( const std::string& analyser, const std::map<std::string, std::string>& options );

	void processFrame();
	void process();

private:
	qav::qvideo	_referenceVideo;
	std::vector<qav::qvideo*> _videos;

	stats::s_base* _analyzer;

	const size_t _width;
	const size_t _height;
};

#endif