#include "btwo_patcher.h"
#include "PatcherController.h"

using namespace AonePatcher;

extern "C"
{
struct patcher
{
	patcher()
	{
		m_progress_cb = NULL;
		m_finish_cb = NULL;
		m_error_cb = NULL;
		m_ud = NULL;
	}

	PatcherController* m_controller;
	patcher_progress_cb m_progress_cb;
	patcher_finish_cb m_finish_cb;
	patcher_error_cb m_error_cb;
	void* m_ud;
};

static patcher* _default_patcher = NULL;

struct patcher* patcher_get_default()
{
	if (_default_patcher == NULL)
	{
		_default_patcher = new patcher();
		_default_patcher->m_controller = new PatcherController();
	}
	return _default_patcher;
}

struct patcher* patcher_create()
{
	return new patcher();
}


void patcher_free(struct patcher* p)
{
	if (p == _default_patcher)
	{
		_default_patcher = NULL;
	}
	delete p->m_controller;
	delete p;
}

void patcher_set_url(struct patcher* p, const char* list_url)
{
	p->m_controller->SetURL(list_url);
}

void patcher_start(struct patcher* p)
{
	p->m_controller->Start();
}

void patcher_cancel(struct patcher* p)
{
	p->m_controller->Cancel();
}

static std::string _path;
const char* patcher_path(struct patcher* p)
{
	_path = p->m_controller->GetPath();
	return _path.c_str();
}

void patcher_get_list(struct patcher*p)
{
	p->m_controller->GetList();
}

void patcher_callback(struct patcher* p, void* ud, patcher_progress_cb progress_cb, patcher_finish_cb finish_cb, patcher_error_cb error_cb)
{
	p->m_ud = ud;
	p->m_progress_cb = progress_cb;
	p->m_finish_cb = finish_cb;
	p->m_error_cb = error_cb;

	if (progress_cb != NULL)
	{
		p->m_controller->SetProgressCB([p,ud](uint32 wb, uint32 eb){
			p->m_progress_cb(p, ud, wb, eb);
		});
	}
	else
	{
		p->m_controller->SetProgressCB(nullptr);
	}

	if (finish_cb != NULL)
	{
		p->m_controller->SetFinishCB([p, ud](){
			p->m_finish_cb(p, ud);
		});
	}
	else
	{
		p->m_controller->SetFinishCB(nullptr);
	}

	if (error_cb != NULL)
	{
		p->m_controller->SetErrorCB([p, ud](PatchErrorCode code){
			p->m_error_cb(p, ud, static_cast<uint32>(code));
		});
	}
	else
	{
		p->m_controller->SetErrorCB(nullptr);
	}
}

void patcher_notify(struct patcher* p)
{
	p->m_controller->Notify();
}

std::string _error_msg;
uint32 patcher_get_result(struct patcher* p, uint32* inner_code, const char** msg)
{
	uint32 innerCode;
	PatchErrorCode code = p->m_controller->GetResult(innerCode, _error_msg);
	if (inner_code != NULL)
	{
		*inner_code = innerCode;
	}
	if (msg != NULL)
	{
		*msg = _error_msg.c_str();
	}

	return static_cast<uint32>(code);
}

uint32 patcher_copy_result(struct patcher* p, uint32* inner_code, char* msg)
{
	const char* retMsg;
	uint32 ret = patcher_get_result(p, inner_code, &retMsg);
	memcpy(msg, retMsg, strlen(retMsg));
	return ret;
}

void patcher_set_path(struct patcher* p, const char* path)
{
	p->m_controller->SetPath(path);
}

void patcher_set_package_res_path(struct patcher* p, const char* package_res_path)
{
	p->m_controller->SetPackageResPath(package_res_path);
}

void patcher_check(struct patcher* p)
{
	p->m_controller->CheckPatchValid();
}

void patcher_remove_useless_files(struct patcher* p)
{
	p->m_controller->RemoveUselessFiles();
}

}
