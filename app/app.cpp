
#include "qpsnr.hpp"
#include "settings.h"

#include <iostream>
#include <stdexcept>
#include <getopt.h>

const std::string __qpsnr__ = "qpsnr";
const std::string __version__ = "0.2.5";

void print_help(void) {
	std::cerr << __qpsnr__ << " v" << __version__ << " - (C) 2010, 2011, 2012 E. Oriani - 2013 E. Oriani, Paul Caron" << std::endl;
	std::cerr << "Usage: " << __qpsnr__ << " [options] -r ref.video compare.video1 compare.video2 ..."  << std::endl;
	std::cerr << std::endl;
	std::cerr << "-r,--reference:" << std::endl;
	std::cerr << "\tset reference video (mandatory)" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-v,--video-size:" << std::endl;
	std::cerr << "\tset analysis video size WIDTHxHEIGHT (ie. 1280x720), default is reference video size" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-s,--skip-frames:" << std::endl;
	std::cerr << "\tskip n initial frames" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-m,--max-frames:" << std::endl;
	std::cerr << "\tset max frames to process before quit" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-I,--save-frames:" << std::endl;
	std::cerr << "\tsave frames (ppm format)" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-G,--ignore-fps:" << std::endl;
	std::cerr << "\tanalyze videos even if the expected fps are different" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-a,--analyzer:" << std::endl;
	std::cerr << "\tpsnr : execute the psnr for each frame" << std::endl;
	std::cerr << "\tavg_psnr : take the average of the psnr every n frames (use option \"fpa\" to set it)" << std::endl;
	std::cerr << "\tssim : execute the ssim (Y colorspace) on the frames divided in blocks (use option \"blocksize\" to set the size)" << std::endl;
	std::cerr << "\tavg_ssim : take the average of the ssim (Y colorspace) every n frames (use option \"fpa\" to set it)" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-o,--aopts: (specify option1=value1:option2=value2:...)" << std::endl;
	std::cerr << "\tfpa : set the frames per average, default 25" << std::endl;
	std::cerr << "\tcolorspace : set the colorspace (\"rgb\", \"hsi\", \"ycbcr\" or \"y\"), default \"rgb\"" << std::endl;
	std::cerr << "\tblocksize : set blocksize for ssim analysis, default 8" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-l,--log-level:" << std::endl;
	std::cerr << "\t0 : No log" << std::endl;
	std::cerr << "\t1 : Errors only" << std::endl;
	std::cerr << "\t2 : Warnings and errors" << std::endl;
	std::cerr << "\t3 : Info, warnings and errors (default)" << std::endl;
	std::cerr << "\t4 : Debug, info, warnings and errors" << std::endl;
	std::cerr << std::endl;
	std::cerr << "-h,--help:" << std::endl;
	std::cerr << "\tprint this help and exit" << std::endl;
}

