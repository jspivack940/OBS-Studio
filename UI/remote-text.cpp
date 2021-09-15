/******************************************************************************
    Copyright (C) 2015 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <util/curl/curl-helper.h>
#include "obs-app.hpp"
#include "qt-wrappers.hpp"
#include "remote-text.hpp"

using namespace std;

static auto curl_deleter = [](CURL *curl) { curl_easy_cleanup(curl); };
using Curl = unique_ptr<CURL, decltype(curl_deleter)>;

static size_t string_write(char *ptr, size_t size, size_t nmemb, string &str)
{
	size_t total = size * nmemb;
	if (total)
		str.append(ptr, total);

	return total;
}

void RemoteTextThread::run()
{
	char error[CURL_ERROR_SIZE];
	CURLcode code;

	string versionString("User-Agent: obs-basic ");
	versionString += App()->GetVersionString();

	string contentTypeString;
	if (!contentType.empty()) {
		contentTypeString += "Content-Type: ";
		contentTypeString += contentType;
	}

	Curl curl{curl_easy_init(), curl_deleter};
	if (curl) {
		struct curl_slist *header = nullptr;
		string str;

		header = curl_slist_append(header, versionString.c_str());

		if (!contentTypeString.empty()) {
			header = curl_slist_append(header,
						   contentTypeString.c_str());
		}

		for (std::string &h : extraHeaders)
			header = curl_slist_append(header, h.c_str());

		curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl.get(), CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, header);
		curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, error);
		curl_easy_setopt(curl.get(), CURLOPT_FAILONERROR, 1L);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION,
				 string_write);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &str);
		curl_obs_set_revoke_setting(curl.get());

		if (timeoutSec)
			curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT,
					 timeoutSec);

#if LIBCURL_VERSION_NUM >= 0x072400
		// A lot of servers don't yet support ALPN
		curl_easy_setopt(curl.get(), CURLOPT_SSL_ENABLE_ALPN, 0);
#endif

		if (!postData.empty()) {
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS,
					 postData.c_str());
		}

		code = curl_easy_perform(curl.get());
		if (code != CURLE_OK) {
			blog(LOG_WARNING,
			     "RemoteTextThread: HTTP request failed. %s",
			     error);
			emit Result(QString(), QT_UTF8(error));
		} else {
			emit Result(QT_UTF8(str.c_str()), QString());
		}

		curl_slist_free_all(header);
	}
}
