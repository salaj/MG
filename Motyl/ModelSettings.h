#pragma once

#include "resource.h"
#include "stdafx.h"


// ModelSettings dialog

class ModelSettings : public CDialogEx
{
	DECLARE_DYNAMIC(ModelSettings)

public:
	ModelSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~ModelSettings();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
