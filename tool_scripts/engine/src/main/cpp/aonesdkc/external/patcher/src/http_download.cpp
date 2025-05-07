
#include "http_download.h"
#include "cplog.h"
#include <curl/curl.h>
#include "CPFileUtils.h"
#include "DataConfig.h"
#include "CurlManagerPool.h"
#include <sys/stat.h>
namespace AonePatcher
{
	enum EDlErr
	{
		None,
        CannotOpenLocFile,
        CannotInitCurl,
        DownloadedFileSizeIsZero,
        NoThisAddress,
        NetworkError,
        NotsupportBreakPoint
	};

	class HttpImp{
	public:
		CURL* curl_;
	};

    
    /**
     This callback function gets called by libcurl as soon as there is data received that needs to be saved.
     `datap` points to the delivered data, and the size of that data is nmemb;
     size is always 1.
     */
	static size_t writef_(void* datap, size_t size, int nmenb, void* userp)
	{
		auto* dler = (HttpDownload*)userp;
//        printf("\n--- write, nmenb = %d\n", nmenb);
		return dler->writec_(datap, size*nmenb);
	}
    
    static double dltotal_ = 0.0;//即将要下载的 注意如果设置了range头 这个是剩余的下载量 不是整个文件的大小 没设置range头 就是整个文件的大小
    static double local_save_ = 0.0;//本地已下载的
    
    /**
     This function gets called by libcurl instead of its internal equivalent with a frequent interval.
     While data is being transferred it will be called very frequently, and during slow periods like when nothing is being transferred it can slow down to about one call per second.

     @param userp   This is the pointer set with `CURLOPT_PROGRESSDATA`, it is not used by libcurl but is only passed along from the application to the callback.
     @param dltotal The total number of bytes libcurl expects to download in this transfer.
     @param dlnow   The number of bytes downloaded so far.
     @param ultotal The total number of bytes libcurl expects to upload in this transfer.
     @param ulnow   The number of bytes uploaded so far.
     
     @return Returning a non-zero value from this callback will cause libcurl to abort the transfer and return CURLE_ABORTED_BY_CALLBACK.
     
     @discussion This function is used to determin whether cancel the download or not.
     */
    static int progressf_(void *userp,double dltotal,
                                  double dlnow,
                                  double ultotal,
                                  double ulnow)
    {
        
        dltotal_ = dltotal;
        auto* dler = (HttpDownload*)userp;
//        printf("--- progressf, now = %.f, total = %.f, should interrept = %d\n", dlnow, dltotal_, dler->progressc_());
        return dler->progressc_();
    }
    

	HttpDownload::HttpDownload(void)
        : tag_(0)
		, addr_("")
		, path_("")
		, file_(nullptr)
		, imp_(HttpImpPtr())
		, errcod_(0)
		, total_(0.0f)
		, currt_(0.0f)
		, interruptd_(false)
		, callback_(nullptr)
		, isEnableBreakPoint_(false)
		, isSupportRangeDownload_(false)
		, isCheckSpeed_(false)
	{
	}
	HttpDownload::~HttpDownload(void)
	{
	}

	size_t HttpDownload::writec_(void* ptr, size_t size)
	{
		// don't need check the size is empty.
		size_t writed = fwrite(ptr, 1, size, file_);
		if (writed == size)
		{
			currt_ += writed;
            total_ = dltotal_+local_save_;
			callback_(currt_, total_);
		}
		else
		{
			// output write data error
			CP_LOGE("output write data error\n");
		}

		return writed;
	}
    
	int HttpDownload::progressc_(void)
	{
		return (int)interruptd_.load();
	}

	void HttpDownload::address(const string& val)
	{
		addr_ = val;
	}
	void HttpDownload::locpath(const string& val)
	{
		path_ = val;
	}
	void HttpDownload::notify(dlcbk val)
	{
		callback_ = val;
	}

	int HttpDownload::download(void)
	{
		do
		{
			errcod_ = openfile_();
			if (EDlErr::None != errcod_){
				break;
			}
			errcod_ = opencurl_();
			if (EDlErr::None != errcod_){
				break;
			}
			errcod_ = download_();
			if (EDlErr::None != errcod_){
				break;
			}
			errcod_ = checkfile_();
			if (EDlErr::None != errcod_){
				break;
			}

			errcod_ = EDlErr::None;

		} while (false);

		cleanup_();

		return errcod_;
	}

