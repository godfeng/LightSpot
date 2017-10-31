
// LightSpotDlg.h : ͷ�ļ�
//

#pragma once
#include"opencv2\opencv.hpp"

// CLightSpotDlg �Ի���
class CLightSpotDlg : public CDialogEx
{
// ����
public:
	CLightSpotDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LIGHTSPOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnStart();
	DECLARE_MESSAGE_MAP()
public:
	bool bStartRunning;
	cv::Mat SrcImage;
	cv::VideoCapture Cap;
	void ConnectShowWindow(const char* name, int nID, cv::Size &size);
	cv::Size sizeMain, sizeCol, sizeRow;

};
