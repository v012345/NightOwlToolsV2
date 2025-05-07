

#ifndef ___download_file_h___
#define ___download_file_h___

#include <string>
#include <functional>
#include <memory>
#include <atomic>

using namespace std;

namespace AonePatcher
{
	typedef std::function<void(double currt, double total)> dlcbk;
	class HttpImp;
	typedef shared_ptr<HttpImp> HttpImpPtr;

	class HttpDownload
	{
	public:
		HttpDownload(void);
		virtual ~HttpDownload(void);

		virtual void address(const string& val);
		virtual void locpath(const string& val);
		virtual void notify(dlcbk val);

		virtual int  download(void);
		virtual void interrupt(void);
		virtual bool getInterrupt();

		virtual int resultCode(void){ return errcod_; }
		virtual string& resultText(void){ return errmsg_; }
		size_t writec_(void* datap, size_t size);
		int progressc_(void);

		virtual void setIsCheckSpeed(bool isCheck);
		virtual void setBreakPointEnabled(bool isEnable){ isEnableBreakPoint_ = isEnable; }
		long getLocalFileLenth();

        virtual void setIsSupportRangeDownload(bool isEnable){ isSupportRangeDownload_ = isEnable; }
		double getCurrentSize() const;
		double getTotalSize() const;

        /**
         get expected size

         result will be exact if the size of file over 2M
         */
        double getExpectedSize();

        void queryDownloadInfo();

    protected:

		int openfile_(void);
		int opencurl_(void);
		int download_(void);
		int checkfile_(void);

		void cleanup_(void);

		string		addr_;
		string		path_;
		FILE*		file_;
		HttpImpPtr	imp_;

		int			errcod_;
		string		errmsg_;

		atomic<bool>		interruptd_;
		atomic<bool>     isCheckSpeed_;

		dlcbk		callback_;

		double total_;
		double currt_;

		bool isEnableBreakPoint_;
		bool isSupportRangeDownload_;

		int tag_;
    };

	int downfile(
		const char* addr,
		const char* path,
		dlcbk callback
		);
}

#endif

