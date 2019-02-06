// HPFEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HPFE.h"
#include "HPFEDlg.h"
#include <fstream.h>
#include "HPFEData.h"
#include "HPFEEditDlg.h"

#define ZLIB_DLL
#ifndef _WINDOWS
  #define _WINDOWS
#endif
#include "unzip.h"

#ifdef _
  #define new DEBUG_NEW
  #undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Debugging file
FILE *debug;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum
  {
    IDD = IDD_ABOUTBOX
  };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CHPFEDlg dialog

CHPFEDlg::CHPFEDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHPFEDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CHPFEDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CHPFEDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CHPFEDlg)
  DDX_Control(pDX, IDC_EDIT_BUTTON, EDIT_BUTTON);
  DDX_Control(pDX, IDC_LIST1, ROMLIST);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHPFEDlg, CDialog)
//{{AFX_MSG_MAP(CHPFEDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
ON_COMMAND(ID_FILE_EXIT, OnFileExit)
ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CHPFEDlg message handlers

// Do the stuff included with the code wizard (about box, system menu).
// Then read games data and put it in the list control.
BOOL CHPFEDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  // when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);     // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon


  // Open the debugging output file.
  debug = fopen("c:\\temp\\debug", "w");

  // Create the HPFEData object which holds system objects and column info.
  mainData = new HPFEData(debug);

  // Read the configuration file.
  ifstream configFile("hpfeconf.dat", ios::in);
  if (!configFile)
  {
    MessageBox("Could not open hpfeconf.dat");
  }
  else
  {
    mainData->readConfigFile(configFile);
    configFile.close();
  }

  // Read the systems file.
  ifstream systemsFile("hpfesys.dat", ios::in);
  if (!systemsFile)
  {
    MessageBox("Could not open hpfesys.dat");
  }
  else
  {
    mainData->readSystemsFile(systemsFile);
    systemsFile.close();
  }

  // Build game entries from data read from files.
  mainData->buildInitialData();

  // Put the data into the list control.
  mainData->fillListCtrl(&ROMLIST);

  return TRUE;  // return TRUE  unless you set the focus to a control
}


void CHPFEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CHPFEDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}


// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CHPFEDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}


void CHPFEDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  // TODO: Add your control notification handler code here
}


void CHPFEDlg::OnHelpAbout() 
{
  // TODO: Add your command handler code here

}


// Given a filename, return its extension (all characters
// after the last separator).
// (name starts with "hd" to not conflict with similar method
//  in SysObj.cpp)
char *hdfindExtension(char *filename)
{
  if (filename == NULL) return NULL;

  int endOfBaseName = -1;
  int len = strlen(filename);

  // Find location of last separator.
  for (int i = (len - 1); i >= 0; i--)
  {
    if (filename[i] == '.')
    {
      endOfBaseName = i;
      break;
    }
  }

  // If separator isn't in name or is last char, return null.
  if ((endOfBaseName == -1) || (endOfBaseName == (len - 1)))
    return NULL;

  // Duplicate truncated name.
  char *returnMe = strdup(filename + (sizeof(char) * (1 + endOfBaseName)));
  // Return duplicate.
  return returnMe;
}


// Given a long filename and directory, find the 
// 8.3 representation of the filename.
char* find8Dot3Filename(char* longFilename, char* directory)
{
  // Create a filemask for the filename and directory.
  static char filemask[defaultStringLength];
  sprintf(filemask, "%s\\%s", directory, longFilename);

  // Use Windows API to find that file.
  WIN32_FIND_DATA *findFileData = new WIN32_FIND_DATA;
  HANDLE findFileHandle;
  findFileHandle = FindFirstFile(filemask, findFileData);
  if (findFileHandle != INVALID_HANDLE_VALUE)
  {
    if (findFileData->cAlternateFileName == NULL)
      return findFileData->cFileName;
    else
      return findFileData->cAlternateFileName;
  }

  // Not found; just return long name...
  else
    return longFilename;
}


// My string compare is case-INsensitive, otherwise like strcmp.
// (name starts with "hd" to not conflict with similar method
//  in HPFEData.cpp)
int hdmystrcmp(char *s1, char *s2)
{
  if (s1 == NULL) return 1;
  if (s2 == NULL) return -1;

  int s1len = strlen(s1);
  int s2len = strlen(s2);

  char diff = 'A' - 'a';
  char a;
  char b;

  for (int i = 0; i < s1len; i++)
  {
    if (i >= s2len) return 1;
    if ((s1[i] >= 'a') && (s1[i] <= 'z')) a = s1[i] + diff;
    else a = s1[i];
    if ((s2[i] >= 'a') && (s2[i] <= 'z')) b = s2[i] + diff;
    else b = s2[i];
    if (a > b) return 1;
    if (a < b) return -1;
  }

  return 0;
}


