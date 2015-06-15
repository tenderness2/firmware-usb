#include <sstream>
#include <curl/curl.h>

namespace http_client {

	std::size_t write_to_stream(void *data, 
								std::size_t size, 
								std::size_t nmemb, 
								std::stringstream *stream)
	{
		stream->write(static_cast<char *>(data), size * nmemb);
		return size * nmemb;
	}
	
	void request_url_to_stream(std::string const &url, 
							   std::stringstream *stream)
	{
		CURL *curl = curl_easy_init();
		if(!curl) {
			throw std::runtime_error{"CURL init failed"};
		}

		CLOG(INFO, "http_client") << "requesting " << url;
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_stream);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, stream);
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if(res != CURLE_OK) {
			throw std::runtime_error{curl_easy_strerror(res)};
		}
	}

	std::string request_url_to_string(std::string const &url)
	{
		std::stringstream stream;
		request_url_to_stream(url, &stream);
		
		return stream.str();
	}
}

