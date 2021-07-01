//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit8.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm8 *Form8;

extern UnicodeString NetworkIF;
UnicodeString USte;

//---------------------------------------------------------------------------
__fastcall TForm8::TForm8(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm8::ComboBox1KeyPress(TObject *Sender, wchar_t &Key)
{
USte=ComboBox1->Text;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button1Click(TObject *Sender)
{
Form8->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm8::ComboBox1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
ComboBox1->Text=USte;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::FormCloseQuery(TObject *Sender, bool &CanClose)
{
NetworkIF=ComboBox1->Text;
Form1->Enabled=true;
Form1->Visible=true;
Form1->Button1Click(Form8->Button1);
}
//---------------------------------------------------------------------------