// Does this file have a valid extension for this system object?
bool fileHasValidExtension(char* filename, SysObj* currSO)
{
  char* extension = hdfindExtension(filename);

  charstarVectorIt it; 
  for (it = (currSO->extensions).begin(); it != (currSO->extensions).end(); it++)
  {
    if (!hdmystrcmp(*it, extension)) return TRUE;
  } 

  return FALSE;
}


// Use parameters to start a "runit" batch file for a game.
void writeOpeningBatchStuff(FILE* runbat, SysObj* currSO, GameEntry* currGE, char* commandLineFilename, char* commandLineDirectory)
{
  // Go to the working dir drive.
  fprintf(runbat, "%c:\n", currGE->workingDir[0]);

  // Change to the working dir.
  fprintf(runbat, "cd %s\n", currGE->workingDir);

  // Make sure files are writable.
  fprintf(runbat, "attrib -r *.*\n");

  // Create emulator command line.
  char runEmulator[defaultStringLength];
  runEmulator[0] = '\0';
  currSO->insertGameNameIntoCommandLine(runEmulator, currGE->commandLine, commandLineFilename, commandLineDirectory);
  fprintf(runbat, "%s\n", runEmulator);
}


// Given a system object, a game entry, a vector of temporary game files created,
// and an indication of whether or not we've found the first game file (of
// potentially many), unzip files, and create a command line. Return whether or
// not we've found the first game file to run.
boolean processGameFile(SysObj* currSO, GameEntry* currGE, charstarVector *filenamesUnzipped, 
                        boolean haveFirstGame, char* commandLine, char* workingDirectory)
{
  static char deleteFilename[defaultStringLength];

  // If current game isn't zipped, simply copy info.
  if (strcmp(currGE->needToUnzip, "yes"))
  {
	if (haveFirstGame == FALSE)
	{
      currSO->insertGameNameIntoCommandLine(commandLine, currGE->commandLine, currGE->filename, currGE->romDir);
      strcpy(workingDirectory, currGE->workingDir);
      haveFirstGame = TRUE;
	}
  }

  // If current game is in ZIP, extract it.
  else
  {
    // If current game is in a multigame zip, extract the single file.
    if (!strcmp(currGE->multiGameZip, "yes"))
    {
      static char zipFilename[defaultStringLength];  // name of zip file
      static char zippedFile[defaultStringLength];   // name of file in zip
      static char outputFile[defaultStringLength];   // name and path of destination file

      sprintf(zipFilename, "%s\\%s", currGE->romDir, currGE->zipFile);
      sprintf(zippedFile, "%s.%s", currGE->name, hdfindExtension(currGE->filename));
      sprintf(outputFile, "%s\\%s", currGE->workingDir, currGE->filename);

      fprintf(debug, "Looking in zip file %s for file %s to write to %s\n",
              zipFilename, zippedFile, outputFile);

	  // Open the zip file, locate the file to unzip, then unzip it.
      unzFile zipfile = unzOpen(zipFilename);   
      if (zipfile != NULL)
      {
        int zipresult = unzLocateFile(zipfile, zippedFile, 2);
        if (zipresult != UNZ_END_OF_LIST_OF_FILE)
        {
          const int BUFLEN = 65536;
          static char buffer[BUFLEN];
          FILE *outfile = fopen(outputFile, "wb");
          unzOpenCurrentFile(zipfile);
          int len;
          while (true)
          {
            len = unzReadCurrentFile(zipfile, buffer, BUFLEN);
            if (len == 0) break;
            fwrite(buffer, 1, len, outfile);
          }
          unzCloseCurrentFile(zipfile);
          fclose(outfile);

		  // If this is the first game, write out opening lines to batch file.
          if (haveFirstGame == FALSE)
		  {
            currSO->insertGameNameIntoCommandLine(commandLine, currGE->commandLine, find8Dot3Filename(currGE->filename, currGE->workingDir), currGE->workingDir);
            strcpy(workingDirectory, currGE->workingDir);
            haveFirstGame = TRUE;
		  }

		  // Add this filename to the list to delete after running the emulator.
          sprintf(deleteFilename, "%s\\%s", currGE->workingDir, 
		          find8Dot3Filename(currGE->filename, currGE->workingDir));
		  (*filenamesUnzipped).push_back(strdup(deleteFilename));
        }
        unzClose(zipfile);
      }
    }

    // If current game is in a zip file that isn't a multigame
    // zip file, unzip all files and run the first.
    else
    {
      static char zipFilename[defaultStringLength];  // name of zip file

      sprintf(zipFilename, "%s\\%s", currGE->romDir, currGE->zipFile);

      fprintf(debug, "Looking in zip file %s for all files\n", zipFilename);

	  // Open the zip file and unzip all files that have a valid extension.
      unzFile zipfile = unzOpen(zipFilename);   
      if (zipfile != NULL)
      {
        int zipresult = unzGoToFirstFile(zipfile);
        while (zipresult != UNZ_END_OF_LIST_OF_FILE)
		{
          static unz_file_info *fileInfo = new unz_file_info;
          static char filenameInZip[defaultStringLength];
          static char extra[defaultStringLength];
          static char comment[defaultStringLength];
          static char outfilename[defaultStringLength];

          unzGetCurrentFileInfo(zipfile,
                                fileInfo,
                                filenameInZip, defaultStringLength,
                                extra, defaultStringLength,
                                comment, defaultStringLength);

		  // If the file has a valid extension, unzip it and 
		  // create a command line if it's the first valid file.
		  // Add it to the list of unzipped files (to delete them later).
		  if (fileHasValidExtension(filenameInZip, currSO))
		  {
            const int BUFLEN = 65536;
            static char buffer[BUFLEN];
		    sprintf(outfilename, "%s\\%s", currGE->workingDir, filenameInZip);
            FILE *outfile = fopen(outfilename, "wb");
            unzOpenCurrentFile(zipfile);
            int len;
            while (true)
			{
              len = unzReadCurrentFile(zipfile, buffer, BUFLEN);
              if (len == 0) break;
              fwrite(buffer, 1, len, outfile);
			}
            unzCloseCurrentFile(zipfile);
            fclose(outfile);

		    // If this is the first game, write out opening lines to batch file.
		    if (haveFirstGame == FALSE)
			{
              currSO->insertGameNameIntoCommandLine(commandLine, currGE->commandLine, find8Dot3Filename(filenameInZip, currGE->workingDir), currGE->workingDir);
			  strcpy(workingDirectory, currGE->workingDir);
              haveFirstGame = TRUE;
			}

		    // Add this filename to the list to delete after running the emulator.
            sprintf(deleteFilename, "%s\\%s", currGE->workingDir, 
			        find8Dot3Filename(filenameInZip, currGE->workingDir));
		    (*filenamesUnzipped).push_back(strdup(deleteFilename));
		  }

          zipresult = unzGoToNextFile(zipfile);
		}
	  }
    }
  }

  return haveFirstGame;
}


