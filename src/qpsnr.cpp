
#include "qpsnr.hpp"
#include "stats.h"
#include "settings.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

Qpsnr::Qpsnr(const std::string& reference, const size_t width, const size_t height)
	: _referenceVideo( reference.c_str(), width, height )
	, _analyzer ( NULL )
	, _width( width )
	, _height( height )
{
}

Qpsnr::~Qpsnr()
{
	if( _analyzer != NULL )
	{
		delete _analyzer;
	}
	
	for( size_t i = 0; i < _videos.size(); ++i )
	{
		delete _videos.at(i);
	}
}

void Qpsnr::addVideo( const std::string& video )
{
	const qav::scr_size	referenceSize = _referenceVideo.get_size();
	qav::qvideo* v = new qav::qvideo(video.c_str(), referenceSize.x, referenceSize.y);
	_videos.push_back( v );

	if( _referenceVideo.get_fps_k() != _videos.back()->get_fps_k() )
	{
		throw std::runtime_error("different fps.");
	}
}

void Qpsnr::initAnalyser( const std::string& analyser, const std::map<std::string, std::string>& options )
{
	const qav::scr_size	referenceSize = _referenceVideo.get_size();

	_analyzer = stats::get_analyzer(
		analyser.c_str(),
		_videos.size(),
		referenceSize.x,
		referenceSize.y,
		std::cout );

	// set the default values, in case will get overwritten
	std::ostringstream oss;
	oss << _referenceVideo.get_fps_k() / 1000;
	
	_analyzer->set_parameter("fpa", oss.str());
	_analyzer->set_parameter("blocksize", "8");
	// load the passed parameters
	for(std::map<std::string, std::string>::const_iterator it = options.begin(); it != options.end(); ++it) {
		LOG_INFO << "Analyzer parameter: " << it->first << " = " << it->second << std::endl;
		_analyzer->set_parameter(it->first.c_str(), it->second.c_str());
	}
}

void Qpsnr::processFrame()
{
	bool skip = false;
	int frameNumber = 0;
	std::vector<unsigned char> referenceFrameBuffer;
	std::vector<std::vector<unsigned char> > videoFrameBuffer(_videos.size());

	_referenceVideo.get_frame(referenceFrameBuffer, &frameNumber, skip);

	for( size_t streamIndex = 0; streamIndex < _videos.size(); ++streamIndex)
	{
		_videos.at(streamIndex)->get_frame(videoFrameBuffer.at(streamIndex), &frameNumber, skip);
	}
	std::vector<bool> v_ok(_videos.size());

	_analyzer->process(
			frameNumber,
			referenceFrameBuffer,
			v_ok,
			videoFrameBuffer
	);
}

void Qpsnr::process()
{
	while(1)
	{
		processFrame();
	}
}
