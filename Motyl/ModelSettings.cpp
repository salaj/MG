// ModelSettings.cpp : implementation file
//

#include "stdafx.h"
#include "ModelSettings.h"
#include "afxdialogex.h"


// ModelSettings dialog

IMPLEMENT_DYNAMIC(ModelSettings, CDialogEx)

ModelSettings::ModelSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(ModelSettings::IDD, pParent)
{

}

ModelSettings::~ModelSettings()
{
}

void ModelSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ModelSettings, CDialogEx)
END_MESSAGE_MAP()


// ModelSettings message handlers
