//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TLabel *Label11;
	TGroupBox *GroupBox1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit4;
	TEdit *Edit3;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit7;
	TEdit *Edit8;
	TButton *Button1;
	TEdit *Edit9;
	TComboBox *ComboBox1;
	TButton *Button2;
	TTimer *Timer1;
	TTimer *Timer2;
	TButton *Button3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit2KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit3KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit4KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit5KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit6KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit7KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit8KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Edit1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit2KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit3KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit4KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit5KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit6KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit7KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit8KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Edit10KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TForm5(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