int parse_options(int argc, char *argv[], std::map<std::string, std::string>& aopt) {
	aopt.clear();
	int c = 0,
	option_index = 0;

	static struct option long_options[] =
	{
		{"analyzer", required_argument, 0, 'a'},
		{"max-frames", required_argument, 0, 'm'},
		{"skip-frames", required_argument, 0, 's'},
		{"reference", required_argument, 0, 'r'},
		{"log-level", required_argument, 0, 'l'},
		{"save-frames", no_argument, 0, 'I'},
		{"video-size", required_argument, 0, 'v'},
		{"ignore-fps", no_argument, 0, 'G'},
		{"help", no_argument, 0, 'h'},
		{"aopts", required_argument, 0, 'o'},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long (argc, argv, "a:l:m:o:r:s:v:hIG", long_options, &option_index)) != -1) {
		switch (c) {
			case 'a':
			{
				settings::ANALYZER = optarg;
				break;
			}
			case 'v':
			{
				const char *p_x = strchr(optarg, 'x');
				if (!p_x)
					p_x = strchr(optarg, 'X');
				if (!p_x)
					throw std::runtime_error("Invalid video size specified (use WIDTHxHEIGHT format, ie. 1280x720)");
				const std::string s_x(optarg, p_x-optarg);
				const std::string s_y(p_x+1);
				if (s_x.empty() || s_y.empty())
					throw std::runtime_error("Invalid video size specified (use WIDTHxHEIGHT format, ie. 1280x720)");
				
				const int i_x = atoi(s_x.c_str());
				const int i_y = atoi(s_y.c_str());
				if (i_x <=0 || i_y <=0)
					throw std::runtime_error("Invalid video size specified, negative or 0 width/height");

				settings::VIDEO_SIZE_W = i_x;
				settings::VIDEO_SIZE_H = i_y;
				
				break;
			}
			case 's':
				{
					const int skip_frames = atoi(optarg);
					if (skip_frames > 0 ) settings::SKIP_FRAMES = skip_frames;
				}
				break;
			case 'r':
			{
				settings::REF_VIDEO = optarg;
				break;
			}
			case 'm':
			{
				const int max_frames = atoi(optarg);
				if (max_frames > 0 )
					settings::MAX_FRAMES = max_frames;
				break;
			}
			case 'l':
				if (isdigit(optarg[0])) {
					char log_level[2];
					log_level[0] = optarg[0];
					log_level[1] = '\0';
					const int log_ilev = atoi(log_level);
					switch (log_ilev) {
						case 0:
							settings::LOG = 0x00;
							break;
						case 1:
							settings::LOG = 0x01;
							break;
						case 2:
							settings::LOG = 0x03;
							break;
						case 3:
							settings::LOG = 0x07;
							break;
						case 4:
							settings::LOG = 0x0F;
							break;
						default:
							break;
					}
				}
				break;
			case 'h':
				print_help();
				exit(0);
				break;
			case 'I':
				settings::SAVE_IMAGES = true;
				break;
			case 'G':
				settings::IGNORE_FPS = true;
				break;
			case 'o':
				{
					const char 	*p_opts = optarg,
							*p_colon = 0;
					while((p_colon = strchr(p_opts, ':'))) {
						const std::string	c_opt(p_opts, (p_colon-p_opts));
						const size_t 		p_equal = c_opt.find('=');
						if (std::string::npos != p_equal)
							aopt[c_opt.substr(0, p_equal)] = c_opt.substr(p_equal+1);
						p_opts = p_colon+1;
					}
					const std::string	c_opt(p_opts);
					const size_t 		p_equal = c_opt.find('=');
					if (std::string::npos != p_equal)
						aopt[c_opt.substr(0, p_equal)] = c_opt.substr(p_equal+1);
				}
				break;
			case '?':
				if (strchr("almorsv", optopt)) {
					std::cerr << "Option -" << (char)optopt << " requires an argument" << std::endl;
					print_help();
					exit(1);
				} else if (isprint (optopt)) {
					std::cerr << "Option -" << (char)optopt << " is unknown" << std::endl;
					print_help();
					exit(1);
				}
				break;
			default:
				std::cerr << "Invalid option: " << c << std::endl;
				print_help();
				exit(1);
				break;
		}
	}
	// fix here the frame limit
	if (settings::SKIP_FRAMES > 0 && settings::MAX_FRAMES>0) settings::MAX_FRAMES += settings::SKIP_FRAMES;
	return optind;
}


int main(int argc, char *argv[])
{
	std::map<std::string, std::string>	aoptions;
	const int param = parse_options(argc, argv, aoptions);

	if (settings::REF_VIDEO == "")
	{
		std::cerr << "[ERROR] Reference video not specified" << std::endl;
		std::cerr << std::endl;
		print_help();
	}
		
	Qpsnr comparator( "result.xml", settings::REF_VIDEO.c_str(), settings::VIDEO_SIZE_W, settings::VIDEO_SIZE_H );

	for(int i = param; i < argc; ++i)
	{
		comparator.addVideo( argv[i] );
	}

	// print some infos
	//LOG_INFO << "Skip frames: " << ((settings::SKIP_FRAMES > 0) ? settings::SKIP_FRAMES : 0) << std::endl;
	//LOG_INFO << "Max frames: " << ((settings::MAX_FRAMES > 0) ? settings::MAX_FRAMES : 0) << std::endl;
	// create the stats analyzer (like the psnr)
	LOG_INFO << "Analyzer set: " << settings::ANALYZER << std::endl;
	comparator.initAnalyser( settings::ANALYZER.c_str(), aoptions );
		
	comparator.process();
}
