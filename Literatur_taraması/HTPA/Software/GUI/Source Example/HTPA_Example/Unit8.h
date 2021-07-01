//---------------------------------------------------------------------------

#ifndef Unit8H
#define Unit8H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm8 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *ComboBox1;
	TButton *Button1;
	void __fastcall ComboBox1KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall ComboBox1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);



private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TForm8(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm8 *Form8;
//---------------------------------------------------------------------------
#endif