	int HttpDownload::openfile_(void)
	{
		//may have multi thread problem, check later
		if (isEnableBreakPoint_  && CPUtils::CPFileUtils::exist(path_.c_str())){
			file_ = fopen(path_.c_str(), "ab+");
        }else{
			file_ = fopen(path_.c_str(), "wb");
        }
		if (NULL == file_)
		{
			CP_LOGE("open file %s fail , errno is %u", path_.c_str(), errno);
			return EDlErr::CannotOpenLocFile;
		}
		return EDlErr::None;
	}

	int HttpDownload::opencurl_(void)
	{
		CurlInfo curlInfo = CurlManagerPool::GetInstance()->GetAvailableCurl();
		tag_ = curlInfo.tag;

		if (NULL == curlInfo.curl){
            CP_LOGE("HttpDownload::opencurl_ null");
			return EDlErr::CannotInitCurl;
        }
		imp_ = HttpImpPtr(new HttpImp());
		imp_->curl_ = curlInfo.curl;
		return EDlErr::None;
	}

	long HttpDownload::getLocalFileLenth()
	{
		FILE *fp = fopen(path_.c_str(), "rb+");

		if (!fp){
			return 0;
        }
		fseek(fp, 0, SEEK_END);
		long length = ftell(fp);

		fclose(fp);

		return length;
	}

	double HttpDownload::getCurrentSize() const
	{
		return currt_;
	}

	double HttpDownload::getTotalSize() const
	{
		return total_;
	}

    static inline void stringClear(std::string& s,const std::string& removeVal)
    {
        // 删除所有换行
        std::string::size_type r = s.find(removeVal);
        while (r != std::string::npos)
        {
            s.replace(r, removeVal.length(), "");
            r = s.find(removeVal);
        }
    }
	static inline void stringToLower(std::string& s){
        size_t s_l = s.length();
		for (size_t i = 0; i<s_l; i++){
            if(s[i] >= 'A' && s[i] <= 'Z'){
                s[i] += 32;
            }
        }
	}
    size_t respHeaderFilter(char* buff, size_t size, size_t nmemb, void* data){
		auto totalSize = size * nmemb;
		std::string headerStr(buff, totalSize);

        stringClear(headerStr,"\r\n");
        stringClear(headerStr," ");
        stringToLower(headerStr);

        if(headerStr == "accept-ranges:bytes"){//服务器支持分段下载
            auto* dler = (HttpDownload*)data;
            dler->setIsSupportRangeDownload(true);
        }
        return size * nmemb;
    }
    
