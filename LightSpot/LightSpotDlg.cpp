
// LightSpotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LightSpot.h"
#include "LightSpotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define imgWidth  640
#define imgHeight 480
#define ORGL 30
#define ORGR 100
static bool g_bContinueAllThread = false;
CWinThread * pAcquireThread;
unsigned int AcquireThread(void *lpThreadParameter)
{
	CLightSpotDlg *pAcquireDlg = (CLightSpotDlg*)lpThreadParameter;
	//----------------------------------//
	cv::VideoCapture *pCap = &(pAcquireDlg->Cap);
	cv::Size sizeMain = pAcquireDlg->sizeMain;
	cv::Size sizeCol = pAcquireDlg->sizeCol;
	cv::Size sizeRow = pAcquireDlg->sizeRow;
	//----------------------------------//
	cv::Mat SrcImage,GreyImage;
	//cv::Mat SrcRow(cv::Size(255, imgWidth), CV_8U), SrcCol(cv::Size(255, imgHeight), CV_8U);
	cv::Mat DispImage,DispCol,DispRow;
	cv::Point stPt, edPt;
	//---------- while loop -------------//
	while (g_bContinueAllThread)
	{
		//---��ȡCCDͼ��----//
		pCap->read(SrcImage);
		//---ת��Ϊ�Ҷ�ͼ�񷽱㴦��-----//
		cv::cvtColor(SrcImage, GreyImage, cv::COLOR_BGR2GRAY);
		//===��ע�����ߣ�����ʾ=======//
		cv::line(SrcImage, cv::Point(ORGL, imgHeight >> 1), cv::Point(imgWidth - ORGL, imgHeight >> 1), cv::Scalar(0, 0, 255));
		cv::line(SrcImage, cv::Point(imgWidth >> 1, ORGL), cv::Point(imgWidth >> 1, imgHeight - ORGL), cv::Scalar(0, 0, 255));
		cv::rectangle(SrcImage, cv::Rect(ORGR, ORGR, imgWidth - 2 * ORGR, imgHeight - 2 * ORGR), cv::Scalar(0, 0, 255));
		cv::resize(SrcImage, DispImage, sizeMain);
		cv::imshow("SpotView", DispImage);
		
		//--------------
		cv::Mat dataRow = GreyImage.row(imgHeight >> 1);
		cv::Mat dataCol = GreyImage.col(imgWidth >> 1);
		//==============
		stPt = cv::Point(0, 255 - dataRow.at<uchar>(0));
		cv::Mat SrcRow(cv::Size(imgWidth, 255), CV_8U);
		for (int h = 1; h < imgWidth; h++)
		{
			edPt = cv::Point(h, 255-dataRow.at<uchar>(h));
			cv::line(SrcRow, stPt, edPt, cv::Scalar::all(0),2);
			stPt = edPt;
		}
		cv::resize(SrcRow, DispRow, sizeRow);
		cv::imshow("ViewRowLine", DispRow);
		//--------------
		stPt = cv::Point(0, 255 - dataCol.at<uchar>(0));
		cv::Mat SrcCol(cv::Size(imgHeight, 255), CV_8U);
		for (int h = 1; h < imgHeight; h++)
		{
			edPt = cv::Point(h, 255 - dataCol.at<uchar>(h));
			cv::line(SrcCol, stPt, edPt, cv::Scalar::all(0),2);
			stPt = edPt;
		}
		cv::resize(SrcCol, DispCol, sizeCol);
		cv::imshow("ViewColLine", DispCol);
	}
	return TRUE;
}


// CLightSpotDlg �Ի���
CLightSpotDlg::CLightSpotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightSpotDlg::IDD, pParent)
	, bStartRunning(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLightSpotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLightSpotDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CLightSpotDlg::OnBnStart)
END_MESSAGE_MAP()


// CLightSpotDlg ��Ϣ�������

BOOL CLightSpotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	//-----------------------------
	ConnectShowWindow("SpotView", IDC_SPOT_VIEW, sizeMain);
	ConnectShowWindow("ViewColLine", IDC_VIEW_COL, sizeCol);
	ConnectShowWindow("ViewRowLine", IDC_VIEW_ROW, sizeRow);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLightSpotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLightSpotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLightSpotDlg::ConnectShowWindow(const char* name, int nID,cv::Size &size)
{
	cv::namedWindow(cv::String(name), cv::WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle(name);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(nID)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	CRect rc;
	GetDlgItem(nID)->GetClientRect(&rc);
	size = cv::Size(rc.right, rc.bottom);
}

void CLightSpotDlg::OnBnStart()
{
	if (!bStartRunning)
	{
		if (Cap.open(0))
		{
			if (!Cap.isOpened())
			{
				AfxMessageBox(_T("������ͷʧ��"));
				return;
			}
			if (!Cap.read(SrcImage))
			{
				AfxMessageBox(_T("����ͷ��ȡʧ��"));
				return;
			}
		}
		else
		{
			AfxMessageBox(_T("�޷��ҵ�����ͷ"));
			return;
		}

		bStartRunning = true;
		GetDlgItem(IDC_START)->SetWindowTextW(_T("����"));
		g_bContinueAllThread = true;
		pAcquireThread = AfxBeginThread(AcquireThread, this);
	}
	else
	{
		GetDlgItem(IDC_START)->SetWindowTextW(_T("��ʼ"));
		g_bContinueAllThread = false;
		bStartRunning = false;
	}
}
