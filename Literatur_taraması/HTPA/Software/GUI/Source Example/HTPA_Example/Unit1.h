//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
//---------------------------------------------------------------------------

#define	MAXNROFIPS		100

typedef struct {AnsiString DEVICEIP;
				AnsiString MACID;
				char devType;
				float devkHz;
				bool GAIN;
				bool THvsTA;
				bool PixCvsTA;
				bool F8_14;
				bool BFL3;
				bool IGNORE_ELOFF;
				bool ELOFF32;
				bool SBY;
				bool FLOATCALIB;
				double STEFBOLEXP;
				unsigned int THOM;
				unsigned int THOD;
				unsigned int PCGD;
				unsigned int PCOD;
				unsigned int VDM;
				bool selected;
				unsigned int subframe;
				bool running;
				bool br460800;
				bool MLC;}deviceconsts;


class TForm1 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TIdUDPServer *IdUDPServer1;
	TButton *Button1;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *Edit1;
	TButton *Button2;
	TButton *Button3;
	void __fastcall IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, TIdBytes AData,
          TIdSocketHandle *ABinding);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TForm1(TComponent* Owner);
	void __fastcall TForm1::Initvarias(unsigned int type);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