// On a double click, try to run the emulator on the selected game.
void CHPFEDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  boolean haveFirstFilename = FALSE;
  charstarVector filenamesUnzipped;
  static char commandLine[defaultStringLength];
  static char workingDirectory[defaultStringLength];

  // Get the selected item.
  CListCtrl* pListCtrl = &ROMLIST;
  POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
  if (pos == NULL)
  {
    MessageBox("No game was selected!");
  }
  else
  {
	boolean haveFirstGame = FALSE;

	// For each game file selected...
    while (pos)
    {
      // Find out which item was clicked and get is associated system and game info.
      int nItem = pListCtrl->GetNextSelectedItem(pos);
      SysObj *currSO = mainData->getSysObjFromSortedListCtrl(nItem);
      GameEntry *currGE = mainData->getGameEntryFromSortedListCtrl(nItem);

	  // Process this game file (search ZIPs, etc).
	  haveFirstGame = processGameFile(currSO, currGE, &filenamesUnzipped, haveFirstGame, commandLine, workingDirectory);
    }

    // Run the emulator if there is a valid game.
	if (haveFirstGame)
	{
      fprintf(debug, "***Running %s in directory %s\n", commandLine, workingDirectory);
      STARTUPINFO si;
      PROCESS_INFORMATION pi;

      ZeroMemory( &si, sizeof(si) );
      si.cb = sizeof(si);

      // Start the child process. 
      if(CreateProcess(NULL, // No module name (use command line). 
         commandLine,        // Command line. 
         NULL,               // Process handle not inheritable. 
         NULL,               // Thread handle not inheritable. 
         FALSE,              // Set handle inheritance to FALSE. 
         0,                  // No creation flags. 
         NULL,               // Use parent's environment block. 
         workingDirectory,   // Use parent's starting directory. 
         &si,                // Pointer to STARTUPINFO structure.
         &pi))               // Pointer to PROCESS_INFORMATION structure.     
	  {

        // Wait until child process exits.
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles. 
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
	  }
	  else
	  {
        fprintf(debug, "\n\nscrewed up\n\n");
	  }
	}

	// Delete the temporary game files.
    charstarVectorIt it; 
    for (it = filenamesUnzipped.begin(); it != filenamesUnzipped.end(); it++)
	{
      fprintf(debug, "***Deleting %s\n", *it);
	  if (remove(*it) != 0) fprintf(debug, "  ....could not delete file\n");
	} 
  }

  *pResult = 0;
}


// When the edit button is clicked, allow the user to edit the current game entry.
void CHPFEDlg::OnEditButton() 
{
  // TODO: Add your control notification handler code here
  MessageBox("You clicked the edit button! Duh!");

  CHPFEEditDlg editDlg;
  editDlg.DoModal();
}


void CHPFEDlg::OnFileExit() 
{
  // TODO: Add your command handler code here
  MessageBox("Goodbye!");
  OnOK();

}

void CHPFEDlg::OnClose() 
{
  // TODO: Add your message handler code here and/or call default
  //CDialog::OnClose();
  OnFileExit();
}
