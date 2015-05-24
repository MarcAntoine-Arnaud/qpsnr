
#include "qpsnr.hpp"
#include "stats.h"
#include "settings.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

Qpsnr::Qpsnr(const std::string& outputFilename, const std::string& reference)
	: _outputFile( outputFilename.c_str(), std::ofstream::out )
	, _referenceVideo( reference.c_str() )
	, _analyzer ( NULL )
{
}

Qpsnr::Qpsnr(const std::string& outputFilename, const std::string& reference, const size_t width, const size_t height)
	: _outputFile( outputFilename.c_str(), std::ofstream::out )
	, _referenceVideo( reference.c_str(), width, height )
	, _analyzer ( NULL )
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
		_outputFile );

	// set the default values, in case will get overwritten
	std::ostringstream oss;
	oss << _referenceVideo.get_fps_k() / 1000;
	
	_analyzer->set_parameter("fpa", oss.str());
	_analyzer->set_parameter("blocksize", "8");
	// load the passed parameters
	for(std::map<std::string, std::string>::const_iterator it = options.begin(); it != options.end(); ++it)
	{
		LOG_ERROR << "Analyzer parameter: " << it->first << " = " << it->second << std::endl;
		_analyzer->set_parameter(it->first.c_str(), it->second.c_str());
	}
}

bool Qpsnr::processFrame( int& processedFrameIndex )
{
	bool skip = false;
	std::vector<unsigned char> referenceFrameBuffer;
	std::vector<std::vector<unsigned char> > videoFrameBuffer(_videos.size());

	bool readedFrame = _referenceVideo.get_frame(referenceFrameBuffer, &processedFrameIndex, skip);
	if( ! readedFrame ){
		return false;
	}
	for( size_t streamIndex = 0; streamIndex < _videos.size(); ++streamIndex)
	{
		bool readedFrame = _videos.at(streamIndex)->get_frame(videoFrameBuffer.at(streamIndex), &processedFrameIndex, skip);
		if( ! readedFrame ){
			return false;
		}
	}
	
	if(_videos.size() == 0){
		return false;
	}

	std::vector<bool> v_ok(_videos.size());

	_analyzer->process(
			processedFrameIndex,
			referenceFrameBuffer,
			v_ok,
			videoFrameBuffer
	);
	return true;
}

void Qpsnr::process()
{
	while(1)
	{
		int processedFrameIndex = 0;
		if( ! processFrame( processedFrameIndex ) )
		{
			break;
		}

		std::cerr << "\r Process frame " << processedFrameIndex << std::flush;
		
		if( processedFrameIndex == settings::MAX_FRAMES )
		{
			break;
		}
	}
}
