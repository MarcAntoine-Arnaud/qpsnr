
#ifndef _QPSNR_QPSNR_H_
#define _QPSNR_QPSNR_H_

#include "qav.h"
#include "stats.h"

#include <vector>
#include <map>
#include <fstream>

class Qpsnr
{

public:
	Qpsnr(const std::string& outputFilename, const std::string& reference );
	Qpsnr(const std::string& outputFilename, const std::string& reference, const size_t width, const size_t height );
	~Qpsnr();

	void addVideo( const std::string& video );

	void initAnalyser( const std::string& analyser, const std::map<std::string, std::string>& options );

	bool processFrame( int& processedFrameIndex );
	void process();

private:
	std::ofstream _outputFile;
	
	qav::qvideo	_referenceVideo;
	std::vector<qav::qvideo*> _videos;

	stats::s_base* _analyzer;
};

#endif