    double HttpDownload::getExpectedSize()
    {
        double size = 0.0;
        
        if (addr_.empty()) {
            CP_LOGE("get expected size failed, url is empty!\n");
            return size;
        }
        
        CURLcode res;
       
        CURL* curl = curl_easy_init();
        const char *url = addr_.c_str();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        //仅查询 不请求body
        //get us the resource without a body - use HEAD!
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        //用于表明CURLOPT_HEADERDATA中的stream指针的来源
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
        //Pass a pointer to your callback function, which should match the prototype shown above.
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, respHeaderFilter);
        //支持重定向
        //A long parameter set to 1 tells the library to follow any Location: header redirects that an HTTP server sends in a 30x response.
        //The Location: header can specify a relative or an absolute URL to follow.
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        //阻塞方式执行请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() url:%s failed:%s\n", url, curl_easy_strerror(res));
            size = -1;
        }
        //Pass a pointer to a double to receive the content-length of the download.
        //This is the value read from the Content-Length: field. Since 7.19.4, this returns -1 if the size is not known.
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,&size);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_getinfo() url:%s failed:%s\n", url, curl_easy_strerror(res));
            size = -1;
        }
        curl_easy_cleanup(curl);
        return size;
    }
    
	int HttpDownload::download_(void)
	{
		long locFileLength = 0;
		if (isEnableBreakPoint_){
			locFileLength = getLocalFileLenth();
            if(total_>0 && locFileLength>=total_){ //已经下载完了
                CP_LOGW("HttpDownload::download_ skip");
                return EDlErr::None;
            }
        }
		CURL* curl_ = imp_->curl_;
		//curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(curl_, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl_, CURLOPT_URL, addr_.c_str());
		curl_easy_setopt(curl_, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl_, CURLOPT_NOBODY, 0);

        // set the write callback function.
		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writef_);
        // A data pointer to pass to the write callback.
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);

		if (isEnableBreakPoint_){
            char rangeStr[1024] = {0};
            sprintf(rangeStr,"%ld-", locFileLength);
            CP_LOGI("HttpDownload::download_ rangeStr:%d",rangeStr);
            curl_easy_setopt(curl_, CURLOPT_RANGE, rangeStr);
        }

        // set progress
		curl_easy_setopt(curl_, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl_, CURLOPT_PROGRESSFUNCTION, progressf_);
		curl_easy_setopt(curl_, CURLOPT_PROGRESSDATA, this);

		curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1);

        // in 20s, the speed < 2Byte/sec
		if (isCheckSpeed_.load())
		{
            int speedLimit = DataConfig::getInstance()->getDLSpeedLimit();
            int speedTime = DataConfig::getInstance()->getDLSpeedTime();
            curl_easy_setopt(curl_, CURLOPT_LOW_SPEED_LIMIT, speedLimit);
            curl_easy_setopt(curl_, CURLOPT_LOW_SPEED_TIME, speedTime);
		}
		
        // A synchronized request.
        // This will block the code below, if request finished, then check the last response code.
		CURLcode curlcode = curl_easy_perform(curl_);
        if (curlcode != CURLE_OK) {
            CP_LOGE("\n[curl_easy_perform] error. code = %u\n",curlcode);
        }

        /**
         Pass a pointer to a long to receive the last received HTTP, FTP or SMTP response code.
         This option was previously known as CURLINFO_HTTP_CODE in libcurl 7.10.7 and earlier.
         The stored value will be zero if no server response code has been received.
         */
		long resp = 0;
		curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &resp);
		if (resp == 416 || curlcode == 33){
			return EDlErr::NotsupportBreakPoint;
        }
		if (resp > 500){
			return EDlErr::NetworkError;
		}
		if (resp > 400){
			return EDlErr::NoThisAddress;
		}
		if (CURLE_OK == curlcode){
			//fflush(file_);//强制缓冲区写入到本地磁盘 win32下没效果 
			fclose(file_);//win32下fflush无效 只能关闭 不然会导致checkfile_ st_size返回0
			file_ = NULL;
			return EDlErr::None;
		}

		return 10000 + curlcode;
	}

	int HttpDownload::checkfile_(void)
	{
        struct stat st{};
        auto ret = stat(path_.c_str(),&st);
		if (ret == 0 && st.st_size > 0){
			return EDlErr::None;
		}

		return EDlErr::DownloadedFileSizeIsZero;
	}

	void HttpDownload::cleanup_(void)
	{
		if (NULL != file_){
			fclose(file_);
        }

		if (imp_.get() != NULL && NULL != imp_->curl_){
			CurlManagerPool::GetInstance()->TabCurlUnusedByTag(tag_);
        }

		file_ = NULL;
		imp_.reset();
	}

	void HttpDownload::interrupt(void)
	{
		interruptd_.store(true);
	}

	bool HttpDownload::getInterrupt()
	{
		return interruptd_.load();
	}

	void HttpDownload::setIsCheckSpeed(bool isCheck)
	{
		isCheckSpeed_.store(isCheck);
	}

    void HttpDownload::queryDownloadInfo() {
        auto totalSize = getExpectedSize();
        setBreakPointEnabled(isSupportRangeDownload_);//支持区间下载才开启断点模式

        if(totalSize <= 0){
            total_ = 0;
        }else{
            total_ = totalSize;
        }
        if(isEnableBreakPoint_){
            currt_ = getLocalFileLenth();
            local_save_ = currt_;
        }else{
            currt_ = 0.0;
        }
    }

    int downfile(
		const char* url,
		const char* loc,
		dlcbk callback
		)
	{
		HttpDownload d;
		d.address(url);
		d.locpath(loc);
		d.notify(callback);

		int ret = d.download();

		if (ret != 0){
			CP_LOGE(" -- msg : download error %d, %s", ret, url);
        }
		return ret;
	}
}